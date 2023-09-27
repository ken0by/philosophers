/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_meal.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/07 16:27:52 by rofuente          #+#    #+#             */
/*   Updated: 2023/09/27 17:00:51 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	ft_print(philo, "has taken a fork 🍴");
	pthread_mutex_lock(philo->r_fork);
	ft_print(philo, "has taken a fork 🍴");
	if (is_dead(philo))
		return (0);
	philo->last = get_current_time();
	ft_print(philo, "is eating 🍝");
	ft_usleep(philo->table->time_to_eat);
	pthread_mutex_lock(philo->table->times_eat_m);
	philo->times_eat++;
	pthread_mutex_unlock(philo->table->times_eat_m);
	pthread_mutex_unlock(philo->l_fork);
	pthread_mutex_unlock(philo->r_fork);
	return (1);
}

static int	sleep_think(t_philo *philo)
{
	ft_print(philo, "is sleeping 🛌");
	ft_usleep(philo->table->time_to_sleep);
	ft_print(philo, "is thinking 🤔");
	return (1);
}

static int	actions(t_philo *philo)
{
	if (is_dead(philo) || !eat(philo) || !sleep_think(philo))
		return (0);
	if (is_dead(philo))
		return (0);
	return (1);
}

static void	*filosofofo(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if ((philo->id % 2) == 0)
		ft_usleep(philo->table->time_to_eat);
	pthread_mutex_lock(philo->table->died_m);
	pthread_mutex_lock(philo->table->end_m);
	while ((philo->table->flag_dead == 0) && (philo->table->finish == 0))
	{
		pthread_mutex_unlock(philo->table->died_m);
		pthread_mutex_unlock(philo->table->end_m);
		if (actions(philo) == 0)
			break ;
		pthread_mutex_lock(philo->table->died_m);
		pthread_mutex_lock(philo->table->end_m);
	}
	pthread_mutex_unlock(philo->table->died_m);
	pthread_mutex_unlock(philo->table->end_m);
	return (NULL);
}

int	start_meal(t_table *table)
{
	int			i;
	t_philo		*philo_s;
	pthread_t	*philo_t;

	printf("");
	table->start_m = malloc(sizeof(pthread_mutex_t));
	if (!table->start_m)
		return (0);
	if (pthread_mutex_init(table->start_m, NULL) != 0)
		return (free(table->start_m), 0);
	pthread_mutex_lock(table->start_m);
	table->start = get_current_time();
	pthread_mutex_unlock(table->start_m);
	i = -1;
	while (++i < table->n_philo)
	{
		philo_t = table->philo_thread + i;
		philo_s = table->philosophers[i];
		if (pthread_create(philo_t, NULL, filosofofo, (void *)philo_s) != 0)
			return (printf("Failed to create a thread!\n"), 0);
	}
	return (1);
}
