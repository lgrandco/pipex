/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: leo <leo@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 23:12:51 by leo               #+#    #+#             */
/*   Updated: 2023/11/26 02:04:19 by leo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include "libft/libft.h"
# define ARGS_ERROR \
	"Usage: ./pipex (INFILE\
 || here_doc LIMITER) cmd1 ... cmdn OUTFILE\n"
# define HEREDOC_LOCATION "/tmp/heredoc"

typedef struct s_vars
{
	char	*outfile_name;
	int		outfile_fd;
	char	*infile_name;
	int		infile_fd;
	char	*path;
	int		is_heredoc;
	char	*delimiter;
}			t_vars;

#endif // !PIPEX_H
