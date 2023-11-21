/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: legrandc <legrandc@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/21 11:19:32 by legrandc          #+#    #+#             */
/*   Updated: 2023/11/21 17:43:13 by legrandc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	main(int ac, char **av)
{
	int		fd;
	int		fildes[2];
	char	*cmd;

	fd = open(av[1], O_RDONLY);
	// pipe(fildes);
	if (fork() == 0)
	{
		dup2(fd, 0);
		cmd = ft_split(av[1]);
		execve(, ft_split(), NULL);
		perror("error");
		return (0);
	}
	return (0);
}
