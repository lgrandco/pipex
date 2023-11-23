/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: legrandc <legrandc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:19:32 by legrandc          #+#    #+#             */
/*   Updated: 2023/11/23 18:12:08 by legrandc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

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
	return (0);
}

void	write_heredoc(char *end, int fd)
{
	char	*line;

	while (1)
	{
		ft_printf("> ");
		line = get_next_line(0);
		if (!ft_strncmp(line, end, ft_strlen(line) - 1))
			break ;
		ft_putstr_fd(line, fd);
		free(line);
	}
	free(line);
}

void	exec_pipe(char *command, char **ev, int is_last, int fd_outfile)
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
			dup2(fd_outfile, STDOUT_FILENO);
		argv = ft_split(command, ' ');
		if (!argv || close(fildes[0]) == -1 || close(fildes[1]) == -1
			|| close(fd_outfile) == -1)
			exit(EXIT_FAILURE);
		path = get_path(argv[0], ev);
		execve(path, argv, ev);
		perror(argv[0]);
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
	if (dup2(fildes[0], STDIN_FILENO) == -1 || close(fildes[0]) == -1
		|| close(fildes[1]) == -1)
		exit(EXIT_FAILURE);
}

int	main(int ac, char **av, char **ev)
{
	int	fd_infile;
	int	fd_outfile;

	if (ac < 5)
		exit(EXIT_FAILURE);
	if (!strcmp(av[1], "here_doc"))
	{
		fd_infile = open("/tmp/here_doc", O_TRUNC | O_RDWR | O_CREAT, 0644);
		write_heredoc(av[2], fd_infile);
		fd_infile = open("/tmp/here_doc", O_WRONLY, 0644);
		fd_outfile = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
		av++;
	}
	else
	{
		fd_infile = open(av[1], O_RDONLY | O_CREAT, 066);
		fd_outfile = open(av[ac - 1], O_WRONLY | O_TRUNC | O_CREAT, 0644);
	}
	av++;
	if (fd_infile == -1 || fd_outfile == -1)
		return (1);
	dup2(fd_infile, STDIN_FILENO);
	close(fd_infile);
	while (*(++av + 1))
	{
		dprintf(2, "[%s]\n\n\n", *av);
		exec_pipe(*av, ev, *(av + 2) == NULL, fd_outfile);
	}
	close(fd_outfile);
	while (ac-- != 3)
	{
		wait(NULL);
	}
	return (0);
}
