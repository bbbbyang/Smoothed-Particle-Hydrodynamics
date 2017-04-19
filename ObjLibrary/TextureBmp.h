//
//  TextureBmp.h
//
//  Encapsulates a module for loading bmp files and converting
//    them to 2D OpenGL textures.
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

#ifndef TEXTURE_BMP_H
#define TEXTURE_BMP_H

#include <string>



//
//  TextureBmp
//
//  A class to represent a .bmp file in memory.  There are
//    functions to load and save a 24-bit TextureBmp, as well as
//    add a TextureBmp to OpenGL video memory.  A TextureBmp can
//    also be loaded from a 32-bit .bmp file with the extra 8
//    bits for each pixel ignored.
//
//  IMPORTANT: Texture coordinates are as follows:
//
//   0        1
// 0 +--------+ 0
//   |        |
//   |        |
//   |        |
//   |        |
// 1 +--------+ 1
//   0        1
//
//  Class Invariant:
//	<1> ma_texture != NULL
//	<2> m_width > 0
//	<3> m_height > 0
//	<4> m_has_alpha ||
//	    m_bytes_per_row == getBytesPerRowNoAlpha(m_width)
//	<5> !m_has_alpha ||
//	    m_bytes_per_row == getBytesPerRowAlpha(m_width)
//	<6> m_array_size == m_bytes_per_row * m_height
//

class TextureBmp
{
public:
//
//  loadImage
//
//  Purpose: To load an image from a 24-/32-bit bitmap file and
//	     add it to OpenGL.
//  Parameter(s):
//	<1> a_filename: A C-string containing the filename of
//			the image to be loaded
//	<1> filename: A string containing the filename of the
//		      image to be loaded
//	<2> invisible_red
//	<3> invisible_green
//	<4> invisible_blue: The colour in the original image to
//			    become invisible.
//  Precondition(s):
//	<1> a_filename != NULL
//  Returns: The OpenGL name for the texture.  If the file does
//	     not exist, 0 is returned.
//  Side Effect: N/A
//

	static unsigned int loadImage (const char* a_filename);
	static unsigned int loadImage (
				   const std::string& filename);

	static unsigned int loadImage (
				const char* a_filename,
				unsigned char invisible_red,
				unsigned char invisible_green,
				unsigned char invisible_blue);
	static unsigned int loadImage (
				const std::string& filename,
				unsigned char invisible_red,
				unsigned char invisible_green,
				unsigned char invisible_blue);

//
//  loadImageArray
//
//  Purpose: To load an array of images from a single 24-/32-bit
//	     bitmap file and add each of them to OpenGL as a
//	     seperate texture.
//  Parameter(s):
//	<1> a_filename: A C-string containing the filename of
//			the image to be loaded
//	<1> filename: A string containing the filename of the
//		      image to be loaded
//	<2> textures_x
//	<3> textures_y: The number of textures in the images
//			along each axis
//	<4> texture_width
//	<5> texture_height: The dimensions of the textures
//	<6> texture_spacing_x
//	<7> texture_spacing_y: The space between the textures
//	<8> a_names: An array to fill with the OpenGL names for
//		     the textures
//	<9> invisible_red
//     <10> invisible_green
//     <11> invisible_blue: The colour in the original image to
//			    become invisible.
//  Precondition(s):
//	<1> a_filename != NULL
//	<2> textures_x != 0
//	<3> textures_y != 0
//	<4> textures_width != 0
//	<5> textures_height != 0
//	<6> The specified textures do not exceed the bounds of
//	    the image in the file
//	<7> a_names is large enought to hold all the texture
//	    names generated
//  Returns: N/A
//  Side Effect: The OpenGL names for the textures are stored
//		 in a_names.  If the file does not exist,
//		 a_names is filled with 0s.
//  Texture Name Orginaization:
//	+---+---+---+
//	| 0 | 1 | 2 |
//	+---+---+---+
//	| 3 | 4 | 5 |
//	+---+---+---+
//	| 6 | 7 | 8 |
//	+---+---+---+
//

