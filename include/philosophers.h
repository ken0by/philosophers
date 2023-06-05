/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:09:58 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/05 18:18:34 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

/* ---------- LIBRERIAS ---------- */
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

# define EAT 1
# define SLEEP 2
# define DEAD 3

/* ---------- STRUCTS ---------- */
typedef struct philosophers
{
	int	philosopher;
	int	philosopher_index;
	int	n_philosophers;
	int	n_forks;
	int	flag_dead;
	int	n_eats;
	int	l_fork;
	int	r_fork;
	int	state;
}	t_philo;

typedef struct times
{
	uint64_t	time_to_eat;
	uint64_t	time_to_dead;
	uint64_t	time_to_sleep;
	int		times_must_eat;
	uint64_t	time_last;
}	t_time;

typedef struct table
{
	t_philo	philosophers;
	t_time	time;
	pthread_mutex_t	*fork;
}	t_table;

/* ---------- FUNCIONES ---------- */

/* FT_UTILS.C */
int	ft_atoi(const char *str);

/* FILOSOFOFO.C */
void	*filosofofo(void *arg);

/* FT_ERROR.C */
void	ft_error(char *str);

#endif
