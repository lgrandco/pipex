char	*next_line(char *save)
{
	size_t i;
	char *line;
	char *tmp;

	if (!save)
		return (NULL);
	tmp = ft_strchr(save, '\n');
	if (!tmp)
	{
		free(save);
		return (NULL);
	}
	i = 0;
	while (save[i] && save[i] != '\n')
		i++;
	line = malloc(ft_strlen(save) - i);
	if (!line)
		return (NULL);
	tmp++;
	i = 0;
	while (*tmp)
		line[i++] = *tmp++;
	line[i] = 0;
	free(save);
	return (line);
}