	static void loadImageArray (
				const char* a_filename,
				unsigned int textures_x,
				unsigned int textures_y,
				unsigned int texture_width,
				unsigned int texture_height,
				unsigned int texture_spacing_x,
				unsigned int texture_spacing_y,
				unsigned int a_names[]);
	static void loadImageArray (
				const std::string& filename,
				unsigned int textures_x,
				unsigned int textures_y,
				unsigned int texture_width,
				unsigned int texture_height,
				unsigned int texture_spacing_x,
				unsigned int texture_spacing_y,
				unsigned int a_names[]);
	static void loadImageArray (
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
				unsigned char invisible_blue);
	static void loadImageArray (
				const std::string& filename,
				unsigned int textures_x,
				unsigned int textures_y,
				unsigned int texture_width,
				unsigned int texture_height,
				unsigned int texture_spacing_x,
				unsigned int texture_spacing_y,
				unsigned int a_names[],
				unsigned char invisible_red,
				unsigned char invisible_green,
				unsigned char invisible_blue);

public:
//
//  Default Constructor
//
//  Purpose: To create a blank white TextureBmp.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new TextureBmp is created representing a
//		 white square 1x1 pixels in size.  The new
//		 TextureBmp is not added to OpenGL.
//

	TextureBmp ();

//
//  Constructor
//
//  Purpose: To create a blank TextureBmp of the specified
//	     dimensions.
//  Parameter(s):
//	<1> width
//	<2> height: The dimensions of the texture to be
//		    generated
//	<3> has_alpha: Whether or not the texture includes an
//		       alpha channel
//  Precondition(s):
//	<1> width > 0
//	<2> height > 0
//  Returns: N/A
//  Side Effect: A blank TextureBmp is created.
//

	TextureBmp (unsigned int width,
		    unsigned int height,
		    bool has_alpha);

//
//  Constructor
//
//  Purpose: To create a TextureBmp from a 24-/32-bit bitmap
//	     file.
//  Parameter(s):
//	<1> a_filename: A C-string containing the filename of
//			the image to be loaded
//	<1> filename: A string containing the filename of the
//		      image to be loaded
//	<2> logfile: The file to write loading errors to
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s):
//	<1> a_filename != NULL
//	<2> logfile.compare("") != 0
//  Returns: N/A
//  Side Effect: If file a_filename/filename exists and is a
//		 valid 24-bit BMP file, the file contents are
//		 loaded into memory.  If a logfile or logging
//		 stream is specified, any loading errors are
//		 written to that file or stream.  Otherwise, any
//		 loading errors are written to the standard
//		 error stream.  The texture is not added to
//		 OpenGL.
//

	TextureBmp (const char* a_filename);
	TextureBmp (const char* a_filename,
		    const std::string& logfile);
	TextureBmp (const char* a_filename,
		    std::ostream& r_logstream);
	TextureBmp (const std::string& filename);
	TextureBmp (const std::string& filename,
		    const std::string& logfile);
	TextureBmp (const std::string& filename,
		    std::ostream& r_logstream);

//
//  Constructor
//
//  Purpose: To create a TextureBmp from an array.
//  Parameter(s):
//	<1> width
//	<2> height: The dimensions of the texture to be
//		    generated
//	<3> has_alpha: Whether or not the texture includes an
//		       alpha channel
//	<4> array_size: The number of elements in the array
//	<5> a_texture: An array containing the colour data for
//		       the texure
//  Precondition(s):
//	<1> a_texture != NULL
//	<2> width > 0
//	<3> height > 0
//	<4> !has_alpha ||
//	    array_size == getBytesPerRowAlpha(width) * height
//	<5> has_alpha ||
//	    array_size == getBytesPerRowNoAlpha(width) * height
//  Returns: N/A
//  Side Effect: A texture is created from the given array.  It
//		 is not added to OpenGL.  a_texture is assumed
//		 to be a dynamically-allocated array and will be
//		 eventually deallocated by the new TextureBmp.
//		 If has_alpha is true, the elements are assumend
//		 to be in RGBA order.  Oteherwise, the elements
//		 are assumed to be in RGB order and tightly
//		 packed except for padding to the next 32-bit
//		 word at the end of each line.
//

	TextureBmp (unsigned int width,
		    unsigned int height,
		    bool has_alpha,
		    unsigned int array_size,
		    unsigned char a_texture[]);

//
//  Constructor
//
//  Purpose: To create a new TextureBmp duplicating a piece of a
//	     larger texture.
//  Parameter(s):
//	<1> source: The source texture
//	<2> x
//	<3> y: The upper left corner of the section of source to
//	       be copied
//	<4> width
//	<5> height: The dimensions of the section of source to
//		    be copied
//  Precondition(s):
//	<1> width > 0
//	<2> height > 0
//	<3> x + width <= source.getWidth()
//	<4> y + height <= source.getHeight()
//  Returns: N/A
//  Side Effect: A new TextureBmp is created duplicating the
//		 specified area of the source texture.  The new
//		 TextureBmp is not added to OpenGL.
//

	TextureBmp (const TextureBmp& source,
		    unsigned int x,
		    unsigned int y,
		    unsigned int width,
		    unsigned int height);

