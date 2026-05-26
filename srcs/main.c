/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/14 16:23:30 by arouland          #+#    #+#             */
/*   Updated: 2026/05/27 01:37:13 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

// ATTENTION MODIFIER LE MAKEFILE ENLEVER FSANITIZE !!

int     check_stop_status(t_data *data)
{
    int status;

    pthread_mutex_lock(&data->monitor_lock);
    status = data->stop_simu;
    pthread_mutex_unlock(&data->monitor_lock);
    return (status);
}

void    start_simu(t_data *data)
{
    int i;

    i = 0;
    // si nb limits meals = 0 -> return au. main et clean
    // si 1 seul philo -> fonction spéciale ?
    while (i < data->nb_philos)
    {
        pthread_create(&data->philos[i].tid, NULL, philo_routine, &data->philos[i]);
        i++;
    }
}

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
    
    print_status(data, philo->id, "is eating");
    pthread_mutex_lock(&data->monitor_lock);
    // Monitor lock car check last_meal_time, doit pas checker en même temps que écriture
    philo->last_meal_time = get_current_time_in_ms(data);
    philo->nb_meals++;
    pthread_mutex_unlock(&data->monitor_lock);
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
     if (data->nb_philos == 1)
    {
        pthread_mutex_lock(&philo->left_fork->fork);
        print_status(data, philo->id, "has taken a fork");
        ft_usleep(data->time_to_die);
        pthread_mutex_unlock(&philo->left_fork->fork);
        return (NULL);
    }
    while(check_stop_status(data) == 0)
    {
        // is full? ->
        
        // printf("Entrée boucle\n");
        // printf("Je suis philo n° : %d\n", philo->id);

        // --- EAT ----
        
        take_forks(data, philo);
        philo_eat(data, philo);
        drop_forks(philo);
        
            // --- SLEEP ----
        philo_sleep(data, philo);
        // ---- THINK ----
        philo_think(data, philo);
        // break;
    }
    return (NULL);
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
    pthread_mutex_destroy(&data->write_lock);
    pthread_mutex_destroy(&data->monitor_lock);
    free(data->philos);
    free(data->forks);
    free(data);
}

int is_all_philos_full(t_data *data)
{
    int i;

    i = 0;
    if (data->nb_must_meals == -1)
        return (0);
    pthread_mutex_lock(&data->monitor_lock);
    while (i < data->nb_philos)
    {
        // Accès à nb_meals, pour éviter datarace on le lock en lecture et écriture
        if (data->philos[i].nb_meals < data->nb_must_meals)
        {
            pthread_mutex_unlock(&data->monitor_lock);
            return (0);
        }
        i++;
    }
    pthread_mutex_unlock(&data->monitor_lock);
    return (1);
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
    printf("nb of philos : %d\n", data->nb_philos);
    printf("time to die : %ld\n", data->time_to_die);
    printf("time to eat : %ld\n", data->time_to_eat);
    printf("time to sleep : %ld\n", data->time_to_sleep);
    if (argc == 6)
        printf("must_meals %d\n", data->nb_must_meals);
    //1776533395091 -> xxx95 = secondes entières; 091 = millisecondes
    printf("Timeval sec + usec: %ld\n", data->start_time);
    long timenow = get_time_in_s_ms();
    printf("gettime timemow: %ld\n", timenow);
    ft_usleep(data->time_to_sleep);
    long newtime = get_time_in_s_ms();
    printf("gettime newtime: %ld\n", newtime);
    printf("Diff: %ld\n", newtime - timenow);
    start_simu(data);
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
        if ((data->nb_must_meals != 1) && is_all_philos_full(data) == 1)
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