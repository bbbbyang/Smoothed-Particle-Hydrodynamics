//
//  SpriteFont.cpp
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

#include <cassert>
#include <cctype>
#include <string>
#include <vector>

#include "GetGlut.h"
#include "TextureBmp.h"
#include "SpriteFont.h"



// These must be above the case expressions that use them.
const unsigned int SpriteFont :: PLAIN                = 0x0;
const unsigned int SpriteFont :: BOLD                 = 0x1;
const unsigned int SpriteFont :: MIRROR               = 0x2;
const unsigned int SpriteFont :: ITALICS              = 0x4;
const unsigned int SpriteFont :: SLANT                = 0x8;
const unsigned int SpriteFont :: BACK_ITALICS         = 0xC;
const unsigned int SpriteFont :: UNDERLINE            = 0x10;
const unsigned int SpriteFont :: DOUBLE_UNDERLINE     = 0x20;
const unsigned int SpriteFont :: RED_UNDERLINE        = 0x30;
const unsigned int SpriteFont :: STRIKETHROUGH        = 0x40;
const unsigned int SpriteFont :: DOUBLE_STRIKETHROUGH = 0x80;
const unsigned int SpriteFont :: RED_STRIKETHROUGH    = 0xC0;
const unsigned int SpriteFont :: WIDE                 = 0x100;
const unsigned int SpriteFont :: VERY_WIDE            = 0x200;
const unsigned int SpriteFont :: NARROW               = 0x300;

using namespace std;
namespace
{
	const unsigned int TEXTURES_PER_ROW = 16;

	const unsigned int SLANT_BLOCK = 0xC;
	const unsigned int UNDERLINE_BLOCK = 0x30;
	const unsigned int STRIKETHROUGH_BLOCK = 0xC0;
	const unsigned int WIDTH_BLOCK = 0x300;

	const unsigned int INVALID_FORMAT_MASK = ~(0xFFFu);

	//
	//  getWidthForFormat
	//  getHeightForFormat
	//
	//  Purpose: To determine the change in width/height
	//	     needed for the specified format.
	//  Parameter(s):
	//	<1> format: The format
	//  Precondition(s): N/A
	//  Returns: The change in width/height needed to
	//	     display a string with formatting format.
	//  Side Effect: N/A
	//

	int getWidthForFormat (unsigned int format)
	{
		int extra = 0;

		if((format & SpriteFont::BOLD) == SpriteFont::BOLD)
			extra += 1;

		switch(format & WIDTH_BLOCK)
		{
		case SpriteFont::WIDE:      extra += 1; break;
		case SpriteFont::VERY_WIDE: extra += 3; break;
		case SpriteFont::NARROW:    extra -= 1; break;
		}

		return extra;
	}

	int getHeightForFormat (unsigned int format)
	{
		switch(format & UNDERLINE_BLOCK)
		{
		case SpriteFont::UNDERLINE:        return 2;
		case SpriteFont::DOUBLE_UNDERLINE: return 3;
		case SpriteFont::RED_UNDERLINE:    return 2;
		default:                           return 0;
		}

	}
	//
	//  getSlantAmountForFormat
	//
	//  Purpose: To determine the slant amount needed for
	//	     the specified format.  This is a movement
	//	     in the +X direction on the top and the -X
	//	     on the bottom.
	//  Parameter(s):
	//	<1> format: The format
	//	<2> height: The height of the text to be slanted
	//  Precondition(s): N/A
	//  Returns: The slant amount needed to display a string
	//	     with formatting format.
	//  Side Effect: N/A
	//

	int getSlantAmountForFormat (unsigned int format, int height)
	{
		switch(format & SLANT_BLOCK)
		{
		case SpriteFont::ITALICS:      return height / 6;
		case SpriteFont::SLANT:        return height / 3;
		case SpriteFont::BACK_ITALICS: return height / -6;
		default:                       return 0;
		}
	}

