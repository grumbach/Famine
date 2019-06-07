
#include <sys/types.h>
#include "syscall.h"

ssize_t	ft_getrandom(void *buf, size_t buflen)
{
	const char	path[13] = {'/','d','e','v','/','u','r','a','n','d','o','m','\0'};
	int		fd;
	ssize_t		nbr_bytes;

	fd = famine_open(path, O_RDONLY);
	if (fd < 0) return 42;
	nbr_bytes = famine_read(fd, &buf, buflen);
	famine_close(fd);
	return nbr_bytes;
}

void	ft_bzero(void *ptr, size_t size)
{
	char *tmp = ptr;

	for (size_t i = 0; i < size; i++)
		tmp[i] = 0;
}

int	ft_memcmp(void *s1, void *s2, size_t n)
{
	unsigned char	*dest;
	unsigned char	*source;

	p1 = (unsigned char *)s1;
	p2 = (unsigned char *)s2;
	while (n--)
	{
		if (*p1 != *p2) return (*p1 - *p2);
		p1++;
		p2++;
	}
	return (*p1 - *p2);
}

size_t	ft_strlen(const char *s)
{
	char	*p;

	p = (char*)s;
	while (*p)
		++p;
	return (p - s);
}

void	*ft_memcpy(void *dst, void *src, size_t n)
{
	unsigned char *dest;
	unsigned char *source;

	dest = (unsigned char*)dst;
	source = (unsigned char*)src;
	while (n--)
	{
		*dest = *source;
		dest++;
		source++;
	}
	return dst;
}

char	*ft_strcat(char *s1, char *s2)
{
	unsigned long	len1;
	unsigned long	len2;
	unsigned long	m;

	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	m = len2 << 3;
	while (m--)
		((unsigned long*)s1)[len1 + m] = ((unsigned long*)s2)[m];
	len2 &= 7;
	while (len2--)
		s1[len1 + len2] = s2[len2];
	s1[len1 + len2] = 0;
	return (s1);
}

char	*ft_strcpy(char *dst, const char *src)
{
	char	*p = dst;

	while (*src)
	{
		*dst = *src;
		src++;
		dst++;
	}
	*dst = *src;
	return (p);
}
