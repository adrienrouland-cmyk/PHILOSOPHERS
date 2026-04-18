/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:23:30 by arouland          #+#    #+#             */
/*   Updated: 2026/04/19 01:41:30 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

static int  is_space(char c)
{
    if ((c >= 9 && c <= 13) || c == 32)
        return (1);
    return (0);
}

long    ft_atol(char *str)
{
    long    nb;
    int     i;
    int negative;

    i = 0;
    nb = 0;
    negative = 1;
    while (is_space(str[i]) == 1)
        i++;
    if (str[i] == '\0')
        return (0);
    if (str[i] == '+' && str[i + 1] != '-')
        i++;
    if (str[i] == '-')
    {
        negative = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        nb = nb * 10 + (str[i] - 48);
        i++;
    }
    nb = nb * negative;
    return (nb);
}
// Mettre en place la protection de l'overflow ?

int check_digit(char **argv)
{
    int i;
    int j;

    i = 1;
    while (argv[i])
    {
        j = 0;
        while (argv[i][j])
        {
            if (argv[i][j] < '0' || argv[i][j] > '9')
                return (1);
            j++;
        }
        i++;
    }
    return (0);
}

int check_data(t_data *data)
{
    // LONG_MAX
    if (data->nb_philos > INT_MAX || data->time_to_die > LONG_MAX
        || data->time_to_eat > LONG_MAX || data->time_to_sleep > LONG_MAX)
        return (write(1, "Arguments errors : over LONG_MAX", 17), 1);
    if (data->nb_philos <= 0 || data->time_to_die <= 0
        || data->time_to_eat <= 0 || data->time_to_sleep <= 0)
        return (write(1, "Arguments errors", 17), 1);

    // Others ?
    return (0);
}

int parse_data(t_data *data, char **argv)
{
    struct timeval tval;
    
    if (check_digit(argv) == 1)
        return (1);
    data->nb_philos = (int)ft_atol(argv[1]);
    data->time_to_die = ft_atol(argv[2]);
    data->time_to_eat = ft_atol(argv[3]);
    data->time_to_sleep = ft_atol(argv[4]);
    if (argv[5])
        data->nb_must_meals = (int)ft_atol(argv[5]);
    gettimeofday(&tval, NULL);
    printf("Timeval sec: %ld\n", tval.tv_sec);
    printf("Timeval usec: %d\n", tval.tv_usec);
    data->start_time = tval.tv_sec * 1000 + tval.tv_usec/1000;
    return (0);
}

void    *philo_routine(void *arg)
{
    t_philo *philo;
    t_data  *data;
    
    philo = (t_philo *)arg;
    data = philo->data;
    printf("Début routine\n");
    while(data->stop_simu == 0)
    {
        printf("Entrée boucle\n");
        break;
    }

    // La routine est une boucle qui continue tant que is_stop != 1
    
    // take forks
    // eat
    // put_forks
    // sleep philo
    // think
    return (NULL);
}

void    init_data(t_data *data)
{
    int i;

    i = 0;
    data->stop_simu = 0;
    data->philos = malloc(sizeof(t_philo) * data->nb_philos);
    if (!data->philos)
        return ;
    data->forks = malloc(sizeof(t_lock) * data->nb_philos);
    if (!data->forks)
        return ;
    while (i < data->nb_philos)
    {
        pthread_mutex_init(&data->forks[i].fork, NULL);
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
// Si 4 philos : philo 0 prend fourchettes 0 et 1
// philo 1 prend fourchettes 1 et 2
// 2 -> 2 et 3 ; 3 -> 3 et 4; 4 -> 4 et 0.

void    start_simu(t_data *data)
{
    int i;

    i = 0;
    while (i < data->nb_philos)
    {
        pthread_create(&data->philos[i].tid, NULL, philo_routine, &data->philos[i]);
        i++;
    }
}

int	main(int argc, char *argv[])
{
	t_data *data;
        
    if (argc != 5 && argc != 6)
        return (write(1, "Wrongs arguments", 17), 1);
    data = malloc(sizeof(t_data));
    if (!data)
        return (1);
    if (parse_data(data, argv) == 1)
        return (write(1, "Parsing error", 14), 1);
    if (check_data(data) == 1)
        return (1);
    init_data(data);
    
    // ------- DEBUG -----------------------------
    printf("nb of philos : %d\n", data->nb_philos);
    printf("time to die : %ld\n", data->time_to_die);
    printf("time to eat : %ld\n", data->time_to_eat);
    printf("time to sleep : %ld\n", data->time_to_sleep);
    //1776533395091 -> xxx95 = secondes entières; 091 = millisecondes
    printf("Timeval sec + usec: %ld\n", data->start_time);
    
    start_simu(data);
    // -----------------------------------------
    return (0);
}
//./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]
// ./philo 5 800 200 200

// --- EAT ----
// prendre les 2 mutex left_fork et right_fork
// afficher "is_eating"
// maj last_meal_time avec le temps actuel
// utilsier fct ft_usleep(data->time_to_eat);
// unlock les mutex

// ---- SLEEP ----
// afficher is_sleeping
// utiliser ft_usleep(data->time_to_sleep);

// ---- THINK ----
// afficher "is_thinking"
// puis le philo repart au début de la boucle pour essayer de manger.