	//
	//  drawLineThrough
	//
	//  Purpose: To draw an underline or strikethrough for
	//	     text of the specified length.  This is the
	//	     same line, it just is at differant heights.
	//  Parameter(s):
	//	<1> start_x
	//	<2> end_x: The start and end of the text
	//	<3> y: The y-coordinate of the line
	//	<4> variant: The variant of line through to draw
	//  Precondition(s):
	//	<1> start_x <= end_x
	//  Returns: N/A
	//  Side Effect: An underline or strikethrough is drawn
	//		 with y-coordinate y for text between
	//		 the x-coordinates start_x amd end_x.
	//		 Variants UNDERLINE and STRIKETHROUGH
	//		 draw a single line, DOUBLE_UNDERLINE
	//		 and DOUBLE_STRIKETHROUGH draw a double
	//		 line, and RED_UNDERLINE and
	//		 RED_STRIKETHROUGH draw a single thick
	//		 red line.
	//

	void drawLineThrough (double start_x, double end_x,
			      double y, unsigned int format)
	{
		switch(format)
		{
		case SpriteFont::UNDERLINE:
		case SpriteFont::STRIKETHROUGH:
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
				glVertex2d(start_x - 1, y);
				glVertex2d(end_x   + 2, y);
			glEnd();
			break;
		case SpriteFont::DOUBLE_UNDERLINE:
		case SpriteFont::DOUBLE_STRIKETHROUGH:
			glDisable(GL_TEXTURE_2D);
			glBegin(GL_LINES);
				glVertex2d(start_x - 1, y - 1);
				glVertex2d(end_x   + 2, y - 1);
				glVertex2d(start_x - 1, y + 1);
				glVertex2d(end_x   + 2, y + 1);
			glEnd();
			break;
		case SpriteFont::RED_UNDERLINE:
		case SpriteFont::RED_STRIKETHROUGH:
			glDisable(GL_TEXTURE_2D);
			glColor4ub(0xFF, 0x00, 0x00, 0xFF);
			glBegin(GL_LINES);
				glVertex2d(start_x - 1, y - 1);
				glVertex2d(end_x   + 2, y - 1);
				glVertex2d(start_x - 1, y);
				glVertex2d(end_x   + 2, y);
			glEnd();
			break;
		}
	}

	bool g_is_2d_view_set_up = false;

}	// end of anonymous namespace





const unsigned int SpriteFont :: NO_BREAK_NEEDED = ~0u;

bool SpriteFont :: isAPowerOf2 (unsigned int n)
{
	for(unsigned int i = 1; i != 0; i <<= 1)
		if(n == i)
			return true;
	return false;
}

bool SpriteFont :: isValidFormat (unsigned int format)
{
	if((format & INVALID_FORMAT_MASK) == 0)
		return true;
	else
		return false;
}

bool SpriteFont :: is2dViewSetUp ()
{
	return g_is_2d_view_set_up;
}

void SpriteFont :: setUp2dView (int width, int height)
{
	assert(!is2dViewSetUp());

	if(width < 1)
		width = 1;
	if(height < 1)
		height = 1;

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	// glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
	glOrtho(0.0, width, 0.0, height, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslated(0.0, (double)(height), 0.0);
	glScaled(1.0, -1.0, 1.0);

	g_is_2d_view_set_up = true;
}

void SpriteFont :: unsetUp2dView ()
{
	assert(is2dViewSetUp());

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	g_is_2d_view_set_up = false;
}



SpriteFont :: SpriteFont ()
{
	m_character_count = 0;
	m_image_size = 0;
	m_character_height = 0;

	for(unsigned int i = 0; i < 0x100; i++)
	{
		ma_character_name[i] = 0;
		ma_character_width[i] = 0;
	}

	assert(invariant());
}

SpriteFont :: SpriteFont (const char* a_image, unsigned char red, unsigned char green, unsigned char blue)
{
	assert(a_image != NULL);
	assert(red != green || red != blue);

	m_character_count = 0;

	load(a_image, red, green, blue);

	assert(invariant());
}

SpriteFont :: SpriteFont (const string& image, unsigned char red, unsigned char green, unsigned char blue)
{
	assert(red != green || red != blue);

	m_character_count = 0;

	load(image.c_str(), red, green, blue);

	assert(invariant());
}

SpriteFont :: ~SpriteFont ()
{
	// delete a whole array of textures - with one command!

	// glDeleteTextures(GLsizei n, const GLuint *textureNames);
	glDeleteTextures(m_character_count, ma_character_name);
}



bool SpriteFont :: isInitalized () const
{
	if(m_character_count != 0)
		return true;
	else
		return false;
}

bool SpriteFont :: is8Bit () const
{
	assert(isInitalized());

	if(m_character_count == 256)
		return true;
	else
		return false;
}

int SpriteFont :: getHeight () const
{
	assert(isInitalized());

	return m_character_height;
}

int SpriteFont :: getHeight (unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));

	return m_character_height + getHeightForFormat(format);
}

