/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:23:30 by arouland          #+#    #+#             */
/*   Updated: 2026/05/27 13:20:34 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// ATTENTION MODIFIER LE MAKEFILE ENLEVER FSANITIZE !!
void    *monitor_routine(void *arg)
{
    t_data *data;

    data = (t_data *)arg;

     // --- Thread de monitoring ---
    while (check_stop_status(data) == 0)
    {
        int i = 0;
        while (i < data->nb_philos)
        {
            // printf(" 1. %ld 2. %ld \n", get_current_time_in_ms(data) - data->philos[i].last_meal_time, data->time_to_die);
            pthread_mutex_lock(&data->monitor_lock);
            // on mutex le monitor lock car philos écrivent last meal time tout le temps
            if (get_current_time_in_ms(data) - data->philos[i].last_meal_time > data->time_to_die)
            {
                data->stop_simu = 1;
                pthread_mutex_lock(&data->write_lock);
                printf("%ld %d %s\n", get_current_time_in_ms(data), data->philos[i].id, "has died");
                pthread_mutex_unlock(&data->write_lock);
                // Le mettre avant pour éviter que soit skip.
                pthread_mutex_unlock(&data->monitor_lock);
                // si has died on sort ici
                break;
            }
            pthread_mutex_unlock(&data->monitor_lock);
            i++;
        }
        if ((data->nb_must_meals != -1) && is_all_philos_full(data) == 1)
        {
            pthread_mutex_lock(&data->monitor_lock);
            data->stop_simu = 1;
            pthread_mutex_unlock(&data->monitor_lock);
            pthread_mutex_lock(&data->write_lock);
            printf("All philosophers ate %d times\n", data->nb_must_meals);
            pthread_mutex_unlock(&data->write_lock);
            break ;
        }
    }
    return (NULL);
}

void    *lone_philo_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    print_status(philo->data, philo->id, "has taken a fork");
    pthread_mutex_lock(&philo->data->monitor_lock);
    while (philo->data->stop_simu == 0)
        ft_usleep(200);
    pthread_mutex_unlock(&philo->data->monitor_lock);
    return (NULL);
}

void    start_simu(t_data *data)
{
    int i;

    i = 0;
    // si nb limits meals = 0 -> return au. main et clean
    // si 1 seul philo -> fonction spéciale ?
    if (data->nb_must_meals == 0)
        return ;
    else if (data->nb_philos == 1)
        pthread_create(&data->philos[0].tid, NULL, lone_philo_routine, &data->philos[0]);
    data->start_time = get_time_in_s_ms();
    while (i < data->nb_philos)
    {
        pthread_create(&data->philos[i].tid, NULL, philo_routine, &data->philos[i]);
        i++;
    }
    pthread_create(&data->monitor_thread, NULL, monitor_routine, data);
    pthread_mutex_lock(&data->monitor_lock);
    data->all_philos_ready = 1;
    pthread_mutex_unlock(&data->monitor_lock);
}

void    stop_and_clean_simu(t_data *data)
{
    int i;
    
    i = 0;
    while (i < data->nb_philos)
    {
        pthread_join(data->philos[i].tid, NULL);
        i++;
    }
    i = 0;
    while (i < data->nb_philos)
    {
        pthread_mutex_destroy(&data->forks[i].fork);
        i++;
    }
    pthread_join(data->monitor_thread, NULL);
    pthread_mutex_destroy(&data->write_lock);
    pthread_mutex_destroy(&data->monitor_lock);
    free(data->philos);
    free(data->forks);
    free(data);
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
        return (write(1, "Incorrect datas", 14), 1);
    init_data(data);
    
    // ------- DEBUG -----------------------------
    // printf("nb of philos : %d\n", data->nb_philos);
    // printf("time to die : %ld\n", data->time_to_die);
    // printf("time to eat : %ld\n", data->time_to_eat);
    // printf("time to sleep : %ld\n", data->time_to_sleep);
    // if (argc == 6)
    //     printf("must_meals %d\n", data->nb_must_meals);
    // //1776533395091 -> xxx95 = secondes entières; 091 = millisecondes
    // printf("Timeval sec + usec: %ld\n", data->start_time);
    // long timenow = get_time_in_s_ms();
    // printf("gettime timemow: %ld\n", timenow);
    // ft_usleep(data->time_to_sleep);
    // long newtime = get_time_in_s_ms();
    // printf("gettime newtime: %ld\n", newtime);
    // printf("Diff: %ld\n", newtime - timenow);

    // ---- FIN DEBUG --------

    start_simu(data);
    // -----------------------------------------
    stop_and_clean_simu(data);
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