/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 23:44:18 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 19:35:30 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	*lone_philo_routine(void *arg)
{
	t_philo	*lone_philo;

	lone_philo = (t_philo *)arg;
	wait_all_threads(lone_philo->data);
	pthread_mutex_lock(&lone_philo->left_fork->fork);
	print_status(lone_philo->data, lone_philo->id, "has taken a fork");
	while (is_end_simu(lone_philo->data) == 0)
		usleep(1000);
	pthread_mutex_unlock(&lone_philo->left_fork->fork);
	return (NULL);
}

void	stop_clean_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philos)
	{
		if (data->philos[i].thread_created == 1)
			pthread_join(data->philos[i].tid, NULL);
		i++;
	}
	if (data->monitor_created == 1)
		pthread_join(data->monitor_thread, NULL);
	i = 0;
	while (i < data->nb_philos)
	{
		pthread_mutex_destroy(&data->forks[i].fork);
		i++;
	}
	pthread_mutex_destroy(&data->write_lock);
	pthread_mutex_destroy(&data->monitor_lock);
	free(data->forks);
	free(data->philos);
	free(data);
}

void	lone_philo_simulation(t_data *data)
{
	if (pthread_create(&data->philos[0].tid, NULL, lone_philo_routine,
			&data->philos[0]) != 0)
	{
		set_bool(&data->monitor_lock, &data->stop_simu, 1);
		set_bool(&data->monitor_lock, &data->all_philos_ready, 1);
		return ;
	}
	data->philos[0].thread_created = 1;
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
	{
		set_bool(&data->monitor_lock, &data->stop_simu, 1);
		set_bool(&data->monitor_lock, &data->all_philos_ready, 1);
		return ;
	}
	data->monitor_created = 1;
	set_long(&data->monitor_lock, &data->start_time, get_time_in_s_ms());
	set_long(&data->monitor_lock, &data->philos[0].last_meal_time,
		data->start_time);
	set_bool(&data->monitor_lock, &data->all_philos_ready, 1);
}

void	multi_philo_simulation(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->nb_philos)
	{
		if (pthread_create(&data->philos[i].tid, NULL, philo_routine,
				&data->philos[i]) != 0)
		{
			set_early_stop(data);
			return ;
		}
		data->philos[i].thread_created = 1;
	}
	if (pthread_create(&data->monitor_thread, NULL, monitor_routine, data) != 0)
	{
		set_early_stop(data);
		return ;
	}
	data->monitor_created = 1;
	set_long(&data->monitor_lock, &data->start_time, get_time_in_s_ms());
	i = -1;
	while (++i < data->nb_philos)
		set_long(&data->monitor_lock, &data->philos[i].last_meal_time,
			data->start_time);
	set_bool(&data->monitor_lock, &data->all_philos_ready, 1);
}

void	philo_simulation(t_data *data)
{
	if (data->nb_philos == 1)
		lone_philo_simulation(data);
	else
		multi_philo_simulation(data);
}
