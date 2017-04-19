//
//  MtlLibraryManager.h
//
//  A global service to handle MtlLibraries.
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
#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "ObjStringParsing.h"
#include "MtlLibrary.h"
#include "MtlLibraryManager.h"

using namespace std;
using namespace MtlLibraryManager;
namespace
{
	std::vector<MtlLibrary*> g_mtl_libraries;
	MtlLibrary g_empty;
}



unsigned int MtlLibraryManager :: getCount ()
{
	return g_mtl_libraries.size();
}

MtlLibrary& MtlLibraryManager :: get (unsigned int index)
{
	assert(index < getCount());

	return *(g_mtl_libraries[index]);
}

MtlLibrary& MtlLibraryManager :: get (const char* a_name)
{
	assert(a_name != NULL);

	return get(string(a_name), cerr);
}

MtlLibrary& MtlLibraryManager :: get (const char* a_name, const string& logfile)
{
	assert(a_name != NULL);
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	MtlLibrary* p_library = &(get(string(a_name), logstream));
	logstream.close();

	return *p_library;
}

MtlLibrary& MtlLibraryManager :: get (const char* a_name, ostream& r_logstream)
{
	assert(a_name != NULL);

	return get(string(a_name), r_logstream);
}

MtlLibrary& MtlLibraryManager :: get (const string& name)
{
	return get(name, cerr);
}

MtlLibrary& MtlLibraryManager :: get (const string& name, const string& logfile)
{
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	MtlLibrary* p_library = &(get(name, logstream));
	logstream.close();

	return *p_library;
}

MtlLibrary& MtlLibraryManager :: get (const string& name, ostream& r_logstream)
{
	string lower = toLowercase(name);

	for(unsigned int i = 0; i < g_mtl_libraries.size(); i++)
		if(g_mtl_libraries[i]->getName().compare(lower) == 0)
			return *(g_mtl_libraries[i]);

	if(endsWith(lower, ".mtl"))
		return add(MtlLibrary(name, r_logstream));
	else
		return g_empty;
}

bool MtlLibraryManager :: isLoaded (const char* a_name)
{
	assert(a_name != NULL);

	return isLoaded(string(a_name));
}

bool MtlLibraryManager :: isLoaded (const std::string& name)
{
	string lower = toLowercase(name);

	for(unsigned int i = 0; i < g_mtl_libraries.size(); i++)
		if(g_mtl_libraries[i]->getName().compare(lower) == 0)
			return true;
	return false;
}

MtlLibrary& MtlLibraryManager :: add (const MtlLibrary& mtl_library)
{
	assert(!isLoaded(mtl_library.getName()));

	unsigned int index = g_mtl_libraries.size();
	g_mtl_libraries.push_back(new MtlLibrary(mtl_library));

	return *(g_mtl_libraries[index]);
}

void MtlLibraryManager :: unloadAll ()
{
	for(unsigned int i = 0; i < g_mtl_libraries.size(); i++)
		delete g_mtl_libraries[i];
	g_mtl_libraries.clear();
}

void MtlLibraryManager :: loadDisplayTextures ()
{
	// such simple code for such a powerful command...

	for(unsigned int i = 0; i < g_mtl_libraries.size(); i++)
		g_mtl_libraries[i]->loadDisplayTextures();
}

void MtlLibraryManager :: loadAllTextures ()
{
	// such simple code for such a powerful command...

	for(unsigned int i = 0; i < g_mtl_libraries.size(); i++)
		g_mtl_libraries[i]->loadAllTextures();
}
