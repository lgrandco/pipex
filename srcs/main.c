/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: legrandc <legrandc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:19:32 by legrandc          #+#    #+#             */
/*   Updated: 2023/11/26 18:44:03 by legrandc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *program, char **env)
{
	char	*path_tried;
	char	**paths;
	char	**var;
	char	*final_path;
	size_t	i;

	while (*env)
	{
		var = ft_split(*env, '=');
		if (!strcmp("PATH", var[0]))
			break ;
		else
			env++;
		free_matrix(var);
	}
	paths = ft_split(var[1], ':');
	i = 0;
	free_matrix(var);
	while (paths[i])
	{
		path_tried = ft_strjoin(paths[i], "/");
		final_path = ft_strjoin(path_tried, program);
		free(path_tried);
		if (access(final_path, X_OK) == 0)
			return (final_path);
		free(final_path);
		i++;
	}
	free_matrix(paths);
	ft_putstr_fd(program, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	exit(127);
}

void	write_heredoc(char *eof, int fd)
{
	char	*line;
	size_t	eof_len;

	eof_len = ft_strlen(eof);
	while (1)
	{
		ft_printf("> ");
		line = get_next_line(0);
		if (!ft_strncmp(line, eof, eof_len) && line[eof_len] == '\n')
			break ;
		ft_putstr_fd(line, fd);
		free(line);
	}
	free(line);
}

void	exec_child(char *command, char **ev, int is_last, t_vars *vars)
{
	char	**argv;
	char	*path;

	if (!is_last)
		dup2(vars->fildes[1], STDOUT_FILENO);
	else
	{
		if (vars->is_heredoc)
			vars->outfile_fd = open(vars->outfile_name,
					O_WRONLY | O_APPEND | O_CREAT, 0666);
		else
			vars->outfile_fd = open(vars->outfile_name,
					O_WRONLY | O_TRUNC | O_CREAT, 0666);
		if (vars->outfile_fd == -1)
		{
			perror(vars->outfile_name);
			exit(EXIT_FAILURE);
		}
		dup2(vars->outfile_fd, STDOUT_FILENO);
		close(vars->outfile_fd);
		if (vars->is_heredoc)
			unlink(vars->infile_name);
	}
	// ft_putstr_fd("fwefa\n", 2);
	argv = ft_split(command, ' ');
	if (!argv)
		exit(EXIT_FAILURE);
	execve(path, argv, ev);
	if (close(vars->fildes[0]) == -1 || close(vars->fildes[1]) == -1)
		perror("close");
	free_matrix(argv);
	path = get_path(argv[0], ev);
	free(path);
	perror(argv[0]);
	exit(EXIT_FAILURE);
}

void	get_infile(t_vars *vars)
{
	if (vars->is_heredoc)
	{
		vars->infile_fd = open(vars->infile_name, O_TRUNC | O_RDWR | O_CREAT,
				0600);
		if (vars->infile_fd == -1)
		{
			perror(vars->infile_name);
			exit(EXIT_FAILURE);
		}
		write_heredoc(vars->delimiter, vars->infile_fd);
		close(vars->infile_fd);
		vars->infile_fd = open(vars->infile_name, O_RDONLY);
	}
	else
		vars->infile_fd = open(vars->infile_name, O_RDONLY);
	if (vars->infile_fd == -1)
	{
		perror(vars->infile_name);
		exit(EXIT_FAILURE);
	}
	dup2(vars->infile_fd, STDIN_FILENO);
	close(vars->infile_fd);
}

void	init_pipex(int ac, char ***av, t_vars *vars)
{
	if (ac < 5 || (!ft_strncmp((*av)[1], "here_doc", ft_strlen("here_doc"))
			&& ac == 5))
	{
		ft_putstr_fd(ARGS_ERROR, STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	vars->outfile_name = (*av)[ac - 1];
	if (!ft_strncmp((*av)[1], "here_doc", ft_strlen((*av)[1])))
	{
		vars->is_heredoc = 1;
		vars->infile_name = HEREDOC_LOCATION;
		vars->delimiter = (*av)[2];
		(*av)++;
	}
	else
	{
		vars->infile_name = (*av)[1];
		vars->is_heredoc = 0;
	}
	vars->commands_nb = ac - 3 - vars->is_heredoc;
	vars->pids = NULL;
	(*av)++;
	get_infile(vars);
}

int	main(int ac, char **av, char **ev)
{
	t_vars	vars;

	init_pipex(ac, &av, &vars);
	while (*(++av + 1))
	{
		if (pipe(vars.fildes) == -1)
			return (perror("pipe"), EXIT_FAILURE);
		vars.new = ft_lstnew(NULL, fork());
		if ((vars.new->n) == 0)
			exec_child(*av, ev, *(av + 2) == NULL, &vars);
		if ((vars.new->n) == -1)
			return (perror("fork"), EXIT_FAILURE);
		dup2(vars.fildes[0], STDIN_FILENO);
		if (close(vars.fildes[0]) == -1 || close(vars.fildes[1]) == -1)
			perror("close");
		ft_lstadd_back(&vars.pids, vars.new);
	}
	while (vars.pids)
	{
		vars.new = vars.pids->next;
		waitpid(vars.pids->n, &vars.last_exit_code, 0);
		if (WIFEXITED(vars.last_exit_code))
			vars.last_exit_code = WEXITSTATUS(vars.last_exit_code);
		fprintf(stderr, "%d\n", vars.last_exit_code);
		free(vars.pids);
		vars.pids = vars.new;
	}
	fprintf(stderr, "zz:%d\n", vars.last_exit_code);
	return (vars.last_exit_code);
}
