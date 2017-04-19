//
//  TextureBmp.cpp
//
//  This file is part of the ObjLibrary, by Richard Hamilton,
//    which is copyright Hamilton 2009-2014.
//
//  You may use these files for any purpose as long as you do
//    not explicitly claim them as your own work or object to
//    other people using them.  If you are in a position of
//    authority, you may forbid others ffrom using them in areas
//    that fall under your authority.  For example, a professor
//    could forbid students from using them for a class project,
//    or an employer could forbid employees using for a company
//    project.
//
//  If you are destributing the source files, you must not
//    remove this notice.  If you are only destributing compiled
//    code, no credit is required.
//
//  A (theoretically) up-to-date version of the ObjLibrary can
//    be found at:
//  http://infiniplix.ca/resources/obj_library/
//

#include <fstream>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstring>

#include "GetGlut.h"
#include "TextureBmp.h"

using namespace std;
namespace
{
	const unsigned int DEFAULT_WIDTH	= 1;
	const unsigned int DEFAULT_HEIGHT	= 1;


	//
	//  read2Bytes
	//  read4Bytes
	//
	//  Purpose: To read an unsigned 2-/4-byte integer from
	//	     the specified binary file.
	//  Parameter(s):
	//	<1> r_input_file: A reference to the file
	//  Precondition:
	//	<1> r_input_file contains at least 2/4 unread
	//	    bytes
	//  Returns: The unsigned integer read.
	//  Side Effect: The next 2/4 bytes are removed from
	//		 r_input_file.
	//

	unsigned int read2Bytes (ifstream& r_input_file)
	{
		unsigned int b1 = r_input_file.get();
		unsigned int b2 = r_input_file.get();

		return  b1 |
		       (b2 << 8);
	}

	unsigned int read4Bytes (ifstream& r_input_file)
	{
		unsigned int b1 = r_input_file.get();
		unsigned int b2 = r_input_file.get();
		unsigned int b3 = r_input_file.get();
		unsigned int b4 = r_input_file.get();

		return  b1 |
		       (b2 << 8) |
		       (b3 << 16) |
		       (b4 << 24);
	}

	//
	//  write2Bytes
	//  write4Bytes
	//
	//  Purpose: To write an unsigned 2-/4-byte integer
	//	     to the specified binary file.
	//  Parameter(s):
	//	<1> r_output_file: A reference to the binary
	//			   input file to write to
	//	<2> n: The integer to write
	//  Precondition:
	//	<1> r_output_file != NULL
	//	<2> r_output_file contains at least 2/4 unread
	//	    bytes
	//  Returns: N/A
	//  Side Effect: 2/4 bytes representing n are writern to 
	//		 r_output_file.
	//

	void write2Bytes (ofstream& r_output_file, unsigned int n)
	{
		r_output_file.put( n       & 0xFF);
		r_output_file.put((n >> 8) & 0xFF);
	}

	void write4Bytes (ofstream& r_output_file, unsigned int n)
	{
		r_output_file.put( n        & 0xFF);
		r_output_file.put((n >> 8)  & 0xFF);
		r_output_file.put((n >> 16) & 0xFF);
		r_output_file.put((n >> 24) & 0xFF);
	}

	//
	//  swap
	//
	//  Purpose: To swap the two specified values.
	//  Parameter(s):
	//	<1> r_a
	//	<2> r_b: References to the values to swap
	//  Reconditions: N/A
	//  Returns: N/A
	//  Side Effect: The values of r_a and r_b are swapped.
	//

	inline void swap (unsigned char& r_a, unsigned char& r_b)
	{
		unsigned char temp = r_a;
		r_a = r_b;
		r_b = temp;
	}

	//
	//  getBytesPerRowAlpha
	//  getBytesPerRowNoAlpha
	//
	//  Purpose: To determine the number of bytes in a row
	//	     of the specified length if there is/is not
	//	     an alpha channel.
	//  Parameter(s):
	//	<1> width: The length of the row
	//  Reconditions: N/A
	//  Returns: The number of Bytes used to store a row of
	//	     width tightly packed 3-/4-byte values.
	//  Side Effect: N/A
	//

	inline unsigned int getBytesPerRowAlpha (unsigned int width)
	{
		return width * 4;
	}

	inline unsigned int getBytesPerRowNoAlpha (unsigned int width)
	{
		return (((width + 3) / 4) * 4) * 3;
	}

}	// end of anonymous namespace



unsigned int TextureBmp :: loadImage (const char* a_filename)
{
	assert(a_filename != NULL);

	return loadImage(string(a_filename));
}

unsigned int TextureBmp :: loadImage (const string& filename)
{
	TextureBmp image(filename);
	if(image.isBad())
		return 0;

	return image.addToOpenGL();
}

