/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 23:58:31 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 16:44:53 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    philo_sleep(t_philo *philo)
{
    print_status(philo->data, philo->id, "is sleeping");
    ft_usleep(philo->data->time_to_sleep, philo->data);
}

void    philo_think(t_philo *philo)
{
    long    think_time;
    
    print_status(philo->data, philo->id, "is thinking");
    if (philo->data->nb_must_meals % 2 != 0)
    {
        think_time = philo->data->time_to_die - (get_time_in_s_ms()
            - get_long(&philo->data->monitor_lock, &philo->last_meal_time)
            - philo->data->time_to_eat);

        if (think_time < 0)
            think_time = 0;
        ft_usleep(think_time / 2, philo->data);
    }
}
// Temps avant mort : 

void    philo_eat(t_philo *philo)
{
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
    pthread_mutex_lock(&philo->data->monitor_lock);
    philo->nb_meals++;
    if (philo->data->nb_must_meals > 0 && philo->nb_meals >= philo->data->nb_must_meals)
        philo->is_full = 1;
    pthread_mutex_unlock(&philo->data->monitor_lock);
    print_status(philo->data, philo->id, "is eating");
    set_long(&philo->data->monitor_lock, &philo->last_meal_time, get_time_in_s_ms());
    ft_usleep(philo->data->time_to_eat, philo->data);
    pthread_mutex_unlock(&philo->right_fork->fork);
    pthread_mutex_unlock(&philo->left_fork->fork);
}
// En mettant direct get_time_in_s_ms et pas current time du début : sauve

void    *philo_routine(void *arg)
{
    t_philo *philo;
    
    philo = (t_philo *)arg;
    wait_all_threads(philo->data);
    if (philo->id % 2 == 0)
        usleep(1500);
    // On ajoute un petit décalaga au début pour éviter blocage au début
    while (is_end_simu(philo->data) == 0)
    {
        philo_eat(philo);
        if (is_end_simu(philo->data) == 1)
            break;
        philo_sleep(philo);
        if (is_end_simu(philo->data) == 1)
            break;
        philo_think(philo);
    }
    return (NULL);
}

void    *lone_philo_routine(void *arg)
{
    t_philo *lone_philo;

    lone_philo = (t_philo *)arg;
    wait_all_threads(lone_philo->data);
    pthread_mutex_lock(&lone_philo->left_fork->fork);
    print_status(lone_philo->data, lone_philo->id, "has taken a fork");
    while (is_end_simu(lone_philo->data) == 0)
        usleep(1000);
    pthread_mutex_unlock(&lone_philo->left_fork->fork);
    return (NULL);
}