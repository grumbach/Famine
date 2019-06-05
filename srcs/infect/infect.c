/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infect.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:20 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/05 01:13:08 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

inline void		infect(const char *file)
{
	size_t		filesize;

	if ((filesize = read_file(file)) == 0
	|| !alloc_clone(filesize)
	|| !packer(filesize)
	|| !write_clone_file())
		goto exit;

exit:
	free_clone();
	free_file();
}
