/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   finish.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/06 19:02:35 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/06 20:04:17 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int	is_dead(t_philo *philo)
{
	uint64_t	time;

	pthread_mutex_lock(philo->table->times_eat_m);
	time = get_current_time() - philo->last;
	if (time > philo->table->time_to_dead)
	{
		pthread_mutex_unlock(philo->table->times_eat_m);
		pthread_mutex_lock(philo->table->died_m);
		philo->table->flag_dead = 1;
		print_dead(philo);
		pthread_mutex_unlock(philo->table->died_m);
		pthread_mutex_unlock(philo->table->times_eat_m);
		return (1);
	}
	pthread_mutex_unlock(philo->table->died_m);
	pthread_mutex_unlock(philo->table->times_eat_m);
	return (0);
}

static int	check_dead(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philosophers)
	{
		if (is_dead(table->philosophers[i]))
		{
			pthread_mutex_lock(table->msg);
			table->print = 0;
			pthread_mutex_unlock(table->msg);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	filo_end_eat(t_table *table)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (i < table->n_philosophers)
	{
		pthread_mutex_lock(table->times_eat_m);
		if (table->philosophers[i]->times_eat >= table->times_must_eat)
			j++;
		pthread_mutex_unlock(table->times_eat_m);
		if (j == table->n_philosophers)
		{
			printf("All are full\n");
			pthread_mutex_lock(table->end_m);
			table->finish = 1;
			pthread_mutex_unlock(table->end_m);
			return (1);
		}
	}
	return (0);
}

static void	finish_start(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->n_philosophers)
	{
		pthread_mutex_unlock(table->philosophers[i]->l_fork);
		pthread_join(table->philo_thread[i], NULL);
		i++;
	}
	i = 0;
	while (i < table->n_philosophers)
	{
		pthread_mutex_destroy(table->fork + i);
		pthread_mutex_destroy(table->msg + i);
		i++;
	}
	pthread_mutex_destroy(table->start_m);
	pthread_mutex_destroy(table->times_eat_m);
	pthread_mutex_destroy(table->died_m);
	pthread_mutex_destroy(table->end_m);
	free_table(table);
}

int	finish_meal(t_table *table, int argc)
{
	int	i;

	pthread_mutex_lock(table->start_m);
	pthread_mutex_unlock(table->start_m);
	i = 1;
	while (i)
	{
		if (argc == 6)
			if (filo_end_eat(table))
				i = 0;
		if (check_dead(table))
			i = 0;
		pthread_mutex_lock(table->died_m);
		if (table->flag_dead)
			i = 0;
		pthread_mutex_unlock(table->died_m);
	}
	pthread_mutex_lock(table->msg);
	table->print = 0;
	pthread_mutex_unlock(table->msg);
	finish_start(table);
	return (1);
}
