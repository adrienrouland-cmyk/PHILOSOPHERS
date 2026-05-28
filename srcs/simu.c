/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simu.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 23:44:18 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 10:36:36 by arouland         ###   ########.fr       */
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
    // 1. Créer tous les threads
    // 2. Créer le thread monitor
    // 3. Initialiser start_time
        //3.2. boucler sur i < nb_philo pour attribuer le last_meal_time = start_time
    // 4. Ouvrir la barrière

    int i;

    i = 0;
    if (data->nb_philos == 1)
    {
        set_long(&data->monitor_lock, &data->start_time, get_time_in_s_ms());
        data->philos[0].last_meal_time = data->start_time;
        pthread_create(&data->philos[0].tid, NULL, lone_philo_routine, &data->philos[0]);
        return ;
    }
    // Dans le cas du seul philosopher -> on crée une pthread avec une routine où il va juste print take a fork puis attendre de mourir
    else
    {
        while (i < data->nb_philos)
        {
            pthread_create(&data->philos[i].tid, NULL, philo_routine, &data->philos[i]);
            i++;
        }
        // Le while -> je crée tous mes threads et je leur attribue philo_routine.
        pthread_create(&data->monitor_thread, NULL, monitor_routine, data);
        // Je crée mon monitor thread -> tous mes threads sont initialisés mtn
        set_long(&data->monitor_lock, &data->start_time, get_time_in_s_ms());
        i = 0;
        while (i < data->nb_philos)
        {
            set_long(&data->monitor_lock, &data->philos[i].last_meal_time, data->start_time);
            i++;
        }
        // J'attribue mtn le last meal time au start time pour tous mes threads.
        set_bool(&data->monitor_lock, &data->all_philos_ready, 1);
        // Tous mes threads sont créés -> on va pouvoir commencer
    }
}
