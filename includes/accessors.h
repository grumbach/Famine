/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 04:47:10 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 06:47:12 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACCESSORS_H
# define ACCESSORS_H

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "compiler_utils.h"

/*
** Bright facade for safe accessors
*/

struct	safe_pointer
{
	void	*ptr;
	size_t	filesize;
};

/*
** Dark side of the safe accessors...
*/

# define safe(ptr, size)	(safe_accessor(ptr, size, info))

/*
** Functions
*/

void			*safe_accessor(const size_t offset, const size_t size, \
				const struct safe_pointer info);

/*
** original
*/

struct safe_pointer	read_file(const char *filename);
bool			free_file(const struct safe_pointer info);

/*
** clone
*/

struct safe_pointer	alloc_clone(const size_t original_filesize);
bool			write_clone_file(const struct safe_pointer info);
void			free_clone(const struct safe_pointer accessor);

#endif
