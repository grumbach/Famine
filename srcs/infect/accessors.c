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
	int		fd = open(filename, O_RDONLY);

	if (fd < 0)
		return (errors(ERR_SYS, "open failed"));
	if (fstat(fd, &buf) < 0)
		return (errors(ERR_SYS, "fstat failed"));
	if (buf.st_mode & S_IFDIR)
		return (errors(ERR_USAGE, "can't parse directories"));
	if ((ptr = mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		return (errors(ERR_SYS, "mmap failed"));
	if (close(fd))
		return (errors(ERR_SYS, "close failed"));

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
		if (munmap(info.ptr, info.filesize))
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
	struct safe_pointer = ;
	accessor->filesize = original_filesize;
	accessor->ptr = malloc(accessor->filesize + MAX_EST_SIZE);

	if (accessor->ptr == NULL)
		return (errors(ERR_SYS, "while allocating clone"));

	return (true);
}

void		free_clone(void)
{
	free(accessor->ptr);
}

__warn_unused_result
bool		write_clone_file(void)
{
	int	fd = open(OUTPUT_FILENAME, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);

	if (fd == -1)
		return errors(ERR_SYS, "failed creating file " OUTPUT_FILENAME);

	if (write(fd, accessor->ptr, accessor->filesize) == -1)
	{
		close(fd);
		return errors(ERR_SYS, "failed writing to " OUTPUT_FILENAME);
	}

	close(fd);
	return (true);
}
