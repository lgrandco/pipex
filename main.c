/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: root <root@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 23:12:51 by root              #+#    #+#             */
/*   Updated: 2023/09/12 20:59:56 by root             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}
char	**ft_split(char *str, char c)
{
	char	**ret;
	size_t	word_number;
	size_t	i;

	word_number = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] != c && (!i || str[i - 1] == c))
			word_number++;
		i++;
	}
	ret = malloc((word_number + 1) * sizeof(*ret));
	if (!ret)
		return (0);
	word_number = 0;
	while (*str)
	{
		if (*str != c)
		{
			i = 0;
			while (str[i] && str[i] != c)
				i++;
			ret[word_number] = malloc(i + 1);
			i = 0;
			while (*str && *str != c)
				ret[word_number][i++] = *str++;
			ret[word_number++][i] = 0;
		}
		else
			str++;
	}
	ret[word_number] = 0;
	return (ret);
}

int	main(int argc, char *argv[])
{
	char	**d;
	int		i;
	char	*path;
	char	**args;
	int		pid;
	int		fildes[2];

	// if (argc != 5)
	// return (-1);
	pipe(fildes);
	if (fork() == 0)
	{
		path = "/bin/cat";
		args = malloc(sizeof(*args) * 4);
		args[0] = "cat";
		args[1] = argv[1];
		args[2] = NULL;
		args[3] = 0;
		close(fildes[0]);
		dup2(fildes[1], STDOUT_FILENO);
		close(fildes[1]);
		execve(path, args, NULL);
		perror(args[0]);
		exit(EXIT_FAILURE);
	}
	else
	{
		close(fildes[1]);
		if (fork() == 0)
		{
			args = ft_split(argv[2], ' ');
			path = malloc(ft_strlen(args[0]) + ft_strlen("/bin/") + 1);
			if (!path)
				return (-1);
			strcpy(path, "/bin/");
			strcat(path, args[0]);
			close(fildes[1]);
			dup2(fildes[0], STDIN_FILENO);
			close(fildes[0]);
			execve(path, args, NULL);
			perror(args[0]);
			exit(EXIT_FAILURE);
		}
		else
		{
			close(fildes[0]);
			wait(NULL);
			wait(NULL);
		}
	}
}
