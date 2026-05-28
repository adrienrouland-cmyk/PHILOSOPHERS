/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 22:24:17 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 19:30:06 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	attrib_forks(t_data *data, int i)
{
	data->philos[i].left_fork = &data->forks[i];
	if (i == data->nb_philos - 1)
		data->philos[i].right_fork = &data->forks[0];
	else
		data->philos[i].right_fork = &data->forks[i + 1];
}

void	init_each_philo(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_init(&data->forks[i].fork, NULL);
		data->philos[i].data = data;
		data->philos[i].thread_created = 0;
		data->philos[i].id = i + 1;
		data->philos[i].is_full = 0;
		data->philos[i].nb_meals = 0;
		data->philos[i].last_meal_time = 0;
		attrib_forks(data, i);
		i++;
	}
}

int	init_data(t_data *data)
{
	data->stop_simu = 0;
	data->all_philos_ready = 0;
	data->monitor_created = 0;
	data->philos = malloc(sizeof(t_philo) * data->nb_philos);
	if (!data->philos)
		return (1);
	data->forks = malloc(sizeof(t_lock) * data->nb_philos);
	if (!data->forks)
		return (free(data->philos), 1);
	pthread_mutex_init(&data->write_lock, NULL);
	pthread_mutex_init(&data->monitor_lock, NULL);
	init_each_philo(data);
	return (0);
}
