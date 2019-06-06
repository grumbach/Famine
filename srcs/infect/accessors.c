/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 06:32:25 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 06:38:36 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "accessors.h"

/*
** safe()
** returns a safe pointer
** returns NULL if requested memory is out of range
*/

#define OUTPUT_FILENAME	"packed_file"

__warn_unused_result
inline void		*safe_accessor(const size_t offset, const size_t size, \
				const struct safe_pointer info)
{
	if (offset + size > info.filesize || offset + size < offset)
		return (NULL);
	return (info.ptr + offset);
}

__warn_unused_result
inline struct safe_pointer	read_file(const char *filename)
{
	void		*ptr;
	struct stat	buf;
	int		fd = famine_open(filename, O_RDONLY);

	if ((fd < 0)
	|| (fstat(fd, &buf) < 0)
	|| (buf.st_mode & S_IFDIR)
	|| ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
	|| (close(fd)))
		ptr = NULL;

	struct safe_pointer	accessor =
	{
		.ptr      = ptr,
		.filesize = buf.st_size
	};
	return (accessor);
}

inline bool		free_file(struct safe_pointer info)
{
	if (info.ptr)
	{
		if (famine_munmap(info.ptr, info.filesize))
			return errors(ERR_SYS, "munmap failed");
	}
	return (true);
}


__warn_unused_result
inline void		*clone_safe(const size_t offset, const size_t size, \
				const struct safe_pointer info)
{
	if (offset + size > info.filesize || offset + size < offset)
		return (NULL);
	return (info.ptr + offset);
}

__warn_unused_result
inline struct safe_pointer	alloc_clone(const size_t original_filesize)
{
	struct safe_pointer	accessor;
	accessor.filesize = original_filesize;
	accessor.ptr = malloc(accessor.filesize); // TODO: add MAX_EST_SIZE

	return accessor;
}

void		free_clone(const struct safe_pointer accessor)
{
	free(accessor.ptr);
}

__warn_unused_result
bool		write_clone_file(const struct safe_pointer accessor)
{
	int	fd = famine_open(OUTPUT_FILENAME, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

	if (fd == -1)
		return (false);

	if (famine_write(fd, accessor->ptr, accessor->filesize) == -1)
	{
		famine_close(fd);
		return (false);
	}

	famine_close(fd);
	return (true);
}
