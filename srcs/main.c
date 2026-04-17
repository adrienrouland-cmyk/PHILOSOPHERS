/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:23:30 by arouland          #+#    #+#             */
/*   Updated: 2026/04/17 21:52:59 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long    ft_atol(char *str)
{
    
}

int parse_data(t_data *data, char *argv)
{
    
}

void    *philo_routine(void *data)
{
    
}

int	main(int argc, char *argv[])
{
	t_data *data;
    
    if (argc != 5 || argc != 6)
        return (write(1, "Wrongs arguments", 17), 1);
    data = malloc(sizeof(t_data));
    if (!data)
        return (1);
    parse_data(data, argv);
}
//./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
// ./philo 5 800 200 200