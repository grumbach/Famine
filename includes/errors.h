/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/07 01:53:00 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 12:09:22 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

# include <string.h>
# include "utils.h"

enum
{
	ERR_SYS,                // syscall failure
	ERR_THROW,              // throw error form above function
	ERR_USAGE,              // bad usage
	ERR_CORRUPT,            // corrupt file
	ERR_SIZE,               // number of ERRs above, always last
};

# define DEBUG		1

# ifdef DEBUG
#  define errors(err, fmt, ...)	({        \
	dprintf(2, fmt);                  \
	dprintf(2, (char[]){'\n', '\0'}); \
	(false);                          \
})
# else
#  define errors(...)		false
# endif

#endif
