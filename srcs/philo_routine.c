/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 23:58:31 by arouland          #+#    #+#             */
/*   Updated: 2026/05/28 19:36:44 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void	philo_sleep(t_philo *philo)
{
	print_status(philo->data, philo->id, "is sleeping");
	ft_usleep(philo->data->time_to_sleep, philo->data);
}

void	philo_think(t_philo *philo)
{
	long	think_time;

	print_status(philo->data, philo->id, "is thinking");
	if (philo->data->nb_philos % 2 != 0)
	{
		think_time = philo->data->time_to_die - (get_time_in_s_ms()
				- get_long(&philo->data->monitor_lock, &philo->last_meal_time))
			- philo->data->time_to_eat;
		if (think_time < 0)
			think_time = 0;
		ft_usleep(think_time / 2, philo->data);
	}
}

void	take_forks(t_philo *philo)
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
}

void	philo_eat(t_philo *philo)
{
	set_long(&philo->data->monitor_lock, &philo->last_meal_time,
		get_time_in_s_ms());
	print_status(philo->data, philo->id, "is eating");
	pthread_mutex_lock(&philo->data->monitor_lock);
	philo->nb_meals++;
	if (philo->data->nb_must_meals > 0
		&& philo->nb_meals >= philo->data->nb_must_meals)
		philo->is_full = 1;
	pthread_mutex_unlock(&philo->data->monitor_lock);
	ft_usleep(philo->data->time_to_eat, philo->data);
	pthread_mutex_unlock(&philo->right_fork->fork);
	pthread_mutex_unlock(&philo->left_fork->fork);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_all_threads(philo->data);
	if (philo->id % 2 == 0)
		usleep(1500);
	while (is_end_simu(philo->data) == 0)
	{
		take_forks(philo);
		philo_eat(philo);
		if (is_end_simu(philo->data) == 1)
			break ;
		philo_sleep(philo);
		if (is_end_simu(philo->data) == 1)
			break ;
		philo_think(philo);
	}
	return (NULL);
}
