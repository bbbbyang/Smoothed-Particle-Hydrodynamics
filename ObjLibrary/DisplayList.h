//
//  DisplayList.h
//
//  A module to encapsulate an OpenGL display list.
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

#ifndef DISPLAY_LIST_H
#define DISPLAY_LIST_H



//
//  DisplayList
//
//  A wrapper class to encapsulate an OpenGL display list.  A
//    DisplayList can be created, displayed and destroyed.  If a
//    DisplayList is copied, the internal display list will not
//    be copied.  However, the underlying display list will not
//    be destroyed until the last reference is removed.
//
//  A DisplayList can be in one of three states, and this state
//    can be determined with the getState() method.  The states
//    are as follows:
//	<1> empty: No OpenGL display list has be created
//	<2> partial: The OpenGL display list has been partially
//		     specified
//	<3> ready: The OpenGL display list is ready to be used
//
//  A display list is an OpenGL concept that allows a series of
//    OpenGL commands (usually drawing commands) to be stored in
//    and executed from video memory.  This is much faster than
//    repeatedly  running the commands from the CPU (e.g. x100).
//    To use a DisplayList:
//	<1> Create a DisplayList dl
//	<2> dl.begin()
//	<3> Call all the OpenGL commands you want in the list.
//	    These commands will go into the list instead of
//	    executing.
//	<4> dl.end()
//	<5> Call dl.draw() whenever you want to execute those
//	    commands.
//

class DisplayList
{
public:
//
//  EMPTY
//  PARTIAL
//  READY
//
//  These are the constants returned by the getState() method.
//

	static const unsigned int EMPTY;
	static const unsigned int PARTIAL;
	static const unsigned int READY;

public:
//
//  Default Constructor
//
//  Purpose: To create a new empty DisplayList.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new DisplayList is created and marked as
//		 empty.
//

	DisplayList ();

//
//  Copy Constructor
//
//  Purpose: To create a new DisplayList as a copy of another.
//  Parameter(s):
//	<1> original: The DisplayList to copy
//  Precondition(s):
//	<1> original.getState() != DisplayList::PARTIAL
//  Returns: N/A
//  Side Effect: A new display list is created.  If original is
//		 empty, this DisplayList is marked as empty.
//		 Otherwise, if original is ready, this
//		 DisplayList is set to refer to the same OpenGL
//		 display list as original.
//

	DisplayList (const DisplayList& original);


//
//  Assignment Operator
//
//  Purpose: To modify this DisplayList to be a copy of another.
//  Parameter(s):
//	<1> original: The DisplayList to copy
//  Precondition(s):
//	<1> original.getState() != DisplayList::PARTIAL
//  Returns: N/A
//  Side Effect: If original is empty, this DisplayList is
//		 marked as empty.  Otherwise, if original is
//		 ready, this DisplayList is set to refer to the
//		 same OpenGL display list as original.  If this
//		 DisplayList contained the last reference to its
//		 OpenGL display list, that display list is
//		 destroyed.
//

	DisplayList& operator= (const DisplayList& original);

//
//  Destructor
//
//  Purpose: To safely destroy this DisplayList without memory
//	     leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.  If
//		 this DisplayList contains the last reference to
//		 its OpenGL display list, that display list is
//		 destroyed.
//

	~DisplayList ();

//
//  getState
//
//  Purpose: To determine the current state of this DisplayList.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: If this DisplayList is empty, DisplayList::EMPTY is
//	     returned.  If this DisplayList is partially
//	     specified, DisplayList::PARTIAL is returned.
//	     Otherwise, if this DisplayList is fully specified,
//	     DisplayList::READY is returned.
//  Side Effect: N/A
//

	unsigned int getState () const;

//
//  draw
//
//  Purpose: To display the contents of this DisplayList.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> getState() == DisplayList::READY
//  Returns: N/A
//  Side Effect: The OpenGL display list encapsulated by this
//		 DisplayList is called.
//

	void draw () const;

//
//  makeEmpty
//
//  Purpose: To mark this DisplayList as empty.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This DisplayList is marked as empty.  If this
//		 DisplayList contains the last reference to its
//		 OpenGL display list, that display list is
//		 destroyed.
//

	void makeEmpty ();

//
//  begin
//
//  Purpose: To begin specifying this DisplayList.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> getState() != DisplayList::PARTIAL
//  Returns: N/A
//  Side Effect: Specification of this DisplayList is started.
//		 (Almost) all OpenGL commands will be stored in
//		 this DisplayList until end() is called.  For
//		 the complete specification of which commands
//		 are stored in the DisplayList and which are
//		 executed immediately, refer to the OpenGL
//		 offical documentaion.
//

	void begin ();

//
//  end
//
//  Purpose: To end specifying this DisplayList.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> getState() == DisplayList::PARTIAL
//  Returns: N/A
//  Side Effect: Specification of this DisplayList is marked as
//		 completed.
//

	void end ();

private:
//
//  copy
//
//  Purpose: To copy the values of another DisplayList to this
//	     DisplayList.
//  Parameter(s):
//	<1> original: The DisplayList to copy
//  Precondition(s):
//	<1> getState() == DisplayList::EMPTY
//	<2> original.getState() != DisplayList::PARTIAL
//  Returns: N/A
//  Side Effect: If original is empty, this display list is
//		 marked as empty.  Otherwise, this DisplayList
//		 is set to contain the same OpenGL display list
//		 as original.
//

	void copy (const DisplayList& original);

private:
	struct InnerData;

private:
	InnerData* mp_data;

private:
	//
	//  InnerData
	//
	//  A record to store information about an OpenGL
	//    display list.  The list id and a usage count are
	//    stored.  A special value of 0 usages is used to
	//    indicate that the display list is only partially
	//    specified.
	//

	struct InnerData
	{
		unsigned int m_list_id;
		unsigned int m_usages;
	};
};



#endif
