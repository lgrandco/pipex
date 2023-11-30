/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/28 13:08:04 by leo               #+#    #+#             */
/*   Updated: 2023/11/30 03:44:38 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	exit_error(char *s, t_vars *vars, int close_fds)
{
	perror(s);
	free_matrix(vars->paths);
	if (close_fds)
	{
		close(vars->fildes[0]);
		close(vars->fildes[1]);
	}
	exit(EXIT_FAILURE);
}

int	get_path(char *command, t_vars *vars)
{
	char	*tested_path;
	size_t	i;

	if (!command || !ft_strcmp(command, "."))
		return (0);
	if (ft_strchr(command, '/'))
		return (vars->path = ft_strdup(command), (vars->path != NULL) - 1);
	i = 0;
	while (vars->paths && vars->paths[i])
	{
		tested_path = ft_strjoin3(vars->paths[i], "/", command);
		if (!tested_path)
			return (-1);
		if (access(tested_path, F_OK) == 0)
			return (vars->path = tested_path, 0);
		free(tested_path);
		i++;
	}
	return (0);
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

void	get_paths(char **ev, t_vars *vars)
{
	char	**var;

	vars->path = NULL;
	vars->paths = NULL;
	vars->env = ev;
	vars->is_first = 1;
	while (ev && *ev)
	{
		var = ft_split(*ev, '=');
		if (var[0] && var[1] && !ft_strcmp("PATH", var[0]))
		{
			vars->paths = ft_split(var[1], ':');
			free_matrix(var);
			break ;
		}
		ev++;
		free_matrix(var);
	}
}

void	init_pipex(int ac, char ***av, t_vars *vars, char **ev)
{
	vars->is_heredoc = BONUS && (*av)[1] && !ft_strcmp((*av)[1], "here_doc");
	if ((BONUS == 0 && ac != 5) || (BONUS && ac + vars->is_heredoc < 6))
	{
		ft_putstr_fd(ARGS_ERROR, STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	vars->outfile_name = (*av)[ac - 1];
	if (vars->is_heredoc)
	{
		vars->is_heredoc = 1;
		vars->delimiter = (*av)[2];
		(*av)++;
		if (pipe(vars->here_docfd) == -1)
			exit_error("pipe", vars, 0);
		write_heredoc(vars->delimiter, vars->here_docfd[1]);
		close(vars->here_docfd[1]);
	}
	else
	{
		vars->infile_name = (*av)[1];
		vars->is_heredoc = 0;
	}
	(*av)++;
	get_paths(ev, vars);
}
