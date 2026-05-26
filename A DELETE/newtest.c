/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   newtest.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 12:40:15 by arouland          #+#    #+#             */
/*   Updated: 2026/05/26 19:59:32 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// /*Deadlock -> blocage*/

// #include <pthread.h>
// #include <unistd.h>
// #include <stdio.h>

// typedef struct s_mutex
// {
// 	pthread_mutex_t mutex;
// 	int i;
// 	int go;
// }	t_mut;

// void	*go1(void *data)
// {
// 	char	*str;
// 	int i = 0;
// 	t_mut *lock;

// 	lock = (t_mut *)data;
// 	str = "tread 1 : hello\n";
// 	pthread_mutex_lock(&lock->mutex);
// 	printf("go : %d\n", lock->go);
// 	lock->go = 1;
// 	pthread_mutex_unlock(&lock->mutex);
// 	pthread_mutex_lock(&lock->mutex);
// 	while(str[i])
// 	{
// 		write(1, &str[i], 1);
// 		i++;
// 	}
// 	pthread_mutex_unlock(&lock->mutex);
// 	return (NULL);
// }

// void	*go2(void *data)
// {
// 	char	*str;
// 	int i = 0;
// 	t_mut *lock;

// 	lock = (t_mut *)data;
// 	str = "tread 2 : hello\n";
// 	pthread_mutex_lock(&lock->mutex);
// 	printf("go : %d\n", lock->go);
// 	pthread_mutex_unlock(&lock->mutex);
// 	pthread_mutex_lock(&lock->mutex);
// 	while(str[i])
// 	{
// 		write(1, &str[i], 1);
// 		i++;
// 	}
// 	pthread_mutex_unlock(&lock->mutex);
// 	return (NULL);
// }

// int	main(void)
// {
// 	pthread_t tid1;
// 	pthread_t tid2;
// 	t_mut lock;
	
// 	pthread_mutex_init(&lock.mutex, NULL);
// 	lock.go = 0;
// 	pthread_create(&tid1, NULL, go1, &lock);
// 	pthread_create(&tid2, NULL, go2, &lock);
// 	pthread_join(tid1, NULL);
// 	pthread_join(tid2, NULL);
// 	pthread_mutex_destroy(&lock.mutex);
// }

/*****.Deadlock  */

// #include <pthread.h>
// #include <unistd.h>
// #include <stdio.h>

// typedef struct s_mutex
// {
// 	pthread_mutex_t mutex;
// 	int i;
// }	t_mut;

// void	*go1(void *data)
// {
// 	char	*str;
// 	int i = 0;
// 	t_mut *lock;

// 	lock = (t_mut *)data;
// 	str = "tread 1 : hello\n";
// 	pthread_mutex_lock(&lock->mutex);
// 	while(str[i])
// 	{
// 		write(1, &str[i], 1);
// 		i++;
// 	}
// 	pthread_mutex_unlock(&lock->mutex);
// 	return (NULL);
// }

// void	*go2(void *data)
// {
// 	char	*str;
// 	int i = 0;
// 	t_mut *lock;

// 	lock = (t_mut *)data;
// 	str = "tread 2 : hello\n";
// 	pthread_mutex_lock(&lock->mutex);
// 	while(str[i])
// 	{
// 		write(1, &str[i], 1);
// 		i++;
// 	}
// 	pthread_mutex_unlock(&lock->mutex);
// 	return (NULL);
// }

// int	main(void)
// {
// 	pthread_t tid1;
// 	pthread_t tid2;
// 	t_mut lock;
	
// 	pthread_mutex_init(&lock.mutex, NULL);
// 	pthread_create(&tid1, NULL, go1, &lock);
// 	pthread_create(&tid2, NULL, go2, &lock);
// 	pthread_join(tid1, NULL);
// 	pthread_join(tid2, NULL);
// 	pthread_mutex_destroy(&lock.mutex);
// }


/*** Avec des mutex  */

// #define TIMES_TO_COUNT 21000
// #include <pthread.h>
// #include <stdio.h>

// typedef struct s_counter
// {
// 	pthread_mutex_t count_mutex;
// 	unsigned int count;
// }	t_counter;

