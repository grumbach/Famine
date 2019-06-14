/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   famine.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/04 03:38:38 by agrumbac          #+#    #+#             */
/*   Updated: 2019/06/12 13:50:04 by spolowy          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FAMINE_H
# define FAMINE_H

# include <limits.h>
# include <linux/elf.h>
# include <stdbool.h>
# include <stdint.h>
# include <stddef.h>
# include <sys/types.h>

struct client_info
{
	uint32_t	key[4];
	uint64_t	relative_pt_load_address;
	uint64_t	pt_load_size;
	uint64_t	relative_virus_address;
	uint64_t	relative_entry_address;
	uint64_t	virus_size;
	uint64_t	signature[8];
}__attribute__((packed));

/*
** famine
*/

bool		detect_spy(void);

/*
** virus
*/

void		famine_entry(void);
void		virus(void);
void		infect_files_in(const char *path);

/*
** encryption
*/

void	encrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);
void	decrypt(uint num_rounds, char *data, uint32_t const key[4], size_t size);

#endif