unsigned int TextureBmp :: loadImage (const char* a_filename, unsigned char invisible_red, unsigned char invisible_green, unsigned char invisible_blue)
{
	assert(a_filename != NULL);

	return loadImage(string(a_filename), invisible_red, invisible_green, invisible_blue);
}

unsigned int TextureBmp :: loadImage (const string& filename, unsigned char invisible_red, unsigned char invisible_green, unsigned char invisible_blue)
{
	TextureBmp image_basic(filename);
	if(image_basic.isBad())
		return 0;

	TextureBmp image_alpha(image_basic, 0, 0, image_basic.getWidth(), image_basic.getHeight(), invisible_red, invisible_green, invisible_blue);
	assert(!image_alpha.isBad());
	return image_alpha.addToOpenGL();
}

void TextureBmp :: loadImageArray (
				const char* a_filename,
				unsigned int textures_x,
				unsigned int textures_y,
				unsigned int texture_width,
				unsigned int texture_height,
				unsigned int texture_spacing_x,
				unsigned int texture_spacing_y,
				unsigned int a_names[])
{
	assert(a_filename != NULL);
	assert(textures_x != 0);
	assert(textures_y != 0);
	assert(texture_width != 0);
	assert(texture_height != 0);

	loadImageArray(string(a_filename), textures_x, textures_y, texture_width, texture_height, texture_spacing_x, texture_spacing_y, a_names);
}

void TextureBmp :: loadImageArray (
				const string& filename,
				unsigned int textures_x,
				unsigned int textures_y,
				unsigned int texture_width,
				unsigned int texture_height,
				unsigned int texture_spacing_x,
				unsigned int texture_spacing_y,
				unsigned int a_names[])
{
	assert(textures_x != 0);
	assert(textures_y != 0);
	assert(texture_width != 0);
	assert(texture_height != 0);

	unsigned int total_textures = textures_x * textures_y;
	TextureBmp all(filename);

	if(all.isBad())
	{
		for(unsigned int i = 0; i < total_textures; i++)
			a_names[i] = 0;
		return;
	}

	unsigned int seperation_x, seperation_y;
	unsigned int total_required_width, total_required_height;
	unsigned int next_name = 0;

	seperation_x = texture_width  + texture_spacing_x;
	seperation_y = texture_height + texture_spacing_y;
	total_required_width  = textures_x * seperation_x - texture_spacing_x;
	total_required_height = textures_y * seperation_y - texture_spacing_y;

	assert(total_required_width <= all.getWidth());
	assert(total_required_height <= all.getHeight());

	for(unsigned int y = 0; y < textures_y; y++)
		for(unsigned int x = 0; x < textures_x; x++)
		{
			TextureBmp bit(all, x * seperation_x, y * seperation_y, texture_width, texture_height);
			assert(!bit.isBad());
			assert(next_name < total_textures);
			a_names[next_name] = bit.addToOpenGL();
			next_name++;
		}
}

void TextureBmp :: loadImageArray(
				const char* a_filename,
				unsigned int textures_x,
				unsigned int textures_y,
				unsigned int texture_width,
				unsigned int texture_height,
				unsigned int texture_spacing_x,
				unsigned int texture_spacing_y,
				unsigned int a_names[],
				unsigned char invisible_red,
				unsigned char invisible_green,
				unsigned char invisible_blue)
{
	assert(a_filename != NULL);
	assert(textures_x != 0);
	assert(textures_y != 0);
	assert(texture_width != 0);
	assert(texture_height != 0);

	loadImageArray(string(a_filename), textures_x, textures_y, texture_width, texture_height, texture_spacing_x, texture_spacing_y, a_names, invisible_red, invisible_green, invisible_blue);
}

void TextureBmp :: loadImageArray(
				const string& filename,
				unsigned int textures_x,
				unsigned int textures_y,
				unsigned int texture_width,
				unsigned int texture_height,
				unsigned int texture_spacing_x,
				unsigned int texture_spacing_y,
				unsigned int a_names[],
				unsigned char invisible_red,
				unsigned char invisible_green,
				unsigned char invisible_blue)
{
	assert(textures_x != 0);
	assert(textures_y != 0);
	assert(texture_width != 0);
	assert(texture_height != 0);

	unsigned int total_textures = textures_x * textures_y;
	TextureBmp all(filename);

	if(all.isBad())
	{
		for(unsigned int i = 0; i < total_textures; i++)
			a_names[i] = 0;
		return;
	}

	unsigned int seperation_x, seperation_y;
	unsigned int total_required_width, total_required_height;
	unsigned int next_name = 0;

	seperation_x = texture_width  + texture_spacing_x;
	seperation_y = texture_height + texture_spacing_y;
	total_required_width  = textures_x * seperation_x - texture_spacing_x;
	total_required_height = textures_y * seperation_y - texture_spacing_y;

	assert(total_required_width <= all.getWidth());
	assert(total_required_height <= all.getHeight());

	for(unsigned int y = 0; y < textures_y; y++)
		for(unsigned int x = 0; x < textures_x; x++)
		{
			TextureBmp bit(all, x * seperation_x, y * seperation_y, texture_width, texture_height, invisible_red, invisible_green, invisible_blue);
			assert(!bit.isBad());
			assert(next_name < total_textures);
			a_names[next_name] = bit.addToOpenGL();
			next_name++;
		}
}



