/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 22:24:22 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 18:39:50 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	ft_atol_safe(char *str, int *is_overflow)
{
	long long	nb;
	int			i;
	int			negative;

	i = 0;
	nb = 0;
	negative = 1;
	while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
		i++;
	if (str[i] == '+' && str[i + 1] != '-')
		i++;
	if (str[i] == '-')
	{
		negative = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (nb > (INT_MAX - (str[i] - '0')) / 10)
			return (*is_overflow = 1, -1);
		nb = nb * 10 + (str[i] - 48);
		i++;
	}
	return (nb * negative);
}

int	check_is_valid_digit(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while ((argv[i][j] >= 9 && argv[i][j] <= 13) || argv[i][j] == 32)
			j++;
		if (argv[i][j] == '+' || argv[i][j] == '-')
			j++;
		if (argv[i][j] == '\0')
			return (1);
		while (argv[i][j] >= '0' && argv[i][j] <= '9')
			j++;
		while ((argv[i][j] >= 9 && argv[i][j] <= 13) || argv[i][j] == 32)
			j++;
		if (argv[i][j] != '\0')
			return (1);
		i++;
	}
	return (0);
}

int	check_data(t_data *data, char **argv, int is_overflow)
{
	if (is_overflow == 1)
	{
		write(2, "Error: argument values exceed INT_MAX\n", 38);
		return (1);
	}
	if (data->nb_philos <= 0 || data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->time_to_sleep <= 0)
		return (write(2, "Arguments errors : negative values\n", 35), 1);
	if (argv[5] && data->nb_must_meals <= 0)
		return (write(2, "Error: minimal number of meals\n", 32), 1);
	return (0);
}

int	parse_data(t_data *data, char **argv)
{
	int	is_overflow;

	if (check_is_valid_digit(argv) == 1)
		return (write(2, "Non valid arguments\n", 20), 1);
	is_overflow = 0;
	data->nb_philos = (int)ft_atol_safe(argv[1], &is_overflow);
	data->time_to_die = ft_atol_safe(argv[2], &is_overflow);
	data->time_to_eat = ft_atol_safe(argv[3], &is_overflow);
	data->time_to_sleep = ft_atol_safe(argv[4], &is_overflow);
	if (argv[5])
		data->nb_must_meals = (int)ft_atol_safe(argv[5], &is_overflow);
	else
		data->nb_must_meals = -1;
	if (check_data(data, argv, is_overflow) == 1)
		return (1);
	return (0);
}
