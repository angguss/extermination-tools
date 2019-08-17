# extermination-tools

A collection of tools/snippets used to manipulate/extract/convert various resource/data files for the game Extermination/Mission: humanity with accompanying findings and ramblings.


## Full Game

The full version uses RSR files unlike the RES files packaged with the demo. An RSR file requires an accompanying NAM file which acts as the index for the data in the RSR file. The NAM index file is a similar format to the RES files seen in the demo.

	struct nam_file_index {
		uint8_t		filename[47];
		uint8_t		type[5];
		uint32_t	offset;
		uint32_t 	compressed_size;
		uint32_t	decompressed_size
	}

Once the index file has been read, the compressed contents of each packed entry from the RSR file can be read by reading from offset -> offset + compressed_size.

### Decompression

Reversing the compression proved a challenge, the game uses a variant of LZW and re-implementation has not been achieved yet. The files can however be decompressed using the original code & DLL injection

* Compile the project in ./rsr_dumper/extermin
* Make a copy of the original extermin.exe
* Patch the exe to add an import to your compiled extermin.dll & replace the body of WinMain (0x4A7093) with the patch data below
* Run the new executable, click "Yes" when it asks you to dump and it will decompress the files to disk (by default C:/tmp/extermination-dump)

### Patch data

	@0x4A7093, 55 89 E5 FF 75 08 6A 00 90 FF 75 10 FF 75 14 FF 15 25 01 05 01 C9 C2 10 00 90 90

### Further work

The dll project provides a basic framework for calling into the existing game from your own code, there is some unfinished work in extermin.c which starts by replacing the WinMain & WndProc functions with new ones. There are unused addresses listed in addresses.h, there are many more I haven't pulled across from the RE database but with the decompressed files it should be easy to pull apart in IDA or Ghidra.

## Demo

### RES files

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

## Shared

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