TextureBmp :: TextureBmp ()
{
	md_texture = NULL;
	createDefault();

	assert(invariant());
}

TextureBmp :: TextureBmp (unsigned int width, unsigned int height, bool has_alpha)
{
	assert(width > 0);
	assert(height > 0);

	md_texture = NULL;
	init(width, height, has_alpha);

	assert(invariant());
}

TextureBmp :: TextureBmp (const char* a_filename)
{
	assert(a_filename != NULL);

	md_texture = NULL;

	load(string(a_filename), cerr);

	assert(invariant());
}

TextureBmp :: TextureBmp (const char* a_filename, const string& logfile)
{
	assert(a_filename != NULL);
	assert(logfile.compare("") != 0);

	md_texture = NULL;

	ofstream logstream(logfile.c_str());
	load(string(a_filename), logstream);
	logstream.close();

	assert(invariant());
}

TextureBmp :: TextureBmp (const char* a_filename, ostream& r_logstream)
{
	assert(a_filename != NULL);

	md_texture = NULL;

	load(string(a_filename), r_logstream);

	assert(invariant());
}

TextureBmp :: TextureBmp (const std::string& filename)
{
	md_texture = NULL;

	load(filename, cerr);

	assert(invariant());
}

TextureBmp :: TextureBmp (const std::string& filename, const string& logfile)
{
	assert(logfile.compare("") != 0);

	md_texture = NULL;

	ofstream logstream(logfile.c_str());
	load(filename, logstream);
	logstream.close();

	assert(invariant());
}

TextureBmp :: TextureBmp (const std::string& filename, ostream& r_logstream)
{
	md_texture = NULL;

	load(filename, r_logstream);

	assert(invariant());
}

TextureBmp :: TextureBmp (unsigned int width, unsigned int height, bool has_alpha, unsigned int array_size, unsigned char a_texture[])
{
	assert(a_texture != NULL);
	assert(width > 0);
	assert(height > 0);
	assert(!has_alpha || array_size == getBytesPerRowAlpha(width)   * height);
	assert( has_alpha || array_size == getBytesPerRowNoAlpha(width) * height);

	m_is_bad = false;
	m_width = width;
	m_height = height;
	m_has_alpha = has_alpha;

	if(m_has_alpha)
		m_bytes_per_row = getBytesPerRowAlpha(m_width);
	else
		m_bytes_per_row = getBytesPerRowNoAlpha(m_width);

	m_array_size = array_size;
	md_texture = a_texture;

	assert(invariant());
}

TextureBmp :: TextureBmp (const TextureBmp& source, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	assert(width > 0);
	assert(height > 0);
	assert(x + width <= source.getWidth());
	assert(y + height <= source.getHeight());

	m_is_bad = source.m_is_bad;
	m_width = width;
	m_height = height;
	m_has_alpha = source.hasAlphaChannel();

	if(m_has_alpha)
		m_bytes_per_row = getBytesPerRowAlpha(m_width);
	else
		m_bytes_per_row = getBytesPerRowNoAlpha(m_width);

	m_array_size = m_bytes_per_row * m_height;
	md_texture = new unsigned char[m_array_size];

	for(unsigned int i = 0; i < m_height; i++)
		for(unsigned int j = 0; j < m_width; j++)
		{
			unsigned int from_index;
			unsigned int to_index;

			if(m_has_alpha)
			{
				from_index = (y + i) * source.m_bytes_per_row + (x + j) * 4;
				to_index   =      i  *        m_bytes_per_row +      j  * 4;

				assert(from_index + 3 < source.m_array_size);
				assert(to_index   + 3 <        m_array_size);

				md_texture[to_index + 3] = source.md_texture[from_index + 3];
			}
			else
			{
				from_index = (y + i) * source.m_bytes_per_row + (x + j) * 3;
				to_index   =      i  *        m_bytes_per_row +      j  * 3;

				assert(from_index + 2 < source.m_array_size);
				assert(to_index   + 2 <        m_array_size);
			}

			md_texture[to_index]     = source.md_texture[from_index];
			md_texture[to_index + 1] = source.md_texture[from_index + 1];
			md_texture[to_index + 2] = source.md_texture[from_index + 2];
		}

	assert(invariant());
}

