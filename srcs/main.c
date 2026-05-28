/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/27 22:00:03 by arouland          #+#    #+#             */
/*   Updated: 2026/05/27 23:46:09 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int main(int argc, char **argv)
{
    t_data *data;

    if (argc != 5 && argc != 6)
        return (write(2, "Wrongs arguments", 17), 1);
    data = malloc(sizeof(t_data));
    if (!data)
        return (1);
    if (parse_data(data, argv) == 1)
        return (free(data), 1);
    if (init_data(data) == 1)
        return (free(data), write(2, "Malloc errors\n", 14), 1);
    philo_simulation(data);
    stop_clean_simulation(data);
}