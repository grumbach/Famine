/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 04:47:10 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/04 05:48:11 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ACCESSORS_H
# define ACCESSORS_H

/*
** Bright facade for safe accessors
*/

typedef	void	*(*f_safe_accessor)(const size_t offset, const size_t size, \
			t_safe_accessor accessor);

static struct			safe_pointer
{
	void	*ptr;
	size_t	filesize;
};

/*
** Dark side of the safe accessors...
*/

typedef struct
{
	f_safe_accessor		access;
	struct safe_pointer	info;
}				t_safe_accessor;

# define safe(ptr, size)	(safe->access(ptr, size, safe->info))

/*
** Functions
*/

void		*original_safe(const size_t offset, const size_t size, \
			const struct safe_pointer info);
t_safe_accessor	read_file(const char *filename);
bool		free_file(const struct safe_pointer info);

void		*clone_safe(const size_t offset, const size_t size, \
			const struct safe_pointer info);
t_safe_accessor	alloc_clone(const size_t original_filesize);
bool		write_clone_file(const struct safe_pointer info);
void		free_clone(const struct safe_pointer info);

#endif
