/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 04:47:10 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 04:46:42 by agrumbac         ###   ########.fr       */
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
** any accessor
*/

void	*safe_accessor(const size_t offset, const size_t size, \
		const struct safe_pointer info);
bool	free_accessor(struct safe_pointer *info);

/*
** original
*/

bool	original_accessor(struct safe_pointer *accessor, const char *filename);

/*
** clone
*/

bool	clone_accessor(struct safe_pointer *accessor, const size_t original_filesize);
bool	write_clone_file(const struct safe_pointer accessor, \
		const char *filename);

#endif
