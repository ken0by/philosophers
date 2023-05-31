/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:49:15 by rofuente          #+#    #+#             */
/*   Updated: 2023/05/31 19:17:16 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	init_hilos(t_table **table)
{
	pthread_t	pid;

	if (pthread_create(&pid, NULL, &funcion, table))
		return ;
}

void	init(int argc, char **argv)
{
	t_table	*table;

	table = malloc(sizeof(t_table));
	table->philosophers.n_philosophers = ft_atoi(argv[1]);
	table->philosophers.n_forks = ft_atoi(argv[1]);
	table->time.time_to_dead = ft_atoi(argv[3]);
	table->time.time_to_eat = ft_atoi(argv[3]);
	table->time.time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		table->time.times_must_eat = ft_atoi(argv[5]);
	table->philosophers.philosopher = 1;
	table->philosophers.flag_dead = 0;
	table->philosophers.n_eats = 0;
	table->philosophers.state = 0;
	if (table->philosophers.n_philosophers == 1)
	{
		printf("Philosopher died because he can't eat\n");
		free (table);
		return ;
	}
	init_hilos(&table);
}

void ft_leaks()
{
	system("leaks philo");
}

int	main(int argc, char **argv)
{
	//atexit(ft_leaks);
	if (argc < 5 || argc > 6)
	{
		printf("Invalid arguments\n");
		return (0);
	}
	else
		init(argc, argv);
	return (0);
}
