//
//  Texture.h
//
//  A module to encapsulate an OpenGL texture.
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

#ifndef TEXTURE_H
#define TEXTURE_H



//
//  Texture
//
//  A wrapper class to encapsulate an OpenGL texture.  A
//    Texture can be created, displayed and destroyed.  If a
//    Texture is copied, the internal texture will not be
//    copied.  However, the underlying texture will not be
//    destroyed until the last reference is removed.
//
//  Class Invariant:
//	<1> mp_data == NULL || mp_data->m_texture_name != 0
//	<2> mp_data == NULL || mp_data->m_usages >= 1
//

class Texture
{
public:
//
//  Default Constructor
//
//  Purpose: To create a new Texture without an associated
//	     texture in video memory.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Texture is created.
//

	Texture ();

//
//  Constructor
//
//  Purpose: To create a new Texture refering to the specified
//	     texture in video memory.
//  Parameter(s):
//	<1> name: The OpenGL texture name
//  Precondition(s):
//	<1> name != 0
//  Returns: N/A
//  Side Effect: A new Texture is created refering to the
//		 texture in video memory with OpenGL texture
//		 name name.
//

	Texture (unsigned int name);

//
//  Copy Constructor
//
//  Purpose: To create a new Texture as a copy of another.
//  Parameter(s):
//	<1> original: The Texture to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Texture is created.  If original refers
//		 to a texture in video memory, the new
//		 DisplayList refers to the same texture.
//		 Otherwise the new Texture does not refer to a
//		 texture in video memory.
//

	Texture (const Texture& original);


//
//  Assignment Operator
//
//  Purpose: To modify this Texture to be a copy of another.
//  Parameter(s):
//	<1> original: The Texture to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: If original refers to a texture in video
//		 memory, this DisplayList refers to the same
//		 texture.  Otherwise this Texture does not refer
//		 to a texture in video memory.  If this Texture
//		 contained the last reference to its video
//		 memory texture, that texture is destroyed.
//

	Texture& operator= (const Texture& original);

//
//  Destructor
//
//  Purpose: To safely destroy this Texture without memory
//	     leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.  If
//		 this Texture contains the last reference to
//		 its video memory texture, that texture is
//		 destroyed.
//

	~Texture ();

//
//  isSet
//
//  Purpose: To determine if this Texture refers to a teture in
//	     video memory.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether there is a texture in video memory that
//	     this Texture refers to.
//  Side Effect: N/A
//

	bool isSet () const;

//
//  getOpenGLName
//
//  Purpose: To determine the OpenGL name for the texture in
//	     video memory refered to by this Texture.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSet()
//  Returns: The OpenGL texture name for the internal texture.
//  Side Effect: N/A
//

	unsigned int getOpenGLName () const;

//
//  activate
//
//  Purpose: To set this texture as the current OpenGL texture.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSet()
//  Returns: N/A
//  Side Effect: This texture is activated.  If GL_TEXTURE_2D is
//		 enabled, the texture in video memory that this
//		 Texture refers to will be used to display
//		 primitives.
//

	void activate () const;

//
//  Equality Test Operator
//
//  Purpose: To determine if another Texture references the same
//	     video memory as this Texture.
//  Parameter(s):
//	<1> other: The Texture to test against.
//  Precondition(s): N/A
//  Returns: Whether this Texture and other refer to the same
//	     texture in video memory.  If either or both
//	     Textures have no associated memory, false is
//	     returned.
//  Side Effect: N/A
//

	bool operator== (const Texture& other) const;

//
//  set
//
//  Purpose: To mark this Texture as refer to the teture in
//	     video memory with the specified OpenGL texture
//	     name.
//  Parameter(s):
//	<1> name: The OpenGL texture name
//  Precondition(s):
//	<1> name != 0
//  Returns: N/A
//  Side Effect: This Texture is marked as refering to the
//		 texture in video memory with OpenGL texture
//		 name name.
//

	void set (unsigned int name);

//
//  setNone
//
//  Purpose: To mark this Texture as not refering to a teture in
//	     video memory.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Texture is marked as refering to nothing.
//		 If this Texture contains the last reference to
//		 its video memory texture, that texture is
//		 destroyed.
//

	void setNone ();

private:
//
//  Helper Function: copy
//
//  Purpose: To copy the values of another Texture to this
//	     Texture.
//  Parameter(s):
//	<1> original: The Texture to copy
//  Precondition(s):
//	<1> !isSet()
//  Returns: N/A
//  Side Effect: If original is refers to a texture in video
//		 memory, this Texture is marked as refering to
//		 the same texture.  Otherwise, this Texture is
//		 marked as not refering to a texture in video
//		 memory.
//

	void copy (const Texture& original);

//
//  Helper Function: invariant
//
//  Purpose: To determine if the class invariant is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: Whether the class invariant is true.
//

	bool invariant () const;

private:
	struct InnerData;

private:
	InnerData* mp_data;

private:
	//
	//  InnerData
	//
	//  A record to store information about a texture in
	//    video memory.  The OpenGL texture name and a usage
	//    count are stored.
	//

	struct InnerData
	{
		unsigned int m_texture_name;
		unsigned int m_usages;
	};
};



#endif
