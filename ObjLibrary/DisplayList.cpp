//
//  DisplayList.cpp
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
#include <string>	// for NULL

#include "GetGlut.h"
#include "DisplayList.h"



const unsigned int DisplayList :: EMPTY   = 0;
const unsigned int DisplayList :: PARTIAL = 1;
const unsigned int DisplayList :: READY   = 2;



DisplayList :: DisplayList ()
{
	mp_data = NULL;
}

DisplayList :: DisplayList (const DisplayList& original)
{
	assert(original.getState() != PARTIAL);

	mp_data = NULL;
	copy(original);
}

DisplayList& DisplayList :: operator= (const DisplayList& original)
{
	if(&original != this)
	{
		makeEmpty();
		copy(original);
	}

	return *this;
}

DisplayList :: ~DisplayList ()
{
	makeEmpty();
}



unsigned int DisplayList :: getState() const
{
	if(mp_data == NULL)
		return EMPTY;
	else if(mp_data->m_usages == 0)
		return PARTIAL;
	else
		return READY;
}

void DisplayList :: draw () const
{
	assert(getState() == READY);

	glCallList(mp_data->m_list_id);
}



void DisplayList :: makeEmpty ()
{
	if(getState() == PARTIAL)
		end();

	if(getState() == READY)
	{
		assert(mp_data->m_usages > 0);

		mp_data->m_usages--;
		if(mp_data->m_usages == 0)
		{
			glDeleteLists(mp_data->m_list_id, 1);
			delete mp_data;
		}

		mp_data = NULL;
	}

	assert(getState() == EMPTY);
}

void DisplayList :: begin ()
{
	assert(getState() != PARTIAL);

	if(getState() == READY)
		makeEmpty();

	assert(getState() == EMPTY);

	mp_data = new InnerData();
	mp_data->m_usages = 0;
	mp_data->m_list_id = glGenLists(1);

	glNewList(mp_data->m_list_id, GL_COMPILE);

	assert(getState() == PARTIAL);
}

void DisplayList :: end ()
{
	assert(getState() == PARTIAL);

	glEndList();

	assert(mp_data->m_usages == 0);
	mp_data->m_usages = 1;

	assert(getState() == READY);
}



void DisplayList :: copy (const DisplayList& original)
{
	assert(getState() == EMPTY);
	assert(original.getState() != PARTIAL);

	mp_data = original.mp_data;

	if(mp_data != NULL)
		mp_data->m_usages++;
}
