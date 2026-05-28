/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 23:58:31 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 12:00:22 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    philo_eat(t_philo *philo)
{
    long current_time;

    current_time = get_current_time_in_ms(philo->data);
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->right_fork->fork);
        print_status(philo->data, philo->id, "has taken a fork");
        pthread_mutex_lock(&philo->left_fork->fork);
        print_status(philo->data, philo->id, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&philo->left_fork->fork);
        print_status(philo->data, philo->id, "has taken a fork");
        pthread_mutex_lock(&philo->right_fork->fork);
        print_status(philo->data, philo->id, "has taken a fork");
    }
    set_long(&philo->data->monitor_lock, &philo->last_meal_time, current_time);
    print_status(philo->data, philo->id, "is eating");
    ft_usleep(philo->data->time_to_eat);
    pthread_mutex_lock(&philo->data->monitor_lock);
    philo->nb_meals++;
    if (philo->nb_meals >= philo->data->nb_must_meals)
        philo->is_full = 1;
    pthread_mutex_lock(&philo->data->monitor_lock);
    pthread_mutex_unlock(&philo->right_fork->fork);
    pthread_mutex_unlock(&philo->left_fork->fork);
}

void    *philo_routine(void *arg)
{
    t_philo *philo;
    
    philo = (t_philo *)arg;
    wait_all_threads(philo->data);
    while (is_end_simu(philo->data) == 0)
    {
        //eat
        if (is_end_simu(philo->data) == 1)
        // sleep
        // Check simulation finie ?
        // think
    }
    return (NULL);
}

void    *lone_philo_routine(void *arg)
{
    t_philo *lone_philo;

    lone_philo = (t_philo *)arg;
    // print_status(philo->data, philo->id, "has taken a fork");
    
    // Suite à faire
}