TextureBmp :: TextureBmp (const TextureBmp& source, unsigned int x, unsigned int y, unsigned int width, unsigned int height,
			  unsigned char invisible_red, unsigned char invisible_green, unsigned char invisible_blue)
{
	assert(width > 0);
	assert(height > 0);
	assert(x + width <= source.getWidth());
	assert(y + height <= source.getHeight());
	assert(source.hasAlphaChannel() == false);

	m_is_bad = source.m_is_bad;
	m_width = width;
	m_height = height;
	m_has_alpha = true;
	m_bytes_per_row = getBytesPerRowAlpha(m_width);
	m_array_size = m_bytes_per_row * m_height;
	md_texture = new unsigned char[m_array_size];

	for(unsigned int i = 0; i < m_height; i++)
		for(unsigned int j = 0; j < m_width; j++)
		{
			unsigned int from_index = (y + i) * source.m_bytes_per_row + (x + j) * 3;
			unsigned int to_index   =      i  *        m_bytes_per_row +      j  * 4;

			md_texture[to_index]     = source.md_texture[from_index];
			md_texture[to_index + 1] = source.md_texture[from_index + 1];
			md_texture[to_index + 2] = source.md_texture[from_index + 2];

			assert(from_index + 2 < source.m_array_size);
			assert(to_index   + 3 <        m_array_size);

			unsigned char red   = source.md_texture[from_index];
			unsigned char green = source.md_texture[from_index + 1];
			unsigned char blue  = source.md_texture[from_index + 2];

			md_texture[to_index + 0] = red;
			md_texture[to_index + 1] = green;
			md_texture[to_index + 2] = blue;

			if(red == invisible_red && green == invisible_green && blue == invisible_blue)
				md_texture[to_index + 3] = 0;
			else
				md_texture[to_index + 3] = 255;
		}

	assert(invariant());
}

TextureBmp :: TextureBmp (const TextureBmp& original)
{
	md_texture = NULL;
	copy(original);

	assert(invariant());
}

TextureBmp& TextureBmp :: operator= (const TextureBmp& original)
{
	if(&original != this)
	{
		destroy();
		copy(original);
	}

	assert(invariant());
	return *this;
}

TextureBmp :: ~TextureBmp ()
{
	destroy();
}



unsigned char TextureBmp :: getRed(unsigned int x, unsigned int y) const
{
	assert(x < getWidth());
	assert(y < getHeight());

	if(hasAlphaChannel())
	{
		unsigned int index = y * m_bytes_per_row + x * 4;
		assert(index + 3 < m_array_size);
		return md_texture[index];
	}
	else
	{
		unsigned int index = y * m_bytes_per_row + x * 3;
		assert(index + 2 < m_array_size);
		return md_texture[index];
	}
}

unsigned char TextureBmp :: getGreen(unsigned int x, unsigned int y) const
{
	assert(x < getWidth());
	assert(y < getHeight());

	if(hasAlphaChannel())
	{
		unsigned int index = y * m_bytes_per_row + x * 4;
		assert(index + 3 < m_array_size);
		return md_texture[index + 1];
	}
	else
	{
		unsigned int index = y * m_bytes_per_row + x * 3;
		assert(index + 2 < m_array_size);
		return md_texture[index + 1];
	}
}

unsigned char TextureBmp :: getBlue(unsigned int x, unsigned int y) const
{
	assert(x < getWidth());
	assert(y < getHeight());

	if(hasAlphaChannel())
	{
		unsigned int index = y * m_bytes_per_row + x * 4;
		assert(index + 3 < m_array_size);
		return md_texture[index + 2];
	}
	else
	{
		unsigned int index = y * m_bytes_per_row + x * 3;
		assert(index + 2 < m_array_size);
		return md_texture[index + 2];
	}
}

unsigned char TextureBmp :: getAlpha(unsigned int x, unsigned int y) const
{
	assert(x < getWidth());
	assert(y < getHeight());
	assert(hasAlphaChannel());

	unsigned int index = y * m_bytes_per_row + x * 4;
	assert(index + 3 < m_array_size);
	return md_texture[index + 3];
}

unsigned int TextureBmp :: getRGB (unsigned int x, unsigned int y) const
{
	assert(x < getWidth());
	assert(y < getHeight());

	if(hasAlphaChannel())
	{
		unsigned int index = y * m_bytes_per_row + x * 4;
		assert(index + 3 < m_array_size);
		return *((unsigned int*)(md_texture + index)) & 0xFFFFFFFF;
	}
	else
	{
		unsigned int index = y * m_bytes_per_row + x * 3;
		assert(index + 2 < m_array_size);
		return *((unsigned int*)(md_texture + index)) & 0x00FFFFFF;
	}
}

