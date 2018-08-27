# extermination-tools

A collection of tools/snippets used to manipulate/extract/convert various resource/data files for the game Extermination/Mission: humanity with accompanying findings and ramblings.

## RES files

RES files are a very simple VFS filetype with no compression. The format includes a header which points to a list of file entries. In the demo there are two RES files, MENU.RES and PANEL.RES. Both are the same format. The header format as a C struct is the following:

	struct res_file {
	    uint8_t    magic[3];
	    uint8_t    unk_1;
	    uint32_t   file_index_offset;
	};

The file entry index format is:

	struct res_file_index {
	    uint8_t   	filename[23];
	    uint8_t     unk_1; // Always 1 in demo files, unsure of main
	    uint32_t    file_offset;
	    uint32_t    file_length;
	};

The 32 byte file index entries should be read until EOF.

## GFX files

The game seems to use two types of gfx formats included in the two res files, MENU.RES and PANEL.RES.

### MENU.RES

These .gfx files are the following format:

	struct gfx_file {
		uint8_t		magic[4]; // always "dupa"
		uint16_t 	width;
		uint16_t	height;
		uint8_t		pixels[width*height];
	}

The colour format seems to be either 565, 555 or something close. If converted to PNG using RGB565 as the format everything is dark and green.

### PANEL.RES

These .gfx files are different. Suspect multiple images packed in one format or the image sizes hardcoded in binary.


## Misc

All ints regardless of size are little endian

* .FLC - animated graphic, can be converted to gif for viewing purposes using [fli2gif](https://github.com/bovine/fli2gif/)
* .SAM - audio, likely 8-bit PCM or similar.
* .MP - proprietary map file
* .SCR/.INF/ - Some kind of compiled script/definition

## Tool Dependencies

* Python 3
* PIL-compatible library, recommend Pillow on windows.
