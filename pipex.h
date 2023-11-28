/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 23:12:51 by leo               #+#    #+#             */
/*   Updated: 2023/11/28 15:56:19 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# ifndef BONUS
#  define BONUS 1
# endif

# include "libft/libft.h"

# define HERE_DOC_ERR \
	"warning : here-document delimited by end-of-file \
(wanted `EOF')\n"
# define ARGS_ERROR \
	"Usage: ./pipex (INFILE | here_doc LIMITER) cmd1 ... cmdn \
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

}			t_vars;

/* FUNCTIONS */
void		exit_error(char *s, t_vars *vars, int flag);
int			get_path(char *command, t_vars *vars);
void		write_heredoc(char *eof, int fd);
void		get_paths(char **ev, t_vars *vars);
void		init_pipex(int ac, char ***av, t_vars *vars, char **ev);

#endif // !PIPEX_H
