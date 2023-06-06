/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 18:50:38 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/06 19:08:26 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	init_philo(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = 0;
	while (i < table->n_philosophers)
	{
		if (pthread_mutex_init(table->fork + i, NULL) != 0)
			return (0);
		philo = table->philosophers[i];
		philo->philosopher = i + 1;
		philo->msg = table->msg;
		philo->l_fork = &(table->fork[((i + 1) % table->n_philosophers)]);
		philo->r_fork = &(table->fork[i]);
		philo->last = table->start;
		philo->table = table;
	}
	return (1);
}

int	init_mutex(t_table *table, int x)
{
	table->msg = malloc(sizeof(pthread_mutex_t));
	if (!table->msg)
		return (0);
	if (pthread_mutex_init(table->msg, NULL) != 0)
		return (free(table->msg), 0);
	table->fork = malloc(sizeof(pthread_mutex_t) * x);
	if (!table->fork)
		return (free(table->msg), 0);
	return (1);
}

int	init(t_table *table)
{
	int	i;
	int	x;

	x = table->n_philosophers;
	table->start = get_current_time();
	table->finish = 0;
	table->flag_dead = 0;
	table->print = 1;
	table->philosophers = malloc(sizeof(t_philo *) * x);
	if (!table->philosophers)
		return (0);
	i = 0;
	while (i < x)
	{
		table->philosophers[i] = malloc(sizeof(t_philo));
		if (!table->philosophers[i])
			return (free_mtx(table, i), 0);
		i++;
	}
	table->philo_thread = malloc(sizeof(pthread_t) * x);
	if (!table->philo_thread)
		return (free_mtx(table, i), 0);
	if (!init_mutex(table, x))
		return (free_mtx(table, i), 0);
	return (1);
}

int	take_args(t_table *table, char **argv)
{
	table->n_philosophers = ft_atoi(argv[1]);
	table->time_to_dead = ft_atoi(argv[2]);
	table->time_to_eat = ft_atoi(argv[3]);
	table->time_to_sleep = ft_atoi(argv[4]);

	if (argv[5])
		table->times_must_eat = ft_atoi(argv[5]);
	return (1);
}

int	check_args(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (0);
	if (!argv[0])
		return (0);
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 0)
			return (0);
		i++;
	}
	return (1);
}
