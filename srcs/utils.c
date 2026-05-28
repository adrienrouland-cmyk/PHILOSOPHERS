/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:04:53 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 19:35:13 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	set_early_stop(t_data *data)
{
	set_bool(&data->monitor_lock, &data->stop_simu, 1);
	set_bool(&data->monitor_lock, &data->all_philos_ready, 1);
}

long	get_time_in_s_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long	get_current_time_in_ms(t_data *data)
{
	return (get_time_in_s_ms() - data->start_time);
}

int	ft_usleep(long milliseconds, t_data *data)
{
	long	start_time;

	start_time = get_time_in_s_ms();
	while (get_time_in_s_ms() - start_time < milliseconds)
	{
		if (is_end_simu(data))
			break ;
		usleep(500);
	}
	return (0);
}