int SpriteFont :: getWidth (char character) const
{
	assert(isInitalized());

	return ma_character_width[character];
}

int SpriteFont :: getWidth (char character, unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));

	return ma_character_width[character] + getWidthForFormat(format);
}



int SpriteFont :: getWidth (const char* a_str) const
{
	assert(isInitalized());
	assert(a_str != NULL);

	return getWidth(a_str, PLAIN);
}

int SpriteFont :: getWidth (const string& str) const
{
	assert(isInitalized());

	return getWidth(str.c_str(), PLAIN);
}

int SpriteFont :: getWidth (const char* a_str, unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));
	assert(a_str != NULL);

	int extra = getWidthForFormat(format);

	int total = 0;
	for( ; *a_str != '\0'; a_str++)
		total += ma_character_width[*a_str] + extra;

	return total;
}

int SpriteFont :: getWidth (const string& str, unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));

	return getWidth(str.c_str(), format);
}



unsigned int SpriteFont :: getBreakPoint (const char* a_str, unsigned int width) const
{
	assert(isInitalized());
	assert(a_str != NULL);

	return getBreakPoint(a_str, width, PLAIN);
}

unsigned int SpriteFont :: getBreakPoint (const string& str, unsigned int width) const
{
	assert(isInitalized());

	return getBreakPoint(str.c_str(), width, PLAIN);
}

unsigned int SpriteFont :: getBreakPoint (const char* a_str, unsigned int width, unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));
	assert(a_str != NULL);

	int extra = getWidthForFormat(format);;

	unsigned int word_start = 0;
	int total_length = 0;
	for(unsigned int current = 1; a_str[current] != '\0'; current++)
	{
		unsigned char character = a_str[current];

		total_length += (int)(ma_character_width[character]) + extra;

		if(!isspace(character))
		{
			assert(current >= 1);

			if(isspace(a_str[current - 1]))
				word_start = current;

			if(total_length > (int)width)
			{
				if(word_start == 0)
					return current;
				else
					return word_start;
			}
		}
	}

	return NO_BREAK_NEEDED;
}

unsigned int SpriteFont :: getBreakPoint (const string& str, unsigned int width, unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));

	return getBreakPoint(str.c_str(), width, format);
}

vector<string> SpriteFont :: breakString (const char* a_str, unsigned int width) const
{
	assert(isInitalized());
	assert(a_str != NULL);

	return breakString(string(a_str), width, PLAIN);
}

vector<string> SpriteFont :: breakString (const string& str, unsigned int width) const
{
	assert(isInitalized());

	return breakString(str, width, PLAIN);
}

vector<string> SpriteFont :: breakString (const char* a_str, unsigned int width, unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));
	assert(a_str != NULL);

	return breakString(string(a_str), width, format);
}

vector<string> SpriteFont :: breakString (const string& str, unsigned int width, unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));

	vector<string> results;
	string current = str;

	while(true)	// loop drops out below...
	{
		unsigned int break_point = getBreakPoint(current, width, format);

		if(break_point == SpriteFont::NO_BREAK_NEEDED)
		{
			results.push_back(current);
			return results;	// end of loop
		}
		else
		{
			results.push_back(current.substr(0, break_point));
			current = current.substr(break_point);
		}
	}
}




void SpriteFont :: draw (const char* a_str, double x, double y) const
{
	assert(isInitalized());
	assert(a_str != NULL);

	draw(a_str, x, y, 0xFF, 0xFF, 0xFF, PLAIN, 0xFF);
}

