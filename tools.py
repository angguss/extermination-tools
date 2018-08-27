'''
extermination-tools
Copyright (C) 2018 angguss

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
'''

import os, struct
from PIL import Image

class ResFileEntry(object):
    file_index_format = '<23sB2I'
    def __init__(self, bytes):
        unpacked = struct.unpack(ResFileEntry
        .file_index_format, bytes)
        # Strip null bytes
        self.filename = unpacked[0].decode('utf-8').split('\0',1)[0]
        # This is always 1 in demo files
        self.unk_1 = unpacked[1]
        self.file_offset = unpacked[2]
        self.file_length = unpacked[3]

    def __str__(self):
        return str(self.filename)

# Read a RES file and extract contents to a directory
def read_res(output_dir, filename):
    header_format = '<4cI'
    
    # Attempt to create destination directory
    try:
        os.makedirs(dest_path)
    except:
        pass

    files = []

    with open(filename, 'rb') as f:
        header = struct.unpack(header_format, f.read(8))
        f.seek(header[4])

        # Read all entries
        while(1):
            try:
                files.append(ResFileEntry(f.read(32)))
            except:
                break
		
		# Dump out all entries
	    for file in files:
	        f.seek(file.file_offset)
	        file_bytes = f.read(file.file_length)
	        dest_file = output_dir + "/" + file.filename
	        dest_path = os.path.dirname(dest_file)
	        try:
	            os.makedirs(dest_path)
	        except:
	            pass
	        with open(dest_file, "wb") as fw:
	            fw.write(file_bytes)
	        if ".GFX" in dest_file.upper():
	            try:
	                convert_gfx_to_png(dest_file)
	            except:
	                print("Failed to convert: " + dest_file)
	                pass

# Convert a menu.res GFX format file to PNG
def convert_gfx_to_png(filename):
    in_file = filename
    with open(in_file, "rb") as f:
        head = f.read(4)
        if head == b'dupa':
            return
        f.seek(0)
        w = struct.unpack('<H', f.read(2))[0]
        h = struct.unpack('<H', f.read(2))[0]

        print("Converting " + filename + " to PNG")
        readlen = os.path.getsize(in_file) - 4
        pixel_ct = int(readlen / 2)
        struct_str = '<' + str(pixel_ct) + 'H'
        b = struct.unpack(struct_str, f.read(readlen))
        image = Image.new('RGB', (w, h))
        pixels = image.load()
        for i in range(0, pixel_ct):
            red = ((b[i] >> 11) & 0x1F)
            green = ((b[i] >> 5) & 0x3F) 
            blue = (b[i] & 0x1F)
            pixels[int(i % w), int(i / w)] = (red, green, blue)
        image.save(filename.replace("GFX", "PNG"), "PNG")


def main():
    read_res("./menu/", "MENU.RES")

main()