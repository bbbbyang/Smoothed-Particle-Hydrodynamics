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

#ifndef MTL_LIBRARY_MANAGER_H
#define MTL_LIBRARY_MANAGER_H

class MtlLibrary;



namespace MtlLibraryManager
{

//
//  getCount
//
//  Purpose: To deterine the number of material libraries
//	     loaded.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of MtlLibraries loaded.
//  Side Effect: N/A
//

unsigned int getCount ();

//
//  get
//
//  Purpose: To retreive a reference to the material library
//	     with the specified index.
//  Parameter(s):
//	<1> index: Which mtl_library
//  Precondition(s):
//	<1> index < getCount()
//  Returns: A reference to the MtlLibrary with index index.
//  Side Effect: N/A
//

MtlLibrary& get (unsigned int index);

//
//  get
//
//  Purpose: To retreive a reference to the material library
//	     with the specified name.  Name comparisons are
//	     case-insensitive.
//  Parameter(s):
//	<1> a_name: The name of the material library
//	<1> name: The name of the material library
//	<2> logfile: The file to write loading errors to
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s):
//	<1> a_name != NULL
//	<2> logfile.compare("") != 0
//  Returns: A reference to the MtlLibrary with name
//	     a_name/name.  If there is no material library
//	     loaded with name a_name/name, and a_name/name ends
//	     in ".mtl" (case-insensitive), a new MtlLibrary
//	     loaded from a file named a_name/name is returned.
//	     Otherwise an empty material library is returned.
//  Side Effect: If the MtlLibrary has not been loaded, and a
//		 suitable file exists, error messages may be
//		 generated.  If a logfile or logging stream is
//		 specified, any loading errors are written to
//		 that file or stream.  Otherwise, any loading
//		 errors are written to the standard error
//		 stream.  If the MtlLibrary has already been
//		 loaded, no error messages will be printed.
//

MtlLibrary& get (const char* a_name);
MtlLibrary& get (const char* a_name,
		 const std::string& logfile);
MtlLibrary& get (const char* a_name,
		 std::ostream& r_logstream);
MtlLibrary& get (const std::string& name);
MtlLibrary& get (const std::string& name,
		 const std::string& logfile);
MtlLibrary& get (const std::string& name,
		 std::ostream& r_logstream);

//
//  isLoaded
//
//  Purpose: To determine if a material library with the
//	     specified name exists.  Name comparisons are
//	     case-insensitive.
//  Parameter(s):
//	<1> a_name: The name of the material library
//	<1> name: The name of the material library
//  Precondition(s):
//	<1> a_name != NULL
//  Returns: Whether there is a material library with name
//	     a_name/name.
//  Side Effect: N/A
//

bool isLoaded (const char* a_name);
bool isLoaded (const std::string& name);

//
//  add
//
//  Purpose: To add the specified MtlLibrary to the material
//	     library manager.
//  Parameter(s):
//	<1> mtl_library: The material library
//  Precondition(s):
//	<1> !isLoaded(mtl_library.getName())
//  Returns: A reference to the MtlLibrary added.
//  Side Effect: MtlLibrary mtl_library is added to the material
//		 library manager.
//

MtlLibrary& add (const MtlLibrary& mtl_library);

//
//  unloadAll
//
//  Purpose: To remove all material libraries from the material
//	     library manager.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: tetxure.
//  Side Effect: All material libraries are removed from the
//		 material libraries manager.  Textures that
//		 these MtlLibrarys refer to are not removed from
//		 the texture manager.
//

void unloadAll ();

//
//  loadDisplayTextures
//
//  Purpose: To load all textures that are actually used when
//	     displaying any Material in any MtlLibrary in the
//	     material library manager.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All textures that would be needed to display
//		 any Material in any MtlLibrary in the material
//		 library manager are loaded.
//  WARNING: This command can be very expensive in video memory.
//

void loadDisplayTextures ();

//
//  loadAllTextures
//
//  Purpose: To load all textures referenced by any Material in
//	     any MtlLibrary in the matwerial library manager,
//	     regardless of whether they are actually used.  To
//	     conserve video memory, consider using
//	     loadDisplayTextures() instead.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All textures referenced by any Material in any
//		 MtlLibrary in this material library manager are
//		 loaded.
//  WARNING: This command can be very expensive in video memory.
//

void loadAllTextures ();



};	// end of namespace MtlLibraryManager

#endif
