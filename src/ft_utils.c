/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 18:50:38 by rofuente          #+#    #+#             */
/*   Updated: 2023/05/31 18:51:16 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	ft_atoi(const char *str)
{
	int	nb;
	int	x;
	int	k;

	nb = 0;
	x = 0;
	k = 1;
	while ((str[x] == ' ') || (str[x] == '\f') || (str[x] == '\n')
		|| (str[x] == '\r') || (str[x] == '\t') || (str[x] == '\v'))
		x++;
	if (str[x] == '+' || str[x] == '-')
	{
		if (str[x] == '-')
			k = k * (-1);
		x++;
	}
	while (str[x] >= '0' && str[x] <= '9')
	{
		nb = nb * 10 + str[x] - 48;
		x++;
	}
	return (nb * k);
}