void SpriteFont :: draw (const string& str, double x, double y) const
{
	assert(isInitalized());

	draw(str.c_str(), x, y, 0xFF, 0xFF, 0xFF, PLAIN, 0xFF);
}

void SpriteFont :: draw (const char* a_str, double x, double y, unsigned int format) const
{
	assert(isInitalized());
	assert(a_str != NULL);

	draw(a_str, x, y, 0xFF, 0xFF, 0xFF, format, 0xFF);
}

void SpriteFont :: draw (const string& str, double x, double y, unsigned int format) const
{
	assert(isInitalized());

	draw(str.c_str(), x, y, 0xFF, 0xFF, 0xFF, format, 0xFF);
}

void SpriteFont :: draw (const char* a_str, double x, double y, unsigned char red, unsigned char green, unsigned char blue) const
{
	assert(isInitalized());
	assert(a_str != NULL);

	draw(a_str, x, y, red, green, blue, PLAIN, 0xFF);
}

void SpriteFont :: draw (const string& str, double x, double y, unsigned char red, unsigned char green, unsigned char blue) const
{
	assert(isInitalized());

	draw(str.c_str(), x, y, red, green, blue, PLAIN, 0xFF);
}

void SpriteFont :: draw (const char* a_str, double x, double y, unsigned char red, unsigned char green, unsigned char blue, unsigned int format) const
{
	assert(isInitalized());
	assert(a_str != NULL);

	draw(a_str, x, y, red, green, blue, format, 0xFF);
}

void SpriteFont :: draw (const string& str, double x, double y, unsigned char red, unsigned char green, unsigned char blue, unsigned int format) const
{
	assert(isInitalized());

	draw(str.c_str(), x, y, red, green, blue, format, 0xFF);
}

void SpriteFont :: draw (const char* a_str, double x, double y, unsigned char red, unsigned char green, unsigned char blue, unsigned int format, unsigned char alpha) const
{
	assert(isInitalized());
	assert(a_str != NULL);

	char character;
	double bottom;
	double base;
	double left;
	double right;

	double left_coord;
	double right_coord;

	bool is8bitfont = is8Bit();
	int extra_width  = getWidthForFormat(format);
	int slant_amount = getSlantAmountForFormat(format, m_character_height);
	bool bold   = ((format & BOLD)    == BOLD);
	bool mirror = ((format & MIRROR) == MIRROR);

	double end_x = x + getWidth(a_str, format);

	// we need to flip the texture coordinates to draw mirrored text
	if(mirror)
	{
		left_coord  = 1.0;
		right_coord = 0.0;
	}
	else
	{
		left_coord  = 0.0;
		right_coord = 1.0;
	}


	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_POLYGON_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
		glDepthFunc(GL_LEQUAL);
		glDisable(GL_LIGHTING);
		glShadeModel(GL_FLAT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0);
		glDisable(GL_CULL_FACE);
		glEnable(GL_TEXTURE_2D);
		glColor4ub(red, green, blue, alpha);

		bottom = y + m_image_size;

		// we need to start at the right to draw mirrored text
		if(mirror)
			base = end_x;
		else
			base = x;

		character = *a_str;
		while(character != '\0')
		{
			// mirrored text is drawn with squares to the left of our curser
			if(mirror)
				left = base - m_image_size;
			else
				left = base;

			right = left + m_image_size;

			// only draw characters we have loaded
			if(is8bitfont || ((character & 0x80) == 0x00))
			{
				glBindTexture(GL_TEXTURE_2D, ma_character_name[character]);
				glBegin(GL_QUADS);
					glTexCoord2d(left_coord,  1.0); glVertex2d(left  - slant_amount, bottom);
					glTexCoord2d(left_coord,  0.0); glVertex2d(left  + slant_amount, y);
					glTexCoord2d(right_coord, 0.0); glVertex2d(right + slant_amount, y);
					glTexCoord2d(right_coord, 1.0); glVertex2d(right - slant_amount, bottom);
				glEnd();

				// bold text is just normal text twice
				if(bold)
				{
					glBegin(GL_QUADS);
						glTexCoord2d(left_coord,  1.0); glVertex2d(left  - slant_amount + 1, bottom);
						glTexCoord2d(left_coord,  0.0); glVertex2d(left  + slant_amount + 1, y);
						glTexCoord2d(right_coord, 0.0); glVertex2d(right + slant_amount + 1, y);
						glTexCoord2d(right_coord, 1.0); glVertex2d(right - slant_amount + 1, bottom);
					glEnd();
				}
			}

			// mirrored text moves to the left for each character
			if(mirror)
				base -= (ma_character_width[character] + extra_width);
			else
				base += ma_character_width[character] + extra_width;

			a_str++;
			character = *a_str;
		}

		drawLineThrough(x, end_x, y + m_character_height + 1, format & UNDERLINE_BLOCK);
		drawLineThrough(x, end_x, y + (m_character_height * 2) / 3, format & STRIKETHROUGH_BLOCK);

	glPopAttrib();
}

