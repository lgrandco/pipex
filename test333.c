#include <stdio.h>
#include <stdlib.h>

int	g_z = 2;

int	ft_sqrt(int n)
{
	int			nb;
	long int	i;
	static int	e = 2;
	nb = n;
	i = 1;
	while (i * i <= nb)
	{
		if (i * i == n)
			return (i);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	printf("%d \n", ft_sqrt(atoi(av[1])));
}
