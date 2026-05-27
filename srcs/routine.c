/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 10:41:01 by arouland          #+#    #+#             */
/*   Updated: 2026/05/27 12:53:50 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// Vidéo -> set bool pour commencer toutes les routines en même temps

void    philo_sleep(t_data *data, t_philo *philo)
{
    print_status(data, philo->id, "is sleeping");
    ft_usleep(data->time_to_sleep);
}

void    philo_think(t_data *data, t_philo *philo)
{
    print_status(data, philo->id, "is thinking");
}

void    philo_eat(t_data *data, t_philo *philo)
{
    // Check si y'a les deux fourchettes ?
    
    pthread_mutex_lock(&data->monitor_lock);
    // Monitor lock car check last_meal_time, doit pas checker en même temps que écriture
    philo->last_meal_time = get_current_time_in_ms(data);
    philo->nb_meals++;
    if (data->nb_must_meals > 0 && philo->nb_meals == data->nb_must_meals)
        philo->is_full = 1;
    pthread_mutex_unlock(&data->monitor_lock);
    print_status(data, philo->id, "is eating");
    ft_usleep(data->time_to_eat);
}

void    take_forks(t_data *data, t_philo *philo)
{
    if (philo->id % 2 == 0)
    {
        pthread_mutex_lock(&philo->right_fork->fork);
        print_status(data, philo->id, "has taken a fork");
        pthread_mutex_lock(&philo->left_fork->fork);
        print_status(data, philo->id, "has taken a fork");
    }
    else
    {
        pthread_mutex_lock(&philo->left_fork->fork);
        print_status(data, philo->id, "has taken a fork");
        pthread_mutex_lock(&philo->right_fork->fork);
        print_status(data, philo->id, "has taken a fork");
    }
}

void    drop_forks(t_philo *philo)
{
    pthread_mutex_unlock(&philo->right_fork->fork);
    pthread_mutex_unlock(&philo->left_fork->fork);
}

void    wait_all_philos_ready(t_data *data)
{
    while (1)
    {
        pthread_mutex_lock(&data->monitor_lock);
        if (data->all_philos_ready)
        {
            pthread_mutex_unlock(&data->monitor_lock);
            break ;
        }
        pthread_mutex_unlock(&data->monitor_lock);
        usleep(100); // faire respirer le CPU
    }
}

void    *philo_routine(void *arg)
{
    t_philo *philo;
    t_data  *data;
    
    philo = (t_philo *)arg;
    data = philo->data;
    // printf("Début routine\n");

    // Gestion impair
        // -> ici sûrement attendre un peu si le philo est impair
        // ou gérer l'odre mutex lock left/rigght selon id % 2
    // Du coup bien synchroniser tous les philos pour que les routines commencent en même temps
    // ça on peut le faire avec un booléen dans t_data qui conditionnerait le début de la routine.
    wait_all_philos_ready(data);
    while(check_stop_status(data) == 0)
    {
        take_forks(data, philo);
        philo_eat(data, philo);
        drop_forks(philo);
        
            // --- SLEEP ----
        philo_sleep(data, philo);
        // ---- THINK ----
        philo_think(data, philo);
    }
    return (NULL);
}