/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clone_safe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/05/12 03:15:48 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/04 05:53:26 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
** clone_safe()
** returns a safe pointer in the clone
** returns NULL if requested memory is out of range
*/

__warn_unused_result
inline void		*clone_safe(const size_t offset, const size_t size, \
				const struct safe_pointer info)
{
	if (offset + size > info.filesize || offset + size < offset)
		return (NULL);
	return (info.ptr + offset);
}

__warn_unused_result
inline t_safe_accessor	alloc_clone(const size_t original_filesize)
{
	t_safe_accessor = ;
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
