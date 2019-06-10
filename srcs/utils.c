#include <sys/types.h>
#include <string.h>
#include "syscall.h"

ssize_t	ft_getrandom(void *buf, size_t buflen)
{
	char		path[13] = {'/','d','e','v','/','u','r','a','n','d','o','m','\0'};
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

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	size_t			i;
	const unsigned char	*ch_s1;
	const unsigned char	*ch_s2;

	ch_s1 = (const unsigned char*)s1;
	ch_s2 = (const unsigned char*)s2;
	i = 0;
	while (i < n)
	{
		if (ch_s1[i] != ch_s2[i])
			return (ch_s1[i] - ch_s2[i]);
		i++;
	}
	return (0);
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

char	*ft_strcpy(char *dst, const char *src)
{
	int i = 0;

	while(src[i])
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return dst;
}

char	*ft_strcat(char *dest, char *source)
{
	int len = ft_strlen(dest);

	ft_strcpy(dest + len, source);
	return dest;
}

int	dprintf(int fd, char *fmt, ...)
{
	return famine_write(fd, fmt, ft_strlen(fmt));
}

char	*ft_strstr(const char *s1, const char *s2)
{
	int i = 0;
	int j = 0;

	if (s1 == NULL)
		return (NULL);
	if (s2 == NULL)
		return ((char *)s1);
	while (s1[i])
	{
		j = 0;
		while (s1[i] == s2[j] && (s1[i] || s2[j]))
		{
			i++;
			j++;
		}
		if (s2[j])
			return (char *)s1 + i - j;
		i = i - j + 1;
	}
	return NULL;
}
