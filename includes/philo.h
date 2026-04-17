/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:23:42 by arouland          #+#    #+#             */
/*   Updated: 2026/04/17 21:50:48 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <pthread.h>

typedef struct s_lock
{
	pthread_mutex_t fork;
	int	fork_id;
}	t_lock;

typedef struct s_data
{
	int		nb_philos;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	t_lock	*forks; // tableau des forks = mutex
	t_philo	*philos; // tableau de nos philosophes
}	t_data;

typedef struct s_philo
{
	pthread_t tid;
	int		id;
	int		nb_meals;
	int		is_full;
	long	last_meal_time;
	t_lock	*left_fork;
	t_lock	*right_fork;
} t_philo;

#endif