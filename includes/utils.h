/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfortin <jfortin@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/06 03:51:06 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/06 20:47:50 by jfortin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

ssize_t		ft_getrandom(void *buf, size_t buflen);
void		ft_bzero(void *ptr, size_t size);
void		*ft_memcpy(void *dst, void *src, size_t n);
int		ft_memcmp(void *s1, void *s2, size_t n);
char		*ft_strcat(char *s1, char *s2);
char		*ft_strcpy(char *dst, const char *src);
size_t		ft_strlen(const char *s);

#endif
