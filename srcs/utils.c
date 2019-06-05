
#include <sys/types.h>

int	ft_memcmp(void *s1, void *s2, size_t n)
{
	unsigned char	*p1;
	unsigned char	*p2;

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
	unsigned long	*d1;
	unsigned long	*s1;
	unsigned char	*d2;
	unsigned char	*s2;
	size_t		m;

	m = n >> 3;
	d2 = dst + (m << 3);
	s2 = src + (m << 3);
	d1 = (unsigned long*)dst;
	s1 = (unsigned long*)src;
	n &= 7;
	while (m--)
		*(d1++) = *(s1++);
	while (n--)
		*(d2++) = *(s2++);
	return (dst);
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
