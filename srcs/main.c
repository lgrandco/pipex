/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:19:32 by legrandc          #+#    #+#             */
/*   Updated: 2023/11/22 09:23:33 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	main(int ac, char **av, char **ev)
{
	int		fd;
	int		fildes[2];
	char	**cmd;
	int		fd_outfile;

	fd = open(av[1], O_RDONLY);
	pipe(fildes);
	fd_outfile = open(av[4], O_WRONLY | O_CREAT, 0644);
	if (fd == -1 || fd_outfile == -1)
		;
	if (fork() == 0)
	{
		close(fildes[0]);
		dup2(fd, STDIN_FILENO);
		close(fd);
		cmd = ft_split(av[2], ' ');
		dup2(fildes[1], STDOUT_FILENO);
		close(fildes[1]);
		execve(ft_strjoin("/bin/", cmd[0]), cmd, ev);
		perror(cmd[0]);
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
	wait(NULL);
	close(fildes[1]);
	if (fork() == 0)
	{
		cmd = ft_split(av[3], ' ');
		dup2(fildes[0], STDIN_FILENO);
		close(fildes[0]);
		dup2(fd_outfile, STDOUT_FILENO);
		close(fd_outfile);
		execve(ft_strjoin("/bin/", cmd[0]), cmd, ev);
		perror(cmd[0]);
		fflush(stderr);
		exit(EXIT_FAILURE);
	}
	close(fd_outfile);
	close(fd);
	close(fildes[0]);
	fflush(stderr);
	wait(NULL);
	return (0);
}