//
//  Constructor
//
//  Purpose: To create a new TextureBmp with an alpha channel
//	     duplicating a peice of a larger texture without
//	     one.  The specified colour in the new TextureBmp
//	     will be set to have an alpha of 0.0 and the
//	     remainder of the new TextureBmp will be set to have
//	     an alpha of 1.0.
//  Parameter(s):
//	<1> source: The source TextureBmp
//	<2> x
//	<3> y: The upper left corner of the section of source to
//	       be copied
//	<4> width
//	<5> height: The dimensions of the section of source to
//		    be copied
//	<6> invisible_red
//	<7> invisible_green
//	<8> invisible_blue: The colour in source to become
//			    invisible
//  Precondition(s):
//	<1> width > 0
//	<2> height > 0
//	<3> x + width <= source.getWidth()
//	<4> y + height <= source.getHeight()
//	<5> !source.hasAlphaChannel()
//  Returns: N/A
//  Side Effect: A new TextureBmp is created duplicating the
//		 specified area of source with an added alpha
//		 channel.  The new TextureBmp is not added to
//		 OpenGL.
//

	TextureBmp (const TextureBmp& source,
		    unsigned int x,
		    unsigned int y,
		    unsigned int width,
		    unsigned int height,
		    unsigned char invisible_red,
		    unsigned char invisible_green,
		    unsigned char invisible_blue);

//
//  Copy Constructor
//
//  Purpose: To create a TextureBmp duplicating another
//	     TextureBmp.
//  Parameter(s):
//	<1> original: The TextureBmp to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new TextureBmp is created as a deep copy of
//		 original. The new texture is not added to
//		 OpenGL.
//

	TextureBmp (const TextureBmp& original);

//
//  Assignment Operator
//
//  Purpose: To transform this TextureBmp into a deep copy
//	     of another.
//  Parameter(s):
//	<1> original: The TextureBmp to copy
//  Precondition(s): N/A
//  Returns: A reference to this TextureBmp
//  Side Effect: This TextureBmp is changed into a deep copy
//		 of original.
//

	TextureBmp& operator= (const TextureBmp& original);

//
//  Deconstructor
//
//  Purpose: To safely destroy a TextureBmp without memory
//	     leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	~TextureBmp ();

//
//  isBad
//
//  Purpose: To determine if there is something wrong with this
//	     TextureBmp.  A TextureBmp is considered to be bad
//	     if it is the result of a failed load command
//	     (e.g. if the file did not exist).
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this TextureBmp is bad.
//  Side Effect: N/A
//

	bool isBad () const
	{ return m_is_bad; }

//
//  getWidth
//  getHeight
//
//  Purpose: To return width/height of this TextureBmp.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The width/height of this TextureBmp.
//  Side Effect: N/A
//

	unsigned int getWidth () const
	{ return m_width; }
	unsigned int getHeight () const
	{ return m_height; }

//
//  hasAlphaChannel
//
//  Purpose: To return whether this TextureBmp has an alpha
//	     channel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this TextureBmp has an alpha channel.
//  Side Effect: N/A
//

	bool hasAlphaChannel () const
	{ return m_has_alpha; }

//
//  getArray
//
//  Purpose: To return a pointer to the array representing this
//	     TextureBmp.  Modification of this array will modify
//	     this TextureBmp.  The format of this array matches
//	     requirements of glTexImage2D, so it may be
//	     difficult to work with directly.  Use this function
//	     with extreme caution.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: A pointer to the array representing this
//	     TextureBmp.
//  Side Effect: N/A
//

	unsigned char* getArray () const
	{ return md_texture; }

//
//  getRed
//  getGreen
//  getBlue
//
//  Purpose: To determine the colour component of the specified
//	     pixel in this TextureBmp.
//  Parameter(s):
//	<1> x
//	<2> y: The pixel to query
//  Precondition(s):
//	<1> x < getWidth()
//	<2> y < getHeight()
//  Returns: The colour component of the specified pixel.
//  Side Effect: N/A
//

	unsigned char getRed (unsigned int x,
			      unsigned int y) const;
	unsigned char getGreen (unsigned int x,
				unsigned int y) const;
	unsigned char getBlue (unsigned int x,
			       unsigned int y) const;

//
//  getAlpha
//
//  Purpose: To determine the alpha component of the specified
//	     pixel in this TextureBmp.
//  Parameter(s):
//	<1> x
//	<2> y: The pixel to query
//  Precondition(s):
//	<1> x < getWidth()
//	<2> y < getHeight()
//	<3> hasAlphaChannel()
//  Returns: The colour component of the specified pixel.
//  Side Effect: N/A
//

	unsigned char getAlpha (unsigned int x,
				unsigned int y) const;

