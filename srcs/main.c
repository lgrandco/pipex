/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:19:32 by legrandc          #+#    #+#             */
/*   Updated: 2023/11/28 01:54:12 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *command, char **env)
{
	char	**paths;
	char	**var;
	char	*final_path;
	size_t	i;

	paths = NULL;
	if (!command)
		command = "";
	if (ft_strchr(command, '/'))
		return (ft_strdup(command));
	while (*env)
	{
		var = ft_split(*env, '=');
		if (var[0] && var[1] && !strcmp("PATH", var[0]))
		{
			paths = ft_split(var[1], ':');
			free_matrix(var);
			break ;
		}
		env++;
		free_matrix(var);
	}
	i = 0;
	while (*command != '\0' && *command != '.' && paths && paths[i])
	{
		final_path = ft_strjoin3(paths[i], "/", command);
		if (access(final_path, F_OK) == 0)
		{
			free_matrix(paths);
			return (final_path);
		}
		free(final_path);
		i++;
	}
	free_matrix(paths);
	ft_putstr_fd(command, STDERR_FILENO);
	ft_putstr_fd(": command not found\n", STDERR_FILENO);
	return (NULL);
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
		if (!line)
			ft_putstr_fd(HERE_DOC_ERR, STDERR_FILENO);
		if (!ft_strncmp(line, eof, eof_len) && line[eof_len] == '\n')
			break ;
		ft_putstr_fd(line, fd);
		free(line);
	}
	free(line);
}

void	get_infile(t_vars *vars)
{
	if (vars->is_heredoc)
	{
		pipe(vars->here_docfd);
		write_heredoc(vars->delimiter, vars->here_docfd[1]);
		close(vars->here_docfd[1]);
		vars->infile_fd = vars->here_docfd[0];
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

void	exec_child(char *command, int is_first, int is_last, t_vars *vars)
{
	char	**args;
	char	*path;

	if (is_first)
		get_infile(vars);
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
			close(vars->fildes[0]);
			close(vars->fildes[1]);
			close(STDIN_FILENO);
			perror(vars->outfile_name);
			exit(EXIT_FAILURE);
		}
		dup2(vars->outfile_fd, STDOUT_FILENO);
		close(vars->outfile_fd);
	}
	close(vars->fildes[0]);
	close(vars->fildes[1]);
	// ft_putstr_fd("fwefa\n", 2);
	path = NULL;
	args = ft_split(command, ' ');
	if (!args)
		exit(EXIT_FAILURE);
	path = get_path(args[0], vars->env);
	if (path == NULL)
	{
		free_matrix(args);
		exit(127);
	}
	execve(path, args, vars->env);
	perror(args[0]);
	free(path);
	free_matrix(args);
	exit(EXIT_FAILURE);
}

void	init_pipex(int ac, char ***av, t_vars *vars, char **ev)
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
		vars->delimiter = (*av)[2];
		(*av)++;
	}
	else
	{
		vars->infile_name = (*av)[1];
		vars->is_heredoc = 0;
	}
	vars->commands_nb = ac - 3 - vars->is_heredoc;
	(*av)++;
	vars->env = ev;
}

int	main(int ac, char **av, char **ev)
{
	t_vars	vars;
	int		i;

	i = 0;
	init_pipex(ac, &av, &vars, ev);
	while (*(++av + 1))
	{
		if (pipe(vars.fildes) == -1)
			exit(EXIT_FAILURE);
		vars.last_pid = fork();
		if ((vars.last_pid) == -1)
			return (perror("fork"), EXIT_FAILURE);
		if ((vars.last_pid) == 0)
			exec_child(*av, i == 0, *(av + 2) == NULL, &vars);
		dup2(vars.fildes[0], STDIN_FILENO);
		close(vars.fildes[0]);
		close(vars.fildes[1]);
		i++;
	}
	close(STDIN_FILENO);
	while (errno != ECHILD)
	{
		if (wait(&vars.wstatus) == vars.last_pid)
			if (WIFEXITED(vars.wstatus))
				vars.last_exit_code = WEXITSTATUS(vars.wstatus);
	}
	return (vars.last_exit_code);
}
