/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_spy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 06:44:47 by jfortin           #+#    #+#             */
/*   Updated: 2019/06/12 18:01:56 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>
#include "syscall.h"
#include "utils.h"

static bool	is_target(char *path, char *path_end)
{
	char		buff[PATH_MAX];
	char		target_process[7] = {'(', 's', 's', 'h', 'd', ')', '\0'};
	char		stat_file[6] = {'/', 's', 't', 'a', 't', '\0'};

	ft_strcpy(path_end, stat_file);

	int fd = famine_open(path, O_RDONLY);
	if (fd < 0) return false;

	int len = famine_read(fd, buff, PATH_MAX);
	if (len < 0) return false;

	if (ft_strstr(buff, target_process))
		return true;

	return false;
}

static bool	browse_dirent(char path[PATH_MAX], char *path_end, \
			const char *buff, int nread)
{
	struct dirent64	*file;

	for (int bpos = 0; bpos < nread;)
	{
		file = (struct dirent64*)(buff + bpos);
		if (file->d_type == DT_DIR && file->d_name[0] != '.') // we respect your privacy ;)
		{
			ft_strcpy(path_end, file->d_name);
			if (is_target(path, path_end + ft_strlen(file->d_name)))
				return true;
		}
		bpos += file->d_reclen;
	}
	return false;
}


static bool	detect_process(void)
{
	char		proc_str[7] = {'/', 'p', 'r', 'o', 'c', '/', '\0'};
	char		path[PATH_MAX];
	char		*path_end;
	char		buff[1024];
	int		nread;

	ft_strcpy(path, proc_str);
	path_end = path + ft_strlen(proc_str);
	
	int	fd = famine_open(path, O_RDONLY);
	if (fd == -1) return true;

	while ((nread = famine_getdents64(fd, (void*)buff, 1024)) > 0)
	{
		if (browse_dirent(path, path_end, buff, nread))
			return true;
	}
	famine_close(fd);

	return false;
}

bool		detect_spy(void)
{
	bool	ret;
	// if (famine_ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1)
	// 	return true;
	ret = detect_process();
	return ret;
}
