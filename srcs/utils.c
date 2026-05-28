/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:04:53 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 16:07:03 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    print_status(t_data *data, int philo_id, char *msg)
{
    long current_time;
    int is_end;

    is_end = is_end_simu(data);
    // dans variable avant pour éviter deadlock
    pthread_mutex_lock(&data->write_lock);
    if (is_end == 0)
    {
        current_time = get_current_time_in_ms(data);
        printf("%ld %d %s\n", current_time, philo_id, msg);
    }
    pthread_mutex_unlock(&data->write_lock);
}
// write lock car printf n'est pas thread safe

void    wait_all_threads(t_data *data)
{
    while (get_bool(&data->monitor_lock, &data->all_philos_ready) == 0)
        usleep(100);
}

int     is_end_simu(t_data *data)
{
    return (get_bool(&data->monitor_lock, &data->stop_simu));
}

long    get_time_in_s_ms(void)
{
    struct timeval  time;

    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}

long    get_current_time_in_ms(t_data *data)
{
    return get_time_in_s_ms() - data->start_time;
}

int     ft_usleep(long milliseconds, t_data *data)
{
    long    start_time;

    start_time = get_time_in_s_ms();
    while (get_time_in_s_ms() - start_time < milliseconds)
    {
        if (is_end_simu(data))
            break;
        usleep(500);
    }
    return (0);
}
