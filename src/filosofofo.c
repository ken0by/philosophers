/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   filosofofo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/01 13:17:43 by rofuente          #+#    #+#             */
/*   Updated: 2023/06/01 14:11:30 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	*filosofofo(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	table->philosophers.philosopher = table->philosophers.philosopher_index;
	table->philosophers.flag_dead = 0;
	table->philosophers.n_eats = 0;
	table->philosophers.state = 0;
	table->philosophers.l_fork = 0;
	table->philosophers.r_fork = 0;
	if (table->time.times_must_eat == 0)
	{
		check_if_eat;
	}
	pthread_exit(NULL);
}