//
//  getRGB
//
//  Purpose: To determine the colour of the specified pixel in
//	     this Bmp.  The red component is stored in the least
//	     significant bits.
//  Parameter(s):
//	<1> x
//	<2> y: The pixel to query
//  Precondition(s):
//	<1> x < getWidth()
//	<2> y < getHeight()
//  Returns: The colour of pixel (x, y) in RGB(A) format.
//  Side Effect: N/A
//

	unsigned int getRGB (unsigned int x,
			     unsigned int y) const;

//
//  save
//
//  Purpose: To save this TextureBmp as a .bmp file with the
//	     specified name.  The file extension is not
//	     automatically appended.
//  Parameter(s):
//	<1> a_filename: The name of the file as a C-string
//	<1> filename: The name of the file as a string
//  Precondition(s):
//	<1> a_filename != NULL
//	<2> strlen(a_filename) != 0
//	<1> filename.length() != 0
//  Returns: N/A
//  Side Effect: This TextureBmp is saved as a .bmp file named
//		 a_filename.  The alpha channel for this
//		 TextureBmp, if any, is ignored.
//

	void save (const char* a_filename) const;
	void save (const std::string& filename) const;

//
//  init
//
//  Purpose: To replace this TextureBmp with a blank image of
//	     the specified dimensions.
//  Parameter(s):
//	<1> width
//	<2> height: The dimensions of the texture to be
//		    generated
//	<3> has_alpha: Whether or not the texture includes an
//		       alpha channel
//  Precondition(s):
//	<1> width > 0
//	<2> height > 0
//  Returns: N/A
//  Side Effect: The contents of this TextureBmp are replaced by
//		 a blank texture of size (width, height).  The
//		 texture is not added to OpenGL.  Any existing
//		 texture is lost.
//

	void init (unsigned int width,
		   unsigned int height,
		   bool has_alpha);

//
//  load
//
//  Purpose: To replace this TextureBmp with the contents of a
//	     24-bit bitmap file.
//  Parameter(s):
//	<1> a_filename: A C-string containing the filename of
//			the image to be loaded
//	<1> filename: A string containing the filename of the
//		      image to be loaded
//	<2> logfile: The file to write loading errors to
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s):
//	<1> a_filename != NULL
//	<2> logfile.compare("") != 0
//  Returns: N/A
//  Side Effect: If file a_filename/filename exists and is a
//		 valid 24-bit BMP file, the file contents are
//		 loaded into memory.  If a logfile or logging
//		 stream is specified, any loading errors are
//		 written to that file or stream.  Otherwise, any
//		 loading errors are written to the standard
//		 error stream.  The texture is not added to
//		 OpenGL.  Any existing texture is lost.
//

	void load (const char* a_filename);
	void load (const char* a_filename,
		   const std::string& logfile);
	void load (const char* a_filename,
		   std::ostream& r_logstream);
	void load (const std::string& filename);
	void load (const std::string& filename,
		   const std::string& logfile);
	void load (const std::string& filename,
		   std::ostream& r_logstream);

//
//  markBad
//
//  Purpose: To mark this TextureBmp as bad.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This TextureBmp is marked as bad.
//

	void markBad ()
	{ m_is_bad = true; }

//
//  markNotBad
//
//  Purpose: To mark this TextureBmp as not bad.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This TextureBmp is marked as not bad.
//

	void markNotBad ()
	{ m_is_bad = false; }

//
//  mirrorX
//  mirrorY
//
//  Purpose: To mirror this TextureBmp across the x-/y-axis.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This TextureBmp is mirrored across the
//		 x-/y-axis.
//

	void mirrorX ();
	void mirrorY ();

//
//  setPixel
//
//  Purpose: To change the colour of the specified pixel in this
//	     TextureBmp.
//  Parameter(s):
//	<1> x
//	<2> y: The pixel to query
//	<3> red
//	<4> green
//	<5> blue: The colour for this pixel
//  Precondition(s):
//	<1> x < getWidth()
//	<2> y < getHeight()
//	<3> !hasAlphaChannel()
//  Returns: N/A
//  Side Effect: The colour of the specified pixel is set to
//		 (red, green, blue).
//

	void setPixel (unsigned int x,
		       unsigned int y,
		       unsigned char red,
		       unsigned char green,
		       unsigned char blue);

