/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:19:32 by legrandc          #+#    #+#             */
/*   Updated: 2023/11/26 02:57:52 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*get_path(char *program, char **env)
{
	char	*path_tried;
	char	**paths;

	while (*env)
	{
		paths = ft_split(*env, '=');
		if (!strcmp("PATH", paths[0]))
			break ;
		else
			env++;
	}
	paths = ft_split(paths[1], ':');
	while (*paths)
	{
		path_tried = ft_strjoin(*paths, "/");
		path_tried = ft_strjoin(path_tried, program);
		if (access(path_tried, X_OK) == 0)
			return (path_tried);
		free(path_tried);
		paths++;
	}
	ft_putstr_fd(program, STDERR_FILENO);
	ft_putendl_fd(": command not found\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
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

void	exec_pipe(char *command, char **ev, int is_last, t_vars *vars)
{
	int		fildes[2];
	char	**argv;
	char	*path;

	pipe(fildes);
	if (fork() == 0)
	{
		if (!is_last)
			dup2(fildes[1], STDOUT_FILENO);
		if (is_last)
		{
			if (vars->is_heredoc)
				vars->outfile_fd = open(vars->outfile_name,
						O_WRONLY | O_APPEND | O_CREAT, 0666);
			else
				vars->outfile_fd = open(vars->outfile_name,
						O_WRONLY | O_TRUNC | O_CREAT, 0666);
			dup2(vars->outfile_fd, STDOUT_FILENO);
		}
		argv = ft_split(command, ' ');
		if (!argv)
		{
			perror("malloc");
			EXIT_FAILURE;
		}
		if (close(fildes[0]) == -1 || close(fildes[1]) == -1)
			perror("close");
		path = get_path(argv[0], ev);
		execve(path, argv, ev);
		perror(argv[0]);
		free_matrix(argv);
		exit(EXIT_FAILURE);
	}
	dup2(fildes[0], STDIN_FILENO);
	if (close(fildes[0]) == -1 || close(fildes[1]) == -1)
		perror("close");
}

void	get_infile(t_vars *vars)
{
	if (vars->is_heredoc)
	{
		vars->infile_fd = open(vars->infile_name, O_TRUNC | O_RDWR | O_CREAT,
				0600);
		if (vars->infile_fd == -1)
		{
			perror("open");
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
	(*av)++;
}

int	main(int ac, char **av, char **ev)
{
	t_vars	vars;

	init_pipex(ac, &av, &vars);
	get_infile(&vars);
	dup2(vars.infile_fd, STDIN_FILENO);
	close(vars.infile_fd);
	while (*(++av + 1))
		exec_pipe(*av, ev, *(av + 2) == NULL, &vars);
	while (ac-- != 3)
		wait(NULL);
	return (0);
}