void SpriteFont :: draw (const string& str, double x, double y, unsigned char red, unsigned char green, unsigned char blue, unsigned int format, unsigned char alpha) const
{
	assert(isInitalized());

	draw(str.c_str(), x, y, red, green, blue, format, alpha);
}

void SpriteFont :: draw (const vector<string>& lines, double x, double y) const
{
	assert(isInitalized());

	draw(lines, x, y, 0xFF, 0xFF, 0xFF, PLAIN, 0xFF);
}

void SpriteFont :: draw (const vector<string>& lines, double x, double y, unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));

	draw(lines, x, y, 0xFF, 0xFF, 0xFF, format, 0xFF);
}

void SpriteFont :: draw (const vector<string>& lines, double x, double y, unsigned char red, unsigned char green, unsigned char blue) const
{
	assert(isInitalized());

	draw(lines, x, y, red, green, blue, PLAIN, 0xFF);
}

void SpriteFont :: draw (const vector<string>& lines, double x, double y, unsigned char red, unsigned char green, unsigned char blue, unsigned int format) const
{
	assert(isInitalized());
	assert(isValidFormat(format));

	draw(lines, x, y, red, green, blue, format, 0xFF);
}

void SpriteFont :: draw (const vector<string>& lines, double x, double y, unsigned char red, unsigned char green, unsigned char blue, unsigned int format, unsigned char alpha) const
{
	assert(isInitalized());

	int height = getHeight(format);

	for(unsigned int i = 0; i < lines.size(); i++)
		draw(lines[i], x, y + i * height, red, green, blue, format, alpha);
}



void SpriteFont :: load (const char* a_image)
{
	assert(!isInitalized());
	assert(a_image != NULL);

	load(a_image, 0xFF, 0x00, 0xFF);

	assert(invariant());
}

void SpriteFont :: load (const std::string& image)
{
	assert(!isInitalized());

	load(image.c_str(), 0xFF, 0x00, 0xFF);

	assert(invariant());
}

