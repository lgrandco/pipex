/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:19:32 by legrandc          #+#    #+#             */
/*   Updated: 2023/12/08 22:56:42 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	get_infile(t_vars *vars)
{
	if (vars->is_heredoc)
		vars->infile_fd = vars->here_docfd[0];
	else
		vars->infile_fd = open(vars->infile_name, O_RDONLY);
	if (vars->infile_fd == -1)
		exit_error(vars->infile_name, vars, 1, 0);
	dup2(vars->infile_fd, STDIN_FILENO);
	close(vars->infile_fd);
}

void	get_fds(t_vars *vars, int is_last)
{
	if (vars->is_first)
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
			exit_error(vars->outfile_name, vars, 1, 0);
		dup2(vars->outfile_fd, STDOUT_FILENO);
		close(vars->outfile_fd);
	}
	close(vars->fildes[0]);
	close(vars->fildes[1]);
}

void	exec_child(char *command, int is_last, t_vars *vars)
{
	char	**args;

	get_fds(vars, is_last);
	args = ft_split(command, ' ');
	if (!args || get_path(args[0], vars) == -1)
		exit_error("malloc", vars, 0, 0);
	if (vars->path == NULL)
	{
		if (args[0])
			ft_putstr_fd(args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	else if (access(vars->path, F_OK) == -1 && errno == ENOENT)
		perror(vars->path);
	else
	{
		execve(vars->path, args, vars->env);
		perror(vars->path);
		vars->child_exit_code = 126;
	}
	free(vars->path);
	free_matrix(args);
	free_matrix(vars->paths);
	exit(vars->child_exit_code);
}

int	wait_commands(t_vars *vars)
{
	int	ret;

	while (errno != ECHILD)
	{
		if (wait(&vars->wstatus) == vars->last_pid)
		{
			if (WIFEXITED(vars->wstatus))
				ret = WEXITSTATUS(vars->wstatus);
			else
				ret = 128 + WTERMSIG(vars->wstatus);
		}
	}
	if (vars->is_heredoc)
		close(vars->here_docfd[0]);
	free_matrix(vars->paths);
	return (ret);
}

int	main(int ac, char **av, char **ev)
{
	t_vars	vars;

	init_pipex(ac, &av, &vars, ev);
	while (*(++av + 1))
	{
		if (pipe(vars.fildes) == -1)
			exit_error("pipe", &vars, 0, 1);
		vars.last_pid = fork();
		if ((vars.last_pid) == -1)
			exit_error("fork", &vars, 1, 1);
		if ((vars.last_pid) == 0)
			exec_child(*av, *(av + 2) == NULL, &vars);
		dup2(vars.fildes[0], STDIN_FILENO);
		close(vars.fildes[0]);
		close(vars.fildes[1]);
		vars.is_first = 0;
	}
	return (wait_commands(&vars));
}
