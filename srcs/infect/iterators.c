/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   iterators.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/10 08:11:33 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 12:44:23 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infect.h"
#include "errors.h"

bool	foreach_phdr(const struct safe_pointer info, const struct endians_pointer endians,
	 f_iter_callback callback, void *data)
{
	const Elf64_Ehdr	*elf64_hdr = safe(0, sizeof(Elf64_Ehdr));

	if (elf64_hdr == NULL) return errors(ERR_CORRUPT, '9','1');

	const Elf64_Off		phoff     = endians.endian_8(elf64_hdr->e_phoff);
	const Elf64_Half	phentsize = endians.endian_2(elf64_hdr->e_phentsize);
	Elf64_Half		phnum     = endians.endian_2(elf64_hdr->e_phnum);
	char			(*segments)[phnum][phentsize] = NULL;
	const size_t		array_size = phentsize * phnum;

	if (phentsize < sizeof(Elf64_Phdr)
	|| (array_size / phentsize != phnum)
	|| (!(segments = safe(phoff, array_size))))
		return errors(ERR_CORRUPT, '9','2');

	while (phnum--)
	{
		size_t	elf64_seg_hdr = (size_t)(*segments)[phnum];
		size_t	offset        = (elf64_seg_hdr - (size_t)elf64_hdr);

		if (!callback(info, endians, offset, data))
			return errors(ERR_THROW, '9','3');
	}
	return (true);
}

bool	foreach_shdr(const struct safe_pointer info, const struct endians_pointer endians,
	 f_iter_callback callback, void *data)
{
	const Elf64_Ehdr	*elf64_hdr = safe(0, sizeof(Elf64_Ehdr));

	if (elf64_hdr == NULL) return errors(ERR_CORRUPT, '9','4');

	const Elf64_Off		shoff     = endians.endian_8(elf64_hdr->e_shoff);
	const Elf64_Half	shentsize = endians.endian_2(elf64_hdr->e_shentsize);
	Elf64_Half		shnum     = endians.endian_2(elf64_hdr->e_shnum);
	char			(*sections)[shnum][shentsize] = NULL;
	const size_t		array_size = shentsize * shnum;

	if (shentsize < sizeof(Elf64_Shdr)
	|| (array_size / shentsize != shnum)
	|| (!(sections = safe(shoff, array_size))))
		return errors(ERR_CORRUPT, '9','5');

	while (shnum--)
	{
		size_t	elf64_section_hdr = (size_t)(*sections)[shnum];
		size_t	offset = (elf64_section_hdr - (size_t)elf64_hdr);

		if (!callback(info, endians, offset, data))
			return errors(ERR_THROW, '9','6');
	}
	return (true);
}
