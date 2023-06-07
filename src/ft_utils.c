/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 15:55:53 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/07 16:52:28 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	free_table(t_table *table)
{
	int	i;

	i = -1;
	while (++i <= table->n_philo)
		free(table->philosophers[i]);
	free(table->fork);
	free(table->msg);
	free(table->start_m);
	free(table->times_eat_m);
	free(table->died_m);
	free(table->end_m);
	free(table->philo_thread);
}

void	free_mtx(t_table *table, int i)
{
	int	j;

	j = 0;
	while (j <= i)
	{
		free(table->philosophers[j]);
		j++;
	}
	free(table->philosophers);
}

uint64_t	get_current_time(void)
{
	struct timeval	tv;
	uint64_t		millisec;

	gettimeofday(&tv, NULL);
	millisec = (uint64_t)tv.tv_sec * 1000
		+ (uint64_t)tv.tv_usec / 1000;
	return (millisec);
}

void	print_msg_dead(t_philo *philo)
{
	uint64_t	time;

	time = get_current_time() - philo->table->start;
	pthread_mutex_lock(philo->msg);
	if (philo->table->print)
	{
		printf("%llu %d", time, philo->id);
		printf("\033[31;3m 💀 DIED 💀 \033[0m\n");
	}
	pthread_mutex_unlock(philo->msg);
}

void	print_msg(t_philo *philo, char *s)
{
	uint64_t	time;

	time = get_current_time() - philo->table->start;
	pthread_mutex_lock(philo->msg);
	if (philo->table->print)
		printf("%llu %d %s", time, philo->id, s);
	pthread_mutex_unlock(philo->msg);
}
