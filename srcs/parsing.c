/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 12:48:17 by arouland          #+#    #+#             */
/*   Updated: 2026/04/21 10:11:12 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int  is_space(char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
        return (1);
    return (0);
}

long    ft_atol(char *str)
{
    long    nb;
    int     i;
    int negative;

    i = 0;
    nb = 0;
    negative = 1;
    while (is_space(str[i]) == 1)
        i++;
    if (str[i] == '\0')
        return (0);
    if (str[i] == '+' && str[i + 1] != '-')
        i++;
    if (str[i] == '-')
    {
        negative = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        nb = nb * 10 + (str[i] - 48);
        i++;
    }
    nb = nb * negative;
    return (nb);
}
// Mettre en place la protection de l'overflow ?

int check_is_digit(char **argv)
{
    int i;
    int j;

    i = 1;
    while (argv[i])
    {
        j = 0;
        while (argv[i][j])
        {
            if (argv[i][j] < '0' || argv[i][j] > '9')
                return (1);
            j++;
        }
        i++;
    }
    return (0);
}

int check_data(t_data *data)
{
    // LONG_MAX
    if (data->nb_philos > INT_MAX || data->time_to_die > LONG_MAX
        || data->time_to_eat > LONG_MAX || data->time_to_sleep > LONG_MAX)
        return (write(1, "Arguments errors : over LONG_MAX", 17), 1);
    if (data->nb_philos <= 0 || data->time_to_die <= 0
        || data->time_to_eat <= 0 || data->time_to_sleep <= 0)
        return (write(1, "Arguments errors : negative values", 17), 1);

    // Others ?
    return (0);
}

int parse_data(t_data *data, char **argv)
{   
    if (check_is_digit(argv) == 1)
        return (1);
    data->nb_philos = (int)ft_atol(argv[1]);
    data->time_to_die = ft_atol(argv[2]);
    data->time_to_eat = ft_atol(argv[3]);
    data->time_to_sleep = ft_atol(argv[4]);
    if (argv[5])
        data->nb_must_meals = (int)ft_atol(argv[5]);
    return (0);
}
// Parse les arguments et les attribue
// Check par la suite dans check_data