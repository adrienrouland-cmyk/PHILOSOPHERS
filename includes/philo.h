/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:23:42 by arouland          #+#    #+#             */
/*   Updated: 2026/05/29 10:26:28 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_lock
{
	pthread_mutex_t		fork;
	int					fork_id;
}						t_lock;

typedef struct s_philo
{
	pthread_t			tid;
	int					id;
	int					is_full;
	int					nb_meals;
	long				last_meal_time;
	t_lock				*left_fork;
	t_lock				*right_fork;
	t_data				*data;
	int					thread_created;
}						t_philo;

typedef struct s_data
{
	int					nb_philos;
	int					stop_simu;
	int					all_philos_ready;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	int					nb_must_meals;
	long				start_time;
	pthread_t			monitor_thread;
	pthread_mutex_t		write_lock;
	pthread_mutex_t		monitor_lock;
	t_lock				*forks;
	t_philo				*philos;
	int					monitor_created;
}						t_data;

/* PARSING */
int						check_data(t_data *data, char **argv, int is_overflow);
int						parse_data(t_data *data, char **argv);

/* INIT */
int						init_data(t_data *data);

/* SIMU */
void					stop_clean_simulation(t_data *data);
void					philo_simulation(t_data *data);

/* PHILO ROUTINE */
void					*lone_philo_routine(void *arg);
void					*philo_routine(void *arg);

/* MONITOR */
void					*monitor_routine(void *arg);

/* UTILS */
long					get_time_in_s_ms(void);
long					get_current_time_in_ms(t_data *data);
int						ft_usleep(long milliseconds, t_data *data);
void					print_status(t_data *data, int philo_id, char *msg);
int						check_stop_status(t_data *data);
int						is_all_philos_full(t_data *data);
int						is_end_simu(t_data *data);
void					wait_all_threads(t_data *data);
void					set_early_stop(t_data *data);

/* GET-SET */
void					set_bool(pthread_mutex_t *mutex, int *dest, int value);
int						get_bool(pthread_mutex_t *mutex, int *value);
void					set_long(pthread_mutex_t *mutex, long *dest,
							long value);
long					get_long(pthread_mutex_t *mutex, long *value);

#endif
