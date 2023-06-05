/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filosofofo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 13:17:43 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/05 18:32:21 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

uint64_t	get_current_time(void)
{
	struct timeval	tv;
	uint64_t		millisec;

	gettimeofday(&tv, NULL);
	millisec = (uint64_t)tv.tv_sec * 1000 + (uint64_t)tv.tv_usec / 1000;
	return (millisec);
}

void	check_if_died(t_table *table)
{
	uint64_t	sec;

	sec = get_current_time() - table->time.time_last;
	if (sec >= table->time.time_to_dead)
		ft_error("Philosopher's died\n");
	printf("Philosopher's alive\n");
}

void	eat(t_table *table)
{
	table->philosophers.l_fork = table->philosophers.philosopher;
	table->philosophers.r_fork = (table->philosophers.philosopher + 1) % table->philosophers.n_philosophers;
	pthread_mutex_lock(&table->fork[table->philosophers.l_fork]);
	pthread_mutex_lock(&table->fork[table->philosophers.r_fork]);
	printf("The philosopher %d is eating\n", table->philosophers.philosopher);
	table->time.time_last = get_current_time();
	usleep(table->time.time_to_eat);
	pthread_mutex_unlock(&table->fork[table->philosophers.l_fork]);
	pthread_mutex_unlock(&table->fork[table->philosophers.r_fork]);
}

void	*filosofofo(void *arg)
{
	int				i;
	t_table			*table;

	table = (t_table *)arg;
	table->philosophers.philosopher = table->philosophers.philosopher_index;
	table->philosophers.flag_dead = 0;
	table->philosophers.n_eats = 0;
	table->philosophers.state = 0;
	i = 0;
	while (i < table->philosophers.n_philosophers)
	{
		pthread_mutex_init(&table->fork[i], NULL);
		i++;
	}
	if (table->time.times_must_eat == 0)
	{
		while (1)
		{
			eat(table);
			check_if_died(table);
		}
	}
	else
	{
		i = 0;
		while (i < table->time.times_must_eat)
		{
			eat(table);
			check_if_died(table);
			i++;
		}
	}
	pthread_exit(NULL);
}
