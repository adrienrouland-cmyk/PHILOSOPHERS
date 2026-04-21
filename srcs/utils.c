/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/19 10:01:16 by arouland          #+#    #+#             */
/*   Updated: 2026/04/22 01:21:25 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

void    print_status(t_data *data, int philo_id, char *msg)
{
    pthread_mutex_lock(&data->write_lock);
    if (data->stop_simu == 0)
    {
        printf("%ld %d %s\n", get_current_time_in_ms(data), philo_id, msg);
    }
    pthread_mutex_unlock(&data->write_lock);
}

long    get_time_in_s_ms(void)
{
    struct timeval  time;

    gettimeofday(&time, NULL);
    return (time.tv_sec * 1000 + time.tv_usec / 1000);
}
// Renvoie un timestamp total en ms depuis 1970
// 62496 -> 62 = secondes. 496 = ms.

long    get_current_time_in_ms(t_data *data)
{
    return get_time_in_s_ms() - data->start_time;
}
// Renvoie un timestamp total en ms depuis 1970
// 62496 -> 62 = secondes. 496 = ms.

int     ft_usleep(long milliseconds)
{
    long    start_time;

    start_time = get_time_in_s_ms();
    while (get_time_in_s_ms() - start_time < milliseconds)
        usleep(500);
    return (0);
}
//usleep non précis donc on le fait par bloc de 500 microsecondes
// Donc de 0,5 ms.