void TextureBmp :: save (const char* a_filename) const
{
	assert(a_filename != NULL);
	assert(strlen(a_filename) != 0);

	save(string(a_filename));
}

void TextureBmp :: save (const std::string& filename) const
{
	assert(filename.length() != 0);

	ofstream outfile;
	unsigned int file_size;

	outfile.open(filename.c_str(), ios::out | ios::binary);

//  Header, 14 bytes.
//    16 bits FileType;        Magic number: "BM",
//    32 bits FileSize;        Size of file in 32 byte integers,
//    16 bits Reserved1;       Always 0,
//    16 bits Reserved2;       Always 0,
//    32 bits BitmapOffset.    Starting position of image data, in bytes.

	assert(m_array_size % 4 == 0);
	file_size = 4 + 40;	// headers
	file_size += m_array_size / 4;	// data

	outfile.put('B');
	outfile.put('M');
	write4Bytes(outfile, file_size);
	write2Bytes(outfile, 0);
	write2Bytes(outfile, 0);
	write4Bytes(outfile, 54);

//  The bitmap header is 40 bytes long.
//    4 bytes unsigned Size;            Size of this header, in bytes.
//    4 bytes Width;                    Image width, in pixels.   
//    4 bytes Height;                   Image height, in pixels.
//						(Pos/Neg, origin at bottom, top)
//    2 bytes Planes;                   Number of color planes (always 1).
//    2 bytes BitsPerPixel;             1 to 24.  1, 4, 8 and 24 legal.
//						16 and 32 on Win95.
//    4 bytes unsigned Compression;     0, uncompressed; 1, 8 bit RLE;
//						2, 4 bit RLE; 3, bitfields.
//    4 bytes unsigned SizeOfBitmap;    Size of bitmap in bytes.
//						(0 if uncompressed).
//    4 bytes HorzResolution;           Pixels per meter. (Can be zero)
//    4 bytes VertResolution;           Pixels per meter. (Can be zero)
//    4 bytes unsigned ColorsUsed;      Number of colors in palette.
//						(Can be zero).
//    4 bytes unsigned ColorsImportant. Minimum number of important colors.
//						(Can be zero).

	write4Bytes(outfile, 40);
	write4Bytes(outfile, m_width);
	write4Bytes(outfile, m_height);
	write2Bytes(outfile, 1);
	write2Bytes(outfile, 24);
	write4Bytes(outfile, 0);
	write4Bytes(outfile, 0);
	write4Bytes(outfile, 0);
	write4Bytes(outfile, 0);
	write4Bytes(outfile, 0);
	write4Bytes(outfile, 0);

	//
	//  The data in a .bmp file is from bottom to top for
	//    some odd reason.  We need to print the lines in
	//    reverse order.
	//
	//  Also, we have to reverse the red and blue values,
	//    skip any alpha values, and pad the rows to 4 bytes.
	//

	for(int y = m_height - 1; y >= 0; y--)
	{
		for(unsigned int x = 0; x < m_width; x++)
		{
			unsigned int rgb = getRGB(x, y);

			// write in BGR order
			outfile.put((rgb >> 16) & 0xFF);
			outfile.put((rgb >> 8)  & 0xFF);
			outfile.put( rgb        & 0xFF);
		}

		if(!m_has_alpha)
		{
			// pad the rows to 32-bit words
			switch(m_width % 4)
			{
			case 1:
				outfile.put(0x00);
				break;
			case 2:
				outfile.put(0x00);
				outfile.put(0x00);
				break;
			case 3:
				outfile.put(0x00);
				outfile.put(0x00);
				outfile.put(0x00);
				break;
			}
		}
	}

	outfile.close();
}



void TextureBmp :: init (unsigned int width, unsigned int height, bool has_alpha)
{
	assert(width > 0);
	assert(height > 0);

	if(md_texture != NULL)
		destroy();
	assert(md_texture == NULL);

	m_is_bad = false;
	m_width = width;
	m_height = height;
	m_has_alpha = has_alpha;

	if(m_has_alpha)
		m_bytes_per_row = getBytesPerRowAlpha(m_width);
	else
		m_bytes_per_row = getBytesPerRowNoAlpha(m_width);

	m_array_size = m_bytes_per_row * m_height;
	md_texture = new unsigned char[m_array_size];

	for(unsigned int i = 0; i < m_array_size; i += 4)
	{
		// zero array as 4-byte words
		*((unsigned int*)(md_texture + i)) = 0;
	}

	assert(invariant());
}