void SpriteFont :: load (const char* a_image, unsigned char red, unsigned char green, unsigned char blue)
{
	assert(!isInitalized());
	assert(a_image != NULL);
	assert(red != green || red != blue);

	TextureBmp font(a_image);
	unsigned char* a_tile;
	char channel_to_use;
	unsigned char channel_max;

	assert(font.getWidth() >= 16);
	assert(isAPowerOf2(font.getWidth()));
	assert(font.getHeight() == font.getWidth() || font.getHeight() == font.getWidth() / 2);

	//  Calculate channel that gives best detail.
	//    We will use this if the letters overlap
	//    the outside-coloured area.

	if(red <= green && red <= blue)
	{
		channel_max = 255 - red;
		channel_to_use = 'r';
	}
	else if(green <= blue)
	{
		channel_max = 255 - green;
		channel_to_use = 'g';
	}
	else
	{
		channel_max = 255 - blue;
		channel_to_use = 'b';
	}

	if(font.getHeight() == font.getWidth())
		m_character_count = 256;
	else
		m_character_count = 128;
	m_image_size = font.getWidth() / TEXTURES_PER_ROW;

	glGenTextures(m_character_count, ma_character_name);
	a_tile = new unsigned char[m_image_size * m_image_size];
	for(unsigned int i = 0; i < m_character_count; i++)
	{
		unsigned int cell_x = i % TEXTURES_PER_ROW;
		unsigned int cell_y = i / TEXTURES_PER_ROW;

		unsigned int base_x = cell_x * m_image_size;
		unsigned int base_y = cell_y * m_image_size;

		for(unsigned int y = 0; y < m_image_size; y++)
		{
			unsigned int font_y = base_y + y;

			for(unsigned int x = 0; x < m_image_size; x++)
			{
				unsigned int tile_index = y * m_image_size + x;

				unsigned int font_x = base_x + x;

				unsigned char r = font.getRed(font_x, font_y);
				unsigned char g = font.getGreen(font_x, font_y);
				unsigned char b = font.getBlue(font_x, font_y);

				if(r == red && g == green && b == blue)
				{
					// we are in the "outside" area
					a_tile[tile_index] = 0;
				}
				else if(r != g || r != b)
				{
					//  We are in the "outside" area, but have data.  We
					//    want to use the channel that gives us the most
					//    detailed information
					switch(channel_to_use)
					{
					case 'r': a_tile[tile_index] = (unsigned int)r * 255 / channel_max; break;
					case 'g': a_tile[tile_index] = (unsigned int)g * 255 / channel_max; break;
					case 'b': a_tile[tile_index] = (unsigned int)b * 255 / channel_max; break;
					default:  a_tile[tile_index] = 0; break;
					}
				}
				else
				{
					// a nice ordinary greyscale value
					a_tile[tile_index] = r;
				}
			}
		}

		// convert our tile to an OpenGL texture
		glBindTexture(GL_TEXTURE_2D, ma_character_name[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, m_image_size, m_image_size, 0, GL_ALPHA, GL_UNSIGNED_BYTE, a_tile);

		// calculate the character width from the first row
		ma_character_width[i] = 0;
		for(unsigned int x2 = 0; x2 <= m_image_size; x2++)
		{
			unsigned int this_x = base_x + x2;

			if(x2 >= m_image_size ||
			   (font.getRed  (this_x, base_y) == red &&
			    font.getGreen(this_x, base_y) == green &&
			    font.getBlue (this_x, base_y) == blue))
			{
				ma_character_width[i] = x2;
				break;
			}
		}
	}
	delete[] a_tile;

	// calculate the font height from the first column of the first character
	m_character_height = 0;
	for(unsigned int y2 = 0; y2 <= m_image_size; y2++)
	{
		if(y2 >= m_image_size ||
		   (font.getRed  (0, y2) == red &&
		    font.getGreen(0, y2) == green &&
		    font.getBlue (0, y2) == blue))
		{
			m_character_height = y2;
			break;
		}
	}

	// fill in the rest of the array with '0's
	for(unsigned int i2 = m_character_count; i2 < 0x100; i2++)
	{
		ma_character_name[i2] = 0;
		ma_character_width[i2] = 0;
	}

	assert(invariant());
}

void SpriteFont :: load (const string& image, unsigned char red, unsigned char green, unsigned char blue)
{
	assert(!isInitalized());
	assert(red != green || red != blue);

	load(image.c_str(), red, green, blue);

	assert(invariant());
}



//
//  Copy Constructor
//  Assignment Operator
//
//  These functions have intentionally not been implemented.
//
//  SpriteFont (const SpriteFont& original);
//  SpriteFont& operator= (const SpriteFont& original);
//

bool SpriteFont :: invariant () const
{
	if(m_character_count != 0 && m_character_count != 0x80 && m_character_count != 0x100) return false;
	if(m_character_count != 0 && !isAPowerOf2(m_image_size)) return false;
	if(m_character_height > m_image_size) return false;

	for(unsigned int i = 0; i < m_character_count; i++)
	{
		if(!glIsTexture(ma_character_name[i]))
			return false;
		if(ma_character_width[i] > m_image_size)
			return false;
	}

	return true;
}

