/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 09:45:45 by arouland          #+#    #+#             */
/*   Updated: 2026/04/21 10:09:43 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    init_data(t_data *data)
{
    int i;

    i = 0;
    data->stop_simu = 0;
    data->start_time = get_time_in_s_ms();
    data->philos = malloc(sizeof(t_philo) * data->nb_philos);
    if (!data->philos)
        return ;
    data->forks = malloc(sizeof(t_lock) * data->nb_philos);
    if (!data->forks)
        return ;
    while (i < data->nb_philos)
    {
        pthread_mutex_init(&data->forks[i].fork, NULL); // init les n forks
        data->philos[i].data = data;
        data->philos[i].id = i + 1;
        data->philos[i].nb_meals = 0;
        data->philos[i].is_full = 0;
        data->philos[i].last_meal_time = 0;

        // attribuer les forks aussi.
        data->philos[i].left_fork = &data->forks[i];
            // Pointeur pour copier l'adresse et éviter la copie
                // car ces ressources sont partagées
        if (i == data->nb_philos - 1)
            data->philos[i].right_fork = &data->forks[0];
        else
            data->philos[i].right_fork = &data->forks[i + 1];
        i++;
    }
}
// Création de tous mes philos, et init de leurs mutex.
// Attribution des mutex.
// On créera les phtread après dans start_simu.
// Si 4 philos : philo 0 prend fourchettes 0 et 1
// philo 1 prend fourchettes 1 et 2
// 2 -> 2 et 3 ; 3 -> 3 et 4; 4 -> 4 et 0.
