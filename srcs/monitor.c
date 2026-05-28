/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 00:01:40 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 17:43:53 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int     is_philo_dead(t_data *data)
{
    int i;
    long time_since_last_meal;
    long current_time;
    long last_meal_time;

    i = 0;
    while (i < data->nb_philos)
    {
        current_time = get_time_in_s_ms();
        last_meal_time = get_long(&data->monitor_lock, &data->philos[i].last_meal_time);
        time_since_last_meal = current_time - last_meal_time;
        if (time_since_last_meal > data->time_to_die)
        {
            set_bool(&data->monitor_lock, &data->stop_simu, 1);
            pthread_mutex_lock(&data->write_lock);
            printf("%ld %d has died\n", current_time - data->start_time, data->philos[i].id);
            pthread_mutex_unlock(&data->write_lock);
            return (1);
        }
        i++;
    }
    return (0);
}

int     is_all_philos_full(t_data *data)
{
    int i;
    int full_philos;
    
    i = 0;
    full_philos = 0;
    if (data->nb_must_meals == -1)
        return (0);
    while (i < data->nb_philos)
    {
        if (get_bool(&data->monitor_lock, &data->philos[i].is_full) == 1)
            full_philos++;
        i++;
    }
    if (full_philos == data->nb_philos)
    {
        set_bool(&data->monitor_lock, &data->stop_simu, 1);
        return (1);
    }
    return (0);
}

void    *monitor_routine(void *arg)
{
    t_data *data;
    
    data = (t_data *)arg;
    wait_all_threads(data);
    
    while (is_end_simu(data) == 0)
    {
        if (is_philo_dead(data) == 1)
            return (NULL);
        if (is_all_philos_full(data) == 1)
            return (NULL);
        usleep(1000);
        //usleep pour éviter de check millions par seconde, mais 1ms
    }
    return (NULL);
    // Une fois que tout est lancé -> fin wait_all_threads
    // Le monitor doit checker la fin de la simu
    // Soit par mort -> one died -> fin
    // Soit par assez de meals sont mangés
}