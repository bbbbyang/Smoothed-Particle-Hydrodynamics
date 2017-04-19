//
//  ObjStringParsing.h
//
//  A set of functions used in parsing OBJ files.
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

#ifndef OBJ_STRING_PARSING_H
#define OBJ_STRING_PARSING_H

#include <string>



//
//  nextToken
//
//  Purpose: To determine the index of the next token character
//	     in the specified string.  The next token is here
//	     defined to start with next non-whitespace character
//	     following a whitespace character at or after the
//	     current position.
//  Parameter(s):
//	<1> str: The string to search
//	<2> current: The index to begin searching at
//  Precondition(s): N/A
//  Returns: The index of the beginning of the next token.  If
//	     there is no next token, string::npos is returned.
//  Side Effect: N/A
//

size_t nextToken (const std::string& str, size_t current);

//
//  getTokenLength
//
//  Purpose: To determine the length of the token starting with
//	     the specified character in the specified string.
//	     The token length is here defined as the number of
//	     characters, including the specified character,
//	      before the next whitespace character.
//  Parameter(s):
//	<1> str: The string to search
//	<2> current: The beginning of the token
//  Precondition(s): N/A
//  Returns: The length of the token beginning with current.  If
//	     current is a whitespace character or lies outside
//	     of the string, 0 is returned.
//  Side Effect: N/A
//

size_t getTokenLength (const std::string& str, size_t current);

//
//  nextSlashInToken
//
//  Purpose: To determine the index of the next slash ('/')
//	     character in the current token of the specified
//	     string, and after the specified position.
//  Parameter(s):
//	<1> str: The string to search
//	<2> current: The index to begin searching at
//  Precondition(s): N/A
//  Returns: The index of the next slash in this token.  If
//	     there is no next slash, string::npos is returned.
//  Side Effect: N/A
//

size_t nextSlashInToken(const std::string& str, size_t current);



//
//  toLowercase
//
//  Purpose: To convert the specified string to lowercase.
//  Parameter(s):
//	<1> str: The string to convert
//  Precondition(s): N/A
//  Returns: a_str in lowercase.
//  Side Effect: N/A
//

std::string toLowercase (const std::string& str);

//
//  endsWith
//
//  Purpose: To determine if the specified string ends with the
//	     specified C-string.
//  Parameter(s):
//	<1> str: The string to test
//	<2> end: The end C-string
//  Precondition(s):
//	<1> a_end != NULL
//  Returns: Whether str ends with end.
//  Side Effect: N/A
//

bool endsWith (const std::string& str, const char* a_end);

//
//  endsWith
//
//  Purpose: To determine if the specified string ends with the
//	     specified other string.
//  Parameter(s):
//	<1> str: The string to test
//	<2> end: The end string
//  Precondition(s): N/A
//  Returns: Whether str ends with end.
//  Side Effect: N/A
//

bool endsWith (const std::string& str, const std::string& end);



#endif
