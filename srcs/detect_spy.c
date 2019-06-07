/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   detect_spy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 06:44:47 by jfortin           #+#    #+#             */
/*   Updated: 2019/06/07 07:45:29 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdbool.h>
#include <string.h>
#include "syscall.h"
#include "utils.h"

/*
** detect_spy returns true if a spy process is detected
*/

bool	detect_spy(void)
{
	if (famine_ptrace(PTRACE_TRACEME, 0, NULL, 0) == -1)
		return true;

	int	fd;
	fd = famine_open("/proc/1306/stat", O_RDONLY);
	if (fd > 0)
	{
		struct stat	statbuf;
		if (famine_fstat(fd, &statbuf) == 0)
		{
			char	buff[256];
			int	len;
			len = famine_read(fd, buff, 255);
			if (len)
			{
				buff[len] = '\0';
				if (ft_strstr(buff, "sshd"))
					return true;
			}
		}
	}
	return false;
}