// void	*thread_routine(void *data)
// {
// 	pthread_t tid;
// 	t_counter *counter;
// 	unsigned int i;

// 	tid = pthread_self();
// 	counter = (t_counter *)data;

// 	pthread_mutex_lock(&counter->count_mutex);
// 	printf("Thread %p: compte au départ = %u\n", (void *)tid, (*counter).count);
// 	pthread_mutex_unlock(&counter->count_mutex);
// 	i = 0;
// 	while (i < TIMES_TO_COUNT)
// 	{
// 		pthread_mutex_lock(&counter->count_mutex);
// 		(*counter).count++;
// 		pthread_mutex_unlock(&counter->count_mutex);
// 		i++;
// 	}
// 	printf("Thread %p: compte final = %u\n", (void *)tid, (*counter).count);
// 	return (NULL); // Obligatoire !!
// }

// // DATA RACE : pour passer de 10 à 11, le font en même temps
// // donc passe à 11, pas à 12

// int	main(void)
// {
// 	pthread_t tid1;
// 	pthread_t tid2;
// 	t_counter counter;

// 	counter.count = 0;
// 	pthread_mutex_init(&counter.count_mutex, NULL);
// 	printf("Compte attendu = %u\n", 2 * TIMES_TO_COUNT);
// 	pthread_create(&tid1, NULL, thread_routine, &counter);
// 	printf("Création du premier thread : %p\n", (void *)tid1);
// 	pthread_create(&tid2, NULL, thread_routine, &counter);
// 	printf("Création du second thread : %p\n", (void *)tid1);
	
// 	pthread_join(tid1, NULL);
// 	pthread_join(tid2, NULL);
// }

/* Erreur de synchronisation */

// #define TIMES_TO_COUNT 21000
// #include <pthread.h>
// #include <stdio.h>

// void	*thread_routine(void *data)
// {
// 	pthread_t tid;
// 	unsigned int *count;
// 	unsigned int i;

// 	tid = pthread_self();
// 	count = (unsigned int *)data;

// 	printf("Thread %p: compte au départ = %u\n", (void *)tid, *count);
// 	i = 0;
// 	while (i < TIMES_TO_COUNT)
// 	{
// 		(*count)++;
// 		i++;
// 	}
// 	printf("Thread %p: compte final = %u\n", (void *)tid, *count);
// 	return (NULL); // Obligatoire !!
// }

// // DATA RACE : pour passer de 10 à 11, le font en même temps
// // donc passe à 11, pas à 12

// int	main(void)
// {
// 	pthread_t tid1;
// 	pthread_t tid2;

// 	unsigned int count;

// 	count = 0;

// 	printf("Compte attendu = %u\n", 2 * TIMES_TO_COUNT);
// 	pthread_create(&tid1, NULL, thread_routine, &count);
// 	printf("Création du premier thread : %p\n", (void *)tid1);
// 	pthread_create(&tid2, NULL, thread_routine, &count);
// 	printf("Création du second thread : %p\n", (void *)tid1);
	
// 	pthread_join(tid1, NULL);
// 	pthread_join(tid2, NULL);
// }


/*********** */

// #include <stdio.h>
// #include <pthread.h>
// #include <unistd.h>

// void	*thread_routine(void *data)
// {
// 	pthread_t tid;

// 	(void)data;
// 	tid = pthread_self(); //récup l'id du thread qui exécute
// 	printf("Thread : %p\n", (void *)tid);
// 	return (NULL);
// }

// int	main(void)
// {
// 	pthread_t tid1;
// 	pthread_t tid2;

// 	pthread_create(&tid1, NULL, thread_routine, NULL);
// 	printf("Création du premier thread %p\n", (void *)tid1);
// 	pthread_create(&tid2, NULL, thread_routine, NULL);
// 	printf("Création du deuxième thread %p\n", (void *)tid2);

// 	pthread_join(tid1, NULL);
// 	printf("Main : Union du premier thread %p\n", (void *)tid1);
// 	pthread_join(tid2, NULL);
// 	printf("Main : Union du second thread %p\n", (void *)tid2);

// 	return (0);
// }