//
//  setPixel
//
//  Purpose: To change the colour and alpha value of the
//	     specified pixel in this TextureBmp.
//  Parameter(s):
//	<1> x
//	<2> y: The pixel to query
//	<3> red
//	<4> green
//	<5> blue: The colour for this pixel
//	<6> alpha: The alpha value for this pixel
//  Precondition(s):
//	<1> x < getWidth()
//	<2> y < getHeight()
//	<3> hasAlphaChannel()
//  Returns: N/A
//  Side Effect: The colour of the specified pixel is set to
//		 (red, green, blue) and the alpha value is set
//		 to alpha.
//

	void setPixel (unsigned int x,
		       unsigned int y,
		       unsigned char red,
		       unsigned char green,
		       unsigned char blue,
		       unsigned char alpha);

//
//  setPixel
//
//  Purpose: To change the colour of the specified pixel in this
//	     TextureBmp.  The red component is assumed to be
//	     stored in the least significant bits.
//  Parameter(s):
//	<1> x
//	<2> y: The pixel to query
//	<3> rgb: The colour for this pixel in RGB(A) format
//  Precondition(s):
//	<1> x < getWidth()
//	<2> y < getHeight()
//  Returns: N/A
//  Side Effect: The colour of the specified pixel is set to
//		 rgb.  Not that rgb is considered to include an
//		 alpha component if this TextureBmp has an alpha
//		 channel.
//

	void setPixel (unsigned int x,
		       unsigned int y,
		       unsigned int rgb);

//
//  addToOpenGL
//
//  Purpose: To add this TextureBmp to OpenGL video memory, so
//	     that it can be displayed on the screen.  Mipmaps
//	     are generated.
//  Parameter(s):
//	<1> wrap: The behaviour of the texture outside of the
//		    range [0, 1)
//	<1> wrap_s:
//	<2> wrap_t: The behaviour of the texture outside of the
//		    range [0, 1) along the x-/y-axis
//	<3> mag_filter: The magnification filter
//	<4> min_filter: The minification filter
//  Precondition(s):
//	<1> wrap == GL_REPEAT || wrap == GL_CLAMP
//	<1> wrap_s == GL_REPEAT || wrap_s == GL_CLAMP
//	<2> wrap_t == GL_REPEAT || wrap_t == GL_CLAMP
//	<3> mag_filter == GL_NEAREST ||
//	    mag_filter == GL_LINEAR
//	<4> min_filter == GL_NEAREST ||
//	    min_filter == GL_LINEAR ||
//	    min_filter == GL_NEAREST_MIPMAP_NEAREST ||
//	    min_filter == GL_NEAREST_MIPMAP_LINEAR ||
//	    min_filter == GL_LINEAR_MIPMAP_NEAREST ||
//	    min_filter == GL_LINEAR_MIPMAP_LINEAR
//  Returns: The OpenGL name for the texture.
//  Side Effect: This texture is added to OpenGL texture memory.
//		 This TextureBmp instance is not needed to use
//		 the texture and, unless it is used elsewhere in
//		 client code, can be safely deleted.
//

	unsigned int addToOpenGL () const;
	unsigned int addToOpenGL (unsigned int wrap) const;
	unsigned int addToOpenGL (unsigned int wrap_s,
				  unsigned int wrap_t) const;
	unsigned int addToOpenGL (unsigned int wrap_s,
				  unsigned int wrap_t,
				  unsigned int mag_filter,
				  unsigned int min_filter)
							  const;

private:
//
//  Helper Function: createDefault
//
//  Purpose: To set this TextureBmp to a default texture.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> md_texture == NULL
//  Returns: N/A
//  Side Effect: This TextureBmp is set to contain a white
//		 texture 1x1 in size with no alpha channel.
//		 Existing dynamically allocated memory is not
//		 freed.
//

	void createDefault ();

//
//  Helper Function: copy
//
//  Purpose: To copy all the values from another TextureBmp into
//	     this TextureBmp.
//  Parameter(s):
//	<1> original: The TextureBmp to copy
//  Precondition(s):
//	<1> md_texture == NULL
//  Returns: N/A
//  Side Effect: All values from original are copied to this
//		 TextureBmp.  Existing dynamically allocated
//		 memory is not freed.
//

	void copy (const TextureBmp& original);

//
//  Helper Function: destroy
//
//  Purpose: To free all dynamically allocated memory for this
//	     TextureBmp.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> md_texture != NULL
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//		 ma_texture is set to NULL.
//

	void destroy ();

//
//  Helper Function: invariant
//
//  Purpose: To determine if the class invariant is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invariant is true.
//  Side Effect: N/A
//

	bool invariant () const;

private:
	bool m_is_bad;
	unsigned int m_width;
	unsigned int m_height;
	unsigned int m_bytes_per_row;
	bool m_has_alpha;
	unsigned int m_array_size;
	unsigned char* md_texture;
};

#endif
