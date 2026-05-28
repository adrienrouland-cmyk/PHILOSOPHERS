/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 18:27:30 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 20:06:10 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	print_status(t_data *data, int philo_id, char *msg)
{
	pthread_mutex_lock(&data->write_lock);
	pthread_mutex_lock(&data->monitor_lock);
	if (!data->stop_simu)
	{
		printf("%ld %d %s\n", get_current_time_in_ms(data), philo_id, msg);
	}
	pthread_mutex_unlock(&data->monitor_lock);
	pthread_mutex_unlock(&data->write_lock);
}

void	wait_all_threads(t_data *data)
{
	while (get_bool(&data->monitor_lock, &data->all_philos_ready) == 0)
		usleep(100);
}

int	is_end_simu(t_data *data)
{
	return (get_bool(&data->monitor_lock, &data->stop_simu));
}
