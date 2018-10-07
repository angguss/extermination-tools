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
		uint8_t		unk[4]; // if "dupa" then not this format
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

## Cheats/Console

Console commands in the main game binary are obfuscated, disassembling the exe reveals the following strings:

	const char *console_commands[] = {
		"1.5Z0?).Z85=3?7E",
		"-?()0;Z>?75",
		"%7;*",
		"*;/)?",
		"%/43.",
		"%/43.E",
		"%/43.Q",
		"%/43.W",
		"%8/36>34=",
		"%8/36>34=E",
		"%8/36>34=Q",
		"%8/36>34=W",
		"%(?*;3(",
		"%>?).(5#",
		"%)-3;.65Z*(;->#",
		"%)-3;.65Z*(;->#Z*(50?1.#",
		"%)-3;.65Z*(;->#Z8/>#413",
		"%)-3;.65Z*(;->#Z*6;4?.#",
		"%4/96?;(Z8578",
		";3",
		";3Z5<<",
		";3Z54",
		"%.;*",
		"%)*#",
		"%4?-Z*5-?(%",
		"%.?926;4>Z(/6? Z",
		"%8;0;>?(;Z*;(.#",
		"%(?)5/(9?)",
		"LNJ\\\"NBJ",
		"BJJ\\\"LJJ",
		"KJHN\\\"MLB",
		"9>Z54",
		"9>Z5<<",
		")5/4>Z54",
		")5/4>Z5<<",
		"2;(;Z13(3",
		"%4?.>?6;#",
		"%4?.>?6;#Z/*",
		"%4?.>?6;#Z>5-4",
		"%4?.>?6;#Z)#49",
		")56;(Z)#).?7",
		"%7)=",
		"%7)=Q",
		"%7)=W",
		"%7)=J"
	}

The values can be recovered by applying XOR 122 against each character, revealing the following codes:

	KTO JEST BOGIEM?
	WERSJA DEMO
	_MAP
	PAUSE
	_UNIT
	_UNIT?
	_UNIT+
	_UNIT-
	_BUILDING
	_BUILDING?
	_BUILDING+
	_BUILDING-
	_REPAIR
	_DESTROY
	_SWIATLO PRAWDY
	_SWIATLO PRAWDY PROJEKTY
	_SWIATLO PRAWDY BUDYNKI
	_SWIATLO PRAWDY PLANETY
	_NUCLEAR BOMB
	AI
	AI OFF
	AI ON
	_TAP
	_SPY
	_NEW POWER_
	_TECHLAND RULEZ 
	_BAJADERA PARTY
	_RESOURCES
	640&X480
	800&X600
	1024&X768
	CD ON
	CD OFF
	SOUND ON
	SOUND OFF
	HARA KIRI
	_NETDELAY
	_NETDELAY UP
	_NETDELAY DOWN
	_NETDELAY SYNC
	SOLAR SYSTEM
	_MSG
	_MSG+
	_MSG-
	_MSG0

### Python code to deobfus

	commands = []
	deobfus_commands = []
	tmp_cmd = ""
	for i in range(0, len(commands)):
		for j in range(0, len(commands[i])):
			tmp_cmd += chr(ord(commands[i][j]) ^ 122)
		deobfus_commands.append(tmp_cmd)
		tmp_cmd = ""

## Tool Dependencies

* Python 3
* PIL-compatible library, recommend Pillow on windows.
