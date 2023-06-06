/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filosofofo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 13:17:43 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/06 20:05:25 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	ft_usleep(uint64_t time)
{
	uint64_t	t;

	t = time + get_current_time();
	while (get_current_time() < t)
		usleep(100);
}

static int	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	print_msg(philo, "has take a fork");
	pthread_mutex_lock(philo->r_fork);
	print_msg(philo, "has take a fork");
	pthread_mutex_lock(philo->table->times_eat_m);
	philo->times_eat++;
	philo->last = get_current_time();
	pthread_mutex_unlock(philo->table->times_eat_m);
	print_msg(philo, "is eating");
	ft_usleep(philo->table->time_to_eat);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	return (1);
}

static int	sleep_think(t_philo *philo)
{
	print_msg(philo, "is sleeping");
	ft_usleep(philo->table->time_to_sleep);
	print_msg(philo, "is thinking");
	return (1);
}

static int	actions(t_philo *philo)
{
	if (!eat(philo))
		return (0);
	if (!sleep_think(philo))
		return (0);
	return (1);
}

void	*filosofofo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	pthread_mutex_lock(philo->table->start_m);
	pthread_mutex_unlock(philo->table->start_m);
	if (philo->philosopher % 2 == 0)
		usleep(100);
	pthread_mutex_lock(philo->table->died_m);
	pthread_mutex_lock(philo->table->end_m);
	while (!philo->table->flag_dead && philo->table->finish)
	{
		pthread_mutex_unlock(philo->table->died_m);
		pthread_mutex_unlock(philo->table->end_m);
		if (!actions(philo))
			break ;
		pthread_mutex_lock(philo->table->died_m);
		pthread_mutex_lock(philo->table->end_m);
	}
	pthread_mutex_unlock(philo->table->died_m);
	pthread_mutex_unlock(philo->table->end_m);
	return (NULL);
}
