/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 23:12:51 by leo               #+#    #+#             */
/*   Updated: 2023/11/28 01:01:47 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# define ARGS_ERROR \
	"Usage: ./pipex (INFILE\
 || here_doc LIMITER) cmd1 ... cmdn OUTFILE\n"
# define HERE_DOC_ERR "warning : here-document delimited by end-of-file (wanted `EOF')\n"

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

}			t_vars;

#endif // !PIPEX_H
