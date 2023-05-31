/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rofuente <rofuente@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/31 16:49:15 by rofuente          #+#    #+#             */
/*   Updated: 2023/05/31 17:00:41 by rofuente         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	main(int argc, char **argv)
{
	if (argc < 5 || argc > 6)
	{
		printf("Invalid arguments\n");
		return (0);
	}
	else
		printf("%s\n", argv[0]);
}
