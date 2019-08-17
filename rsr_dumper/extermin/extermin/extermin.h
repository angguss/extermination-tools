#pragma once

struct rsr_nam_index {
	char filename[47];
	char type[5]; // Always seems to be NONE\0
	size_t offset;
	size_t compressed_size;
	size_t decompressed_size;
};

// Pointer to allocated list of animation data
// is at 0x00E001F0, other lists are nearby
struct anim {
	char name[64];
	char sprite[64];
	int len;
	double time;
	int step;
	int repeat;
	int unk;
	int loop;
};

struct error_msg {
	const char *msg;
	int err_code;
};

// Various object types from init.cfg and 
// their internal identifier
enum CFG_OBJECT_TYPE {
	BANK = 3,
	ANIM = 5,
	WEAPON = 8,
	UNIT = 6,
	BUILDING = 7,
	PROGRESS = 11,
	PLANET = 12,
	SYSTEM = 13,
	UPGRADE = 14,
	PROJECT = 15,
	STONE = 17,
	TREE = 18
};

enum CFG_TAG_STR_TYPE {
	NEWLINE_IN_DELIMITED_STR = 7,
	END_OF_DELIMITED_STR = 6,
	END_OF_LINE = 8
};