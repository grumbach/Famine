/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   famine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:38:38 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/07 07:28:17 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAMINE_H
# define FAMINE_H

# include <limits.h>
# include <linux/elf.h>
# include <stdbool.h>

struct client_info
{
	uint32_t	key[4];
	uint64_t	relative_pt_load_address;
	uint64_t	pt_load_size;
	uint64_t	relative_virus_address;
	uint64_t	relative_entry_address;
	uint64_t	virus_size;
}__attribute__((packed));

/*
** famine
*/

void		dear_client(const struct client_info *client);
bool		detect_spy(void);
void		unpacker(void);

/*
** virus
*/

void		virus(void);
void		infect_files_in(const char *path);

#endif
