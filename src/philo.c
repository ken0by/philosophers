/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:36:34 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/07 17:10:42 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int	check_args(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (0);
	i = 0;
	while (++i < argc)
		if (ft_atoi(argv[i]) <= 0)
			return (0);
	return (1);
}

static int	take_args(t_table *table, char **argv)
{
	table->n_philo = ft_atoi(argv[1]);
	table->time_to_dead = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);
	if (argv[5])
		table->times_must_eat = ft_atoi(argv[5]);
	if (!table->n_philo || !table->time_to_dead
		|| !table->time_to_eat || !table->time_to_sleep
		|| (argv[5] && (!table->times_must_eat)))
		return (0);
	return (1);
}

static int	init_threads(t_table *table)
{
	int	i;

	table->start = get_current_time();
	table->finish = 0;
	table->flag_dead = 0;
	table->print = 1;
	table->philosophers = malloc(sizeof(t_philo *) * table->n_philo);
	i = -1;
	while (++i <= table->n_philo)
	{
		table->philosophers[i] = malloc(sizeof(t_philo));
		if (!table->philosophers[i])
			return (free_mtx(table, i), 0);
	}
	table->msg = malloc(sizeof(pthread_mutex_t));
	if (!table->msg)
		return (free_mtx(table, i), 0);
	if (pthread_mutex_init(table->msg, NULL) != 0)
		return (free_mtx(table, i), free(table->msg), 0);
	table->fork = malloc(sizeof(pthread_mutex_t) * table->n_philo);
	if (!table->fork)
		return (free_mtx(table, i), free(table->msg), 0);
	return (1);
}

static int	init_philo(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->n_philo)
	{
		if (pthread_mutex_init(table->fork + i, NULL) != 0)
			return (0);
		philo = table->philosophers[i];
		philo->id = i + 1;
		philo->msg = table->msg;
		philo->l_fork = &(table->fork[(i + 1) % table->n_philo]);
		philo->r_fork = &(table->fork[i]);
		philo->last = table->start;
		philo->table = table;
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (!check_args(argc, argv))
		return (printf("Invalis arguments\n"), 0);
	if (!take_args(&table, argv))
		return (printf("Error will taking args\n"), 0);
	if (!init_threads(&table))
		return (printf("Failed to initialize threads struct\n"), 0);
	if (!init_philo(&table))
		return (free_mtx(&table, table.n_philo),
			printf("Failed to initialize philosophers struct\n"), 0);
	if (!start_meal(&table))
		return (printf("Failed to start\n"), 0);
	if (!finish(&table, argc))
		return (0);
	printf("OK\n");
	return (0);
}
