/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: legrandc <legrandc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/24 23:12:51 by leo               #+#    #+#             */
/*   Updated: 2023/11/26 17:00:51 by legrandc         ###   ########.fr       */
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
	size_t	commands_nb;
	int		fildes[2];
	t_list	*pids;
	t_list	*new;
	int		last_exit_code;
}			t_vars;

#endif // !PIPEX_H
