/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_error.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/05 13:15:45 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/06 19:00:44 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	print_dead(t_philo *philo)
{
	uint64_t	time;

	time = get_current_time() - philo->table->start;
	pthread_mutex_lock(philo->msg);
	if (philo->table->print)
	{
		printf("%llu %d", time, philo->philosopher);
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
		printf("%llu %d %s", time, philo->philosopher, s);
	pthread_mutex_unlock(philo->msg);
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

int	ft_error(char *str)
{
	printf("%s\n", str);
	return (0);
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