void TextureBmp :: load (const char* a_filename)
{
	assert(a_filename != NULL);

	load(string(a_filename), cerr);

	assert(invariant());
}

void TextureBmp :: load (const char* a_filename, const string& logfile)
{
	assert(a_filename != NULL);
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	load(string(a_filename), logstream);
	logstream.close();

	assert(invariant());
}

void TextureBmp :: load (const char* a_filename, ostream& r_logstream)
{
	assert(a_filename != NULL);

	load(string(a_filename), r_logstream);

	assert(invariant());
}

void TextureBmp :: load (const string& filename)
{
	load(filename, cerr);

	assert(invariant());
}

void TextureBmp :: load (const string& filename, const string& logfile)
{
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	load(filename, logstream);
	logstream.close();

	assert(invariant());
}

void TextureBmp :: load (const string& filename, ostream& r_logstream)
{
	unsigned int bit_depth;
	unsigned int header_size;

	if(md_texture != NULL)
		destroy();
	assert(md_texture == NULL);

	m_is_bad = false;

	// Open the input file.
	ifstream input_file(filename.c_str(), ios::in | ios::binary);
	if(!input_file.is_open())
	{
		input_file.close();
		r_logstream << "Error: File \"" << filename << "\" does not exist" << endl;
		md_texture = NULL;
		createDefault();
		m_is_bad = true;

		assert(invariant());
		return;
	}

//  Header, 14 bytes.
//    16 bits FileType;        Magic number: "BM",
//    32 bits FileSize;        Size of file in 32 byte integers,
//    16 bits Reserved1;       Always 0,
//    16 bits Reserved2;       Always 0,
//    32 bits BitmapOffset.    Starting position of image data, in bytes.

	// Check to make sure this is a BMP file
	if(input_file.get() != 'B' || input_file.get() != 'M')
	{
		input_file.close();
		r_logstream << "Error: File \"" << filename << "\" is not a bmp" << endl;
		md_texture = NULL;
		createDefault();
		m_is_bad = true;

		assert(invariant());
		return;
	}

	input_file.ignore(12);
	
//  The bitmap header is 40 bytes long.  At least in theory...
//    4 bytes unsigned Size;            Size of this header, in bytes.
//    4 bytes Width;                    Image width, in pixels.   
//    4 bytes Height;                   Image height, in pixels.  (Pos/Neg, origin at bottom, top)
//    2 bytes Planes;                   Number of color planes (always 1).
//    2 bytes BitsPerPixel;             1 to 24.  1, 4, 8 and 24 legal.  16 and 32 on Win95.
//    4 bytes unsigned Compression;     0, uncompressed; 1, 8 bit RLE; 2, 4 bit RLE; 3, bitfields.
//    4 bytes unsigned SizeOfBitmap;    Size of bitmap in bytes. (0 if uncompressed).
//    4 bytes HorzResolution;           Pixels per meter. (Can be zero)
//    4 bytes VertResolution;           Pixels per meter. (Can be zero)
//    4 bytes unsigned ColorsUsed;      Number of colors in palette.  (Can be zero).
//    4 bytes unsigned ColorsImportant. Minimum number of important colors. (Can be zero).

	// Read in the properties of the BMP file, discard all unused data
	header_size = read4Bytes(input_file);	// how long the header really is
	m_width = read4Bytes(input_file);
	m_height = read4Bytes(input_file);							
	input_file.ignore(2);

	// check depth
	bit_depth = read2Bytes(input_file);
	if(bit_depth == 24)
	{
		m_has_alpha = false;

		//
		//  Determine how many 4-byte words per row to use in
		//    image.  This may be differant from width because
		//    both Bmp and OpenGL only break rows at the end of
		//    a 4-byte word.
		//
		m_bytes_per_row = getBytesPerRowNoAlpha(m_width);
	}
	else if(bit_depth == 32)
	{
		m_has_alpha = true;
		m_bytes_per_row = getBytesPerRowAlpha(m_width);
	}
	else
	{
		input_file.close();
		r_logstream << "Error: File \"" << filename << "\" is not 24-bit or 32-bit" << endl;
		md_texture = NULL;
		createDefault();
		m_is_bad = true;

		assert(invariant());
		return;
	}

	input_file.ignore(header_size - 16);	// skip the rest of the header

	// reserve required memory
	m_array_size = m_bytes_per_row * m_height;
	md_texture = new unsigned char[m_array_size];

	// read actual bitmap
	input_file.read ( (char*)(md_texture), m_array_size);

	input_file.close();

	// reorder pixel colour components
	for(unsigned int y = 0; y < m_height; y++)
		for(unsigned int x = 0; x < m_width; x++)
		{
			unsigned int array_spot;

			if(m_has_alpha)
			{
				// ABGR <-> RGB1
				array_spot = y * m_bytes_per_row + x * 4;
				assert(array_spot + 3 < m_array_size);

				swap(md_texture[array_spot + 1], md_texture[array_spot + 2]);
				md_texture[array_spot] = md_texture[array_spot + 3];
				md_texture[array_spot + 3] = 0xFF;
			}
			else
			{
				// BGR <-> RGB
				array_spot = y * m_bytes_per_row + x * 3;
				assert(array_spot + 2 < m_array_size);

				swap(md_texture[array_spot], md_texture[array_spot + 2]);
			}
		}
	mirrorY();

	assert(invariant());
}

