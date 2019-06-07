/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   endian.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/01 23:54:56 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 05:31:02 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infect.h"
#include <stdint.h>

inline uint16_t	endian_2_noswap(uint16_t n)
{
	return (n);
}

inline uint16_t	endian_2_swap(uint16_t n)
{
	return ((n >> 8) | (n << 8));
}

inline uint32_t	endian_4_noswap(uint32_t n)
{
	return (n);
}

inline uint32_t	endian_4_swap(uint32_t n)
{
	return ((n >> 24) | ((n & 0xff0000) >> 8) | \
		((n & 0xff00) << 8) | (n << 24));
}

inline uint64_t	endian_8_noswap(uint64_t n)
{
	return (n);
}

inline uint64_t	endian_8_swap(uint64_t n)
{
	return ((n & 0xff00000000000000) >> 56 \
		| (n & 0x00ff000000000000) >> 40 \
		| (n & 0x0000ff0000000000) >> 24 \
		| (n & 0x000000ff00000000) >> 8 \
		| (n & 0x00000000ff000000) << 8 \
		| (n & 0x0000000000ff0000) << 24 \
		| (n & 0x000000000000ff00) << 40 \
		| (n & 0x00000000000000ff) << 56);
}

inline void	set_endian(struct endians_pointer *endians, bool big_mode)
{
	uint16_t	(*mode_2[])(uint16_t) =
	{
		endian_2_noswap,
		endian_2_swap
	};
	uint32_t	(*mode_4[])(uint32_t) =
	{
		endian_4_noswap,
		endian_4_swap
	};
	uint64_t	(*mode_8[])(uint64_t) =
	{
		endian_8_noswap,
		endian_8_swap
	};

	endians->endian_2 = mode_2[big_mode];
	endians->endian_4 = mode_4[big_mode];
	endians->endian_8 = mode_8[big_mode];
}
