/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 23:12:51 by leo               #+#    #+#             */
/*   Updated: 2023/12/08 23:02:38 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "libft/libft.h"

# define HERE_DOC_ERR \
	"warning : here-document delimited by end-of-file \
(wanted `EOF')\n"
# define ARGS_ERROR \
	"Usage: ./pipex[_bonus] (INFILE | here_doc LIMITER) cmd1 ... cmdn \
OUTFILE\n"

typedef struct s_vars
{
	char	*outfile_name;
	int		outfile_fd;
	char	*infile_name;
	int		infile_fd;
	char	*path;
	int		is_heredoc;
	char	*delimiter;
	size_t	commands_nb;
	int		fildes[2];
	int		wstatus;
	int		last_exit_code;
	char	**env;
	int		here_docfd[2];
	int		last_pid;
	int		is_first;
	char	**paths;
	int		child_exit_code;
	int		B;

}			t_vars;

/* FUNCTIONS */
void		exit_error(char *s, t_vars *vars, int flag, int flag2);
int			get_path(char *command, t_vars *vars);
void		write_heredoc(char *eof, int fd);
void		get_paths(char **ev, t_vars *vars);
void		init_pipex(int ac, char ***av, t_vars *vars, char **ev);

#endif // !PIPEX_H
