/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   original_safe.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/22 16:40:47 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/04 05:44:27 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** original_safe()
** returns a safe read-only pointer to the original file
** returns NULL if requested memory is out of range
*/

__warn_unused_result
inline void		*original_safe(const size_t offset, const size_t size, \
				const struct safe_pointer info)
{
	if (offset + size > info.filesize || offset + size < offset)
		return (NULL);
	return (info.ptr + offset);
}

__warn_unused_result
inline t_safe_accessor	read_file(const char *filename)
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

	t_safe_accessor	accessor =
	{
		.access = &original_safe,
		.info   =
		{
			.ptr      = ptr,
			.filesize = buf.st_size
		}
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