void TextureBmp :: mirrorX ()
{
	for(unsigned int x1 = 0; x1 < m_width / 2; x1++)
	{
		unsigned int x2 = m_width - 1 - x1;

		for(unsigned int y = 0; y < m_height; y++)
		{
			unsigned int array_spot1;
			unsigned int array_spot2;

			if(m_has_alpha)
			{
				array_spot1 = y * m_bytes_per_row + x1 * 4;
				array_spot2 = y * m_bytes_per_row + x2 * 4;
				assert(array_spot1 + 3 < m_array_size);
				assert(array_spot2 + 3 < m_array_size);

				swap(md_texture[array_spot1 + 3], md_texture[array_spot2 + 3]);
			}
			else
			{
				array_spot1 = y * m_bytes_per_row + x1 * 3;
				array_spot2 = y * m_bytes_per_row + x2 * 3;
				assert(array_spot1 + 2 < m_array_size);
				assert(array_spot2 + 2 < m_array_size);
			}

			swap(md_texture[array_spot1],     md_texture[array_spot2]);
			swap(md_texture[array_spot1 + 1], md_texture[array_spot2 + 1]);
			swap(md_texture[array_spot1 + 2], md_texture[array_spot2 + 2]);
		}
	}
}

void TextureBmp :: mirrorY ()
{
	for(unsigned int y1 = 0; y1 < m_height / 2; y1++)
	{
		unsigned int y2 = m_height - 1 - y1;

		for(unsigned int x = 0; x < m_width; x++)
		{
			unsigned int array_spot1;
			unsigned int array_spot2;

			if(m_has_alpha)
			{
				array_spot1 = y1 * m_bytes_per_row + x * 4;
				array_spot2 = y2 * m_bytes_per_row + x * 4;
				assert(array_spot1 + 3 < m_array_size);
				assert(array_spot2 + 3 < m_array_size);

				swap(md_texture[array_spot1 + 3], md_texture[array_spot2 + 3]);
			}
			else
			{
				array_spot1 = y1 * m_bytes_per_row + x * 3;
				array_spot2 = y2 * m_bytes_per_row + x * 3;
				assert(array_spot1 + 2 < m_array_size);
				assert(array_spot2 + 2 < m_array_size);
			}

			swap(md_texture[array_spot1],     md_texture[array_spot2]);
			swap(md_texture[array_spot1 + 1], md_texture[array_spot2 + 1]);
			swap(md_texture[array_spot1 + 2], md_texture[array_spot2 + 2]);
		}
	}
}

void TextureBmp :: setPixel(unsigned int x, unsigned int y, unsigned char red, unsigned char green, unsigned char blue)
{
	assert(x < getWidth());
	assert(y < getHeight());
	assert(!hasAlphaChannel());

	unsigned int index = y * m_bytes_per_row + x * 3;
	assert(index + 2 < m_array_size);

	md_texture[index]     = red;
	md_texture[index + 1] = green;
	md_texture[index + 2] = blue;

	assert(invariant());
}

void TextureBmp :: setPixel(unsigned int x, unsigned int y, unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha)
{
	assert(x < getWidth());
	assert(y < getHeight());
	assert(hasAlphaChannel());

	unsigned int index = y * m_bytes_per_row + x * 4;
	assert(index + 3 < m_array_size);

	md_texture[index]     = red;
	md_texture[index + 1] = green;
	md_texture[index + 2] = blue;
	md_texture[index + 3] = alpha;

	assert(invariant());
}

