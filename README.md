*This project has been created as part of the 42 curriculum by arouland.*

**Description**

The goal of the philosophers project is to learn multithreading, synchronization, mutexes, and concurrent programming through the Dining Philosophers problem.

Several philosophers sit around a table. Each philosopher alternates between:

- eating,
- sleeping,
- thinking.

To eat, a philosopher must pick up two forks:

- one on the left,
- one on the right.

Since forks are shared resources protected by mutexes, the main challenge of the project is to prevent:

- race conditions,
- deadlocks,
- starvation,
while keeping the simulation stable and precise.

**Instructions**

- Compilation

make

- Execution
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [number_of_times_each_philosopher_must_eat]

- Example
./philo 5 800 200 200


To add in stop_clean_simulation to check the number of meals by philosophers

    // --- Number of meals ---
    printf("\n============ Bilan ============\n");
    i = 0;
    while (i < data->nb_philos)
    {
        printf("Philosophe %d has eaten %d meals.\n", 
            data->philos[i].id, data->philos[i].nb_meals);
        i++;
    }
    printf("================================================\n\n");
    // -----------------------------------------------------------

**Resources**

- https://www.codequoi.com/threads-mutex-et-programmation-concurrente-en-c/
- https://medium.com/@jalal92/the-dining-philosophers-7157cc05315
- Talking with friends
