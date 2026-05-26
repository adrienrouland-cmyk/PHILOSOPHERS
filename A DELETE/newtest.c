/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newtest.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:40:15 by arouland          #+#    #+#             */
/*   Updated: 2026/05/26 12:53:16 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int	main(void)
{
	pthread_t tid1;
	pthread_t tid2;

	pthread_create(&tid1, NULL, thread_routine, NULL)
}