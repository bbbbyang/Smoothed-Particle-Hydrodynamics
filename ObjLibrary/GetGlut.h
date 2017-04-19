//
//  GetGlut.h
//
//  A header file in import OpenGL, GLU, and GLUT on Windows,
//    Mac OSX, Linux, or Solaris.  The intermediate file is
//    helpful because differant includes are needed on
//    differant platforms.
//
//  On Windows you also get <string> because the GLUT library
//    may not compile unless a standard library (not including
//    <cassert>) is included first.
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

#ifndef GET_GLUT_H
#define GET_GLUT_H


// Unix
#ifdef unix
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#elif __unix
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#elif __unix__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Linux
#elif linux
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#elif __linux
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#elif __linux__
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Mac OSX
#elif __APPLE__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#elif __MACH__
#include <OpenGL/gl.h>
#include <GLUT/glut.h>

// Solaris
#elif sun
OpenGL Includes for solaris
#elif __sun
OpenGL Includes for solaris

// FreeBSD
#elif __FreeBSD__
OpenGL Includes for FreeBSD

// Windows
#elif _WIN32	// include 64-bit enviroments
#include <string>	// without this, glut crashes
#include "glut.h"	// http://www.xmission.com/~nate/glut.html
#elif __WIN32__
#include <string>
#include "glut.h"
#endif



#endif
