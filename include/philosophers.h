/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:09:58 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/06 20:03:54 by rofuente         ###   ########.fr       */
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
typedef struct table	t_table;

typedef struct philosophers
{
	int				philosopher;
	int				times_eat;
	uint64_t		last;
	pthread_mutex_t	*l_fork;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*msg;
	t_table			*table;
}	t_philo;

typedef struct table
{
	int				n_philosophers;
	int				times_must_eat;
	int				flag_dead;
	int				finish;
	int				print;
	uint64_t		time_to_eat;
	uint64_t		time_to_dead;
	uint64_t		time_to_sleep;
	uint64_t		start;
	t_philo			**philosophers;
	pthread_t		*philo_thread;
	pthread_mutex_t	*fork;
	pthread_mutex_t	*msg;
	pthread_mutex_t	*start_m;
	pthread_mutex_t	*times_eat_m;
	pthread_mutex_t	*died_m;
	pthread_mutex_t	*end_m;
}	t_table;

/* ---------- FUNCIONES ---------- */
/* PHILOSOPHERS.C */
void	free_table(t_table *table);

/* FT_UTILS.C */
int			check_args(int argc, char **argv);
int			take_args(t_table *table, char **argv);
int			init(t_table *table);
int			init_mutex(t_table *table, int x);
int			init_philo(t_table *table);
/* FT_ATOI.C */
int			ft_atoi(const char *str);

/* FILOSOFOFO.C */
void		*filosofofo(void *arg);
void		ft_usleep(uint64_t time);

/* FT_ERROR.C */
void		print_msg(t_philo *philo, char *s);
void		print_dead(t_philo *philo);
uint64_t	get_current_time(void);
int			ft_error(char *str);
void		free_mtx(t_table *table, int i);

/* FINISH.C */
int	finish_meal(t_table *table, int argc);

#endif
