/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:23:42 by arouland          #+#    #+#             */
/*   Updated: 2026/04/21 10:23:17 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdio.h>
#include <pthread.h>
#include <unistd.h> // usleep
#include <stdlib.h>
#include <limits.h>
#include <sys/time.h>

typedef struct s_data	t_data;

typedef struct s_lock
{
	pthread_mutex_t fork;
	int	fork_id;
}	t_lock;

typedef struct s_philo
{
	pthread_t tid;
	int		id;
	int		nb_meals;
	int		is_full;
	long	last_meal_time;
	t_lock	*left_fork;
	t_lock	*right_fork;
	t_data	*data;
} t_philo;

typedef struct s_data
{
	int		nb_philos;
	int		stop_simu;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	int		nb_must_meals;
	long	start_time;
	t_lock	*forks; // tableau des forks = mutex
	t_philo	*philos; // tableau de nos philosophes
}	t_data;

/* PARSING */
int check_data(t_data *data);
int parse_data(t_data *data, char **argv);


/* INIT */
void    init_data(t_data *data);

/* PHILO */
void    *philo_routine(void *arg);

/* UTILS */
long    get_time_in_s_ms(void);
long    get_current_time_in_ms(t_data *data);
int     ft_usleep(long milliseconds);

#endif