*This project has been created as part of the 42 curriculum by arouland.*

**Description**

Temps avant la mort : 
time_to_die - (current_time - last_meal_time);
time_to_die - (current_time - last_meal_time) - time_to_eat;
= temps possible pour réfléchir
Marge de sécurité en divisant par 2.

**Instructions**


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
