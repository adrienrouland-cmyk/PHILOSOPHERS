/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arouland <arouland@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/17 15:52:10 by arouland          #+#    #+#             */
/*   Updated: 2026/04/17 21:33:48 by arouland         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <pthread.h>

#define TIMES_TO_COUNT 21000

typedef struct s_locks
{
    pthread_mutex_t lock_1;
    pthread_mutex_t lock_2;
    unsigned int count;
}   t_locks;

void    *thread_1_routine(void *data)
{
    pthread_t tid;
    t_locks *locks;
    int i;

    tid = pthread_self();
    locks = (t_locks *)data;
    i = 0;

    printf("Thread %p veut verrouiller lock 1\n", (void *)tid);
    pthread_mutex_lock(&locks->lock_1);
    printf("Thread %p possède lock 1\n", (void *)tid);
    printf("Thread %p veut verrouiller lock 2\n", (void *)tid);
    pthread_mutex_lock(&locks->lock_2);
    printf("Thread %p possède lock 2\n", (void *)tid);
    while (i < TIMES_TO_COUNT)
    {
        locks->count += 1;
        i++;        
    }
    printf("Thread %p déverrouille lock 2\n", (void *)tid);
    pthread_mutex_unlock(&locks->lock_2);
    printf("Thread %p déverrouille lock 1\n", (void *)tid);
    pthread_mutex_unlock(&locks->lock_1);
    printf("Thread %p : Terminé !\n", (void *)tid);
    return (NULL);
}

void    *thread_2_routine(void *data)
{
    pthread_t tid;
    t_locks *locks;
    int i;

    tid = pthread_self();
    locks = (t_locks *)data;
    i = 0;
    
    printf("Thread %p veut verrouiller lock 2\n", (void *)tid);
    pthread_mutex_lock(&locks->lock_2);
    printf("Thread %p possède lock 2\n", (void *)tid);
    printf("Thread %p veut verrouiller lock 1\n", (void *)tid);
    pthread_mutex_lock(&locks->lock_1);
    printf("Thread %p possède lock 1\n", (void *)tid);
    while (i < TIMES_TO_COUNT)
    {
        locks->count += 1;
        i++;        
    }
    printf("Thread %p déverrouille lock 1\n", (void *)tid);
    pthread_mutex_unlock(&locks->lock_1);
    printf("Thread %p déverrouille lock 2\n", (void *)tid);
    pthread_mutex_unlock(&locks->lock_2);
    printf("Thread %p : Terminé !\n", (void *)tid);
    return (NULL);
}

int main(void)
{
    pthread_t tid1;
    pthread_t tid2;
    t_locks locks;

    locks.count = 0;

    pthread_mutex_init(&locks.lock_1, NULL);
    pthread_mutex_init(&locks.lock_2, NULL);

    pthread_create(&tid1, NULL, thread_1_routine, &locks);
    printf("Création 1er thread\n");
    pthread_create(&tid2, NULL, thread_2_routine, &locks);
    printf("Création 2è thread\n");

    pthread_join(tid1, NULL);
    printf("Union 1er thread\n");
    pthread_join(tid2, NULL);
    printf("Union 2è thread\n");

    printf("FIN - Final count : %u\n", locks.count);
}

// typedef struct s_counter
// {
//     pthread_mutex_t count_mutex;
//     unsigned int count;
// } t_counter;

// void    *thread_routine(void *data)
// {
//     pthread_t tid;
//     t_counter *counter;
//     unsigned int i;

//     tid = pthread_self();
//     counter = (t_counter *)data;

//     pthread_mutex_lock(&counter->count_mutex);
//     printf("Thread %p : compte au départ = %u\n", (void *)tid, (*counter).count);
//     pthread_mutex_unlock(&counter->count_mutex);
//     i = 0;
//     while (i < TIMES_TO_COUNT)
//     {
//         pthread_mutex_lock(&counter->count_mutex);
//         counter->count++;
//         pthread_mutex_unlock(&counter->count_mutex);
//         i++;
//     }

//     pthread_mutex_lock(&counter->count_mutex);
//     printf("Thread %p : compte final = %u\n", (void *)tid, (*counter).count);
//     pthread_mutex_unlock(&counter->count_mutex);
//     return (NULL);
// }

// int main(void)
// {
//     pthread_t tid1;
//     pthread_t tid2;

//     t_counter counter;
//     counter.count = 0;
//     pthread_mutex_init(&counter.count_mutex, NULL);
//     printf("Main : le compte attendu est de %u\n", 2* TIMES_TO_COUNT);

//     pthread_create(&tid1, NULL, thread_routine, &counter);
//     printf("Création du premier thread : %p\n", (void *)tid1);
//     pthread_create(&tid2, NULL, thread_routine, &counter);
//     printf("Création du second thread : %p\n", (void *)tid2);

//     pthread_join(tid1, NULL);
//     printf("Main : Union du premier thread %p\n", (void *)tid1);
//     pthread_join(tid2, NULL);
//     printf("Main : Union du second thread %p\n", (void *)tid2);
// }

// void    *thread_routine(void *data)
// {
//     pthread_t tid;
//     unsigned int *count;
//     unsigned int i;

//     tid = pthread_self();
//     count = (unsigned int *)data;

//     printf("Thread %p: compte au départ : %u\n", (void *)tid, *count);
//     i = 0;
//     while (i < TIMES_TO_COUNT)
//     {
//         (*count)++;
//         i++;
//     }
//     printf("Thread - Compte final : %u\n", *count);
//     return (NULL);
// }

// int main(void)
// {
//     pthread_t tid1;
//     pthread_t tid2;

//     unsigned int count;

//     count = 0;

//     printf("Main : Le compte attendu est de %u\n", 2*TIMES_TO_COUNT);

//     pthread_create(&tid1, NULL, thread_routine, &count);
//     printf("Main: Creation du premier thread %p\n", (void *)tid1);

//     pthread_create(&tid2, NULL, thread_routine, &count);
//     printf("Main : Création du second thread %p\n", (void *)tid2);

//     pthread_join(tid1, NULL);
//     pthread_join(tid2, NULL);

//     if (count != (2*TIMES_TO_COUNT))
//         printf("ERREUR : Le compte est de %u", count);
//     else
//         printf("GOOD");
// }

// void    *thread_fct1(void *data)
// {
//     int *shared;

//     shared = (int *)data;
//     *shared = 10;
//     return (NULL);
// }

// void    *thread_fct2(void *data)
// {
//     int *shared;

//     shared = (int *)data;
//     *shared = 15;
//     return (NULL);
// }

// int main(void)
// {
//     pthread_t tid1;
//     pthread_t tid2;
//     int x = 0;

//     printf("Value : %d\n", x);
//     pthread_create(&tid1, NULL, thread_fct1, &x);
//     pthread_create(&tid2, NULL, thread_fct2, &x);

//     pthread_join(tid2, NULL);
//     pthread_join(tid1, NULL);

//     printf("Final value : %d\n", x);
// }

// void *thread_routine(void *data)
// {
//     pthread_t tid;

//     (void)data;
//     tid = pthread_self();
//     printf("Thread : %p\n", (void *)tid);
//     return (NULL);
// }

// int main(void)
// {
//     pthread_t tid1;
//     pthread_t tid2;

//     pthread_create(&tid1, NULL, thread_routine, NULL);
//     printf("Main : création du 1er thread %p\n", (void *)tid1);
    
//     pthread_create(&tid2, NULL, thread_routine, NULL);
//     printf("Main: Creation du second thread %p\n", (void *)tid2);

//     pthread_join(tid1, NULL);
//     printf("Main: Union du premier thread %p\n", (void *)tid1);

//     pthread_join(tid2, NULL);
//     printf("Main : Union du second thread %p\n", (void *)tid2);
//     return (0);
// }