void TextureBmp :: setPixel (unsigned int x, unsigned int y, unsigned int rgb)
{
	assert(x < getWidth());
	assert(y < getHeight());

	unsigned int index;

	if(hasAlphaChannel())
	{
		index = y * m_bytes_per_row + x * 4;
		assert(index + 3 < m_array_size);

		md_texture[index]     =  rgb        & 0xFF;
		md_texture[index + 1] = (rgb >> 8)  & 0xFF;
		md_texture[index + 2] = (rgb >> 16) & 0xFF;
		md_texture[index + 3] = (rgb >> 24) & 0xFF;
	}
	else
	{
		index = y * m_bytes_per_row + x * 3;
		assert(index + 2 < m_array_size);

		md_texture[index]     =  rgb        & 0xFF;
		md_texture[index + 1] = (rgb >> 8)  & 0xFF;
		md_texture[index + 2] = (rgb >> 16) & 0xFF;
	}

	assert(invariant());
}

unsigned int TextureBmp :: addToOpenGL () const
{
	return addToOpenGL(GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
}

unsigned int TextureBmp :: addToOpenGL (unsigned int wrap) const
{
	assert(wrap == GL_REPEAT || wrap == GL_CLAMP);

	return addToOpenGL(wrap, wrap, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
}

unsigned int TextureBmp :: addToOpenGL (unsigned int wrap_s, unsigned int wrap_t) const
{
	assert(wrap_s == GL_REPEAT || wrap_s == GL_CLAMP);
	assert(wrap_t == GL_REPEAT || wrap_t == GL_CLAMP);

	return addToOpenGL(wrap_s, wrap_t, GL_NEAREST, GL_NEAREST_MIPMAP_NEAREST);
}

unsigned int TextureBmp :: addToOpenGL (unsigned int wrap_s, unsigned int wrap_t, unsigned int mag_filter, unsigned int min_filter) const
{
	assert(wrap_s == GL_REPEAT || wrap_s == GL_CLAMP);
	assert(wrap_t == GL_REPEAT || wrap_t == GL_CLAMP);
	assert(mag_filter == GL_NEAREST ||
	       mag_filter == GL_LINEAR);
	assert(min_filter == GL_NEAREST ||
	       min_filter == GL_LINEAR ||
	       min_filter == GL_NEAREST_MIPMAP_NEAREST ||
	       min_filter == GL_NEAREST_MIPMAP_LINEAR ||
	       min_filter == GL_LINEAR_MIPMAP_NEAREST ||
	       min_filter == GL_LINEAR_MIPMAP_LINEAR);

	unsigned int name;

	glGenTextures(1, &name);
	glBindTexture(GL_TEXTURE_2D, name);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter);

	if(min_filter == GL_NEAREST || min_filter == GL_LINEAR)
	{
		// void glTexImage2D(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
		if(m_has_alpha)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, md_texture);
		else
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,  m_width, m_height, 0, GL_RGB,  GL_UNSIGNED_BYTE, md_texture);
	}
	else
	{
		// GLint gluBuild2DMipmaps(GLenum  target,  GLint  internalFormat,  GLsizei  width,  GLsizei  height,  GLenum  format,  GLenum  type,  const void *  data);
		if(m_has_alpha)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, md_texture);
		else
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB,  m_width, m_height, GL_RGB,  GL_UNSIGNED_BYTE, md_texture);
	}

	return name;
}



void TextureBmp :: createDefault ()
{
	assert(md_texture == NULL);

	m_is_bad = false;
	m_width = DEFAULT_WIDTH;
	m_height = DEFAULT_HEIGHT;
	m_bytes_per_row = getBytesPerRowNoAlpha(m_width);
	m_has_alpha = false;
	m_array_size = m_bytes_per_row * m_height;

	md_texture = new unsigned char[m_array_size];
	for(unsigned int i = 0; i < m_array_size; i++)
		md_texture[i] = 0xFF;

	assert(invariant());
}

void TextureBmp :: copy (const TextureBmp& original)
{
	assert(md_texture == NULL);

	m_is_bad = original.m_is_bad;
	m_width = original.m_width;
	m_height = original.m_height;
	m_has_alpha = original.m_has_alpha;
	m_bytes_per_row = original.m_bytes_per_row;
	m_array_size = original.m_array_size;

	md_texture = new unsigned char[m_array_size];
	for(unsigned int i = 0; i < m_array_size; i++)
		md_texture[i] = original.md_texture[i];

	assert(invariant());
}

void TextureBmp :: destroy ()
{
	assert(md_texture != NULL);

	delete md_texture;
	md_texture = NULL;

	assert(md_texture == NULL);
}

bool TextureBmp :: invariant () const
{
	if(md_texture == NULL) return false;
	if(m_width <= 0) return false;
	if(m_height <= 0) return false;
	if(!m_has_alpha && m_bytes_per_row != getBytesPerRowNoAlpha(m_width)) return false;
	if( m_has_alpha && m_bytes_per_row !=   getBytesPerRowAlpha(m_width)) return false;
	if(m_array_size != m_bytes_per_row * m_height) return false;
	return true;
}
