/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 23:44:18 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 16:30:54 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    stop_clean_simulation(t_data *data)
{
    int i;

    i = 0;
    while (i < data->nb_philos)
    {
        pthread_join(data->philos[i].tid, NULL);
        i++;
    }
    pthread_join(data->monitor_thread, NULL);
    i = 0;
    while (i < data->nb_philos)
    {
        pthread_mutex_destroy(&data->forks[i].fork);
        i++;
    }
    // destroy tous les mutex
    pthread_mutex_destroy(&data->write_lock);
    pthread_mutex_destroy(&data->monitor_lock);
    free(data->forks);
    free(data->philos);
    free(data);
}

void    philo_simulation(t_data *data)
{
    int i;

    i = 0;
    if (data->nb_philos == 1)
    {
        pthread_create(&data->philos[0].tid, NULL, lone_philo_routine, &data->philos[0]);
        pthread_create(&data->monitor_thread, NULL, monitor_routine, data);
        set_long(&data->monitor_lock, &data->start_time, get_time_in_s_ms());
        set_long(&data->monitor_lock, &data->philos[0].last_meal_time, data->start_time);
        set_bool(&data->monitor_lock, &data->all_philos_ready, 1);
    }
    else
    {
        while (i < data->nb_philos)
        {
            if (pthread_create(&data->philos[i].tid, NULL, philo_routine, &data->philos[i]) != 0)
            {
                while(--i >= 0)
                    pthread_join(data->philos[i].tid, NULL);
                return ;
            }
            i++;
        }
        pthread_create(&data->monitor_thread, NULL, monitor_routine, data);
        set_long(&data->monitor_lock, &data->start_time, get_time_in_s_ms());
        i = -1;
        while (++i < data->nb_philos)
            set_long(&data->monitor_lock, &data->philos[i].last_meal_time, data->start_time);
        set_bool(&data->monitor_lock, &data->all_philos_ready, 1);
    }
}
