/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:49:15 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/06 20:03:23 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	free_table(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philosophers)
	{
		free(table->philosophers[i]);
		i++;
	}
	free(table->fork);
	free(table->msg);
	free(table->start_m);
	free(table->times_eat_m);
	free(table->died_m);
	free(table->end_m);
	free(table->philo_thread);
	free(table->philosophers[i]);
}

static int	start_meal(t_table *table)
{
	int			i;
	pthread_t	*philo_t;
	t_philo		*philo_s;

	i = 0;
	pthread_mutex_lock(table->start_m);
	while (i < table->n_philosophers)
	{
		philo_t = table->philo_thread + i;
		philo_s = table->philosophers[i];
		if (!pthread_create(philo_t, NULL, filosofofo, philo_s))
			return (printf("Failed to create a thread!\n"), 0);
	}
	pthread_mutex_unlock(table->start_m);
	return (1);
}

void	ft_leaks(void)
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	t_table	table;

	//atexit(ft_leaks);
	if (!check_args(argc, argv))
		return (printf("Invalid arguments!\n"), 0);
	if (!take_args(&table, argv))
		return(printf("Imposible to take arg's!\n"), 0);
	if (!init(&table))
		return (printf("Failed to initialize struct!\n"), 0);
	if (!init_philo(&table))
		return (printf("Failed to initialize philosopher's struct!\n"), 0);
	if (!start_meal(&table))
		return (printf("Failed to start meal!\n"), 0);
	if (!finish_meal(&table, argc))
		return (0);
	return (0);
}
