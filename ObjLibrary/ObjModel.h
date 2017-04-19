//
//  ObjModel.h
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

#ifndef OBJ_MODEL_H
#define OBJ_MODEL_H

//
//  By default, models are drawn in fixed-pipeline mode.  There
//    is a more powerful and faster implementation available if
//    your program is runnign shaders.  For this, you will need:
//
//  TODO: Finish this
//
//  The assumed organization of files is as follows:
//    glew.c
//    glew.h
//    ObjLibrary
//      ObjLibrary library files
//    wglew.c
//
//  To enable shader-based display, define the macro
//    OBJ_LIBRARY_SHADER_DISPLAY.
//
//  #define OBJ_LIBRARY_SHADER_DISPLAY



#include <string>
#include <vector>

#include "Vector3.h"
#include "Vector2.h"

class DisplayList;
class Material;
class MtlLibrary;


//
//  ObjModel
//
//  A class to represent a single OBJ format model in memory.
//    An ObjModel is composed of vertices and faces, specified
//    in terms of vertices.  The model can be displayed with
//    the draw() function.
//
//  An OBJ file stores a model as several data elements
//    referenced by index.  There are lists of vertices, texture
//    coordinate pairs, and normals.  These are then referenced
//    by index by point sets, polylines, and faces.  A point set
//    contains only a set vertices, which are displayed as
//    points.  A polyline contains vertex-texture-coordinate
//    pairs, which are connected to form line segments.  A face
//    contains vertex-texture-coordinate-normal triples, which
//    are are displayed as a single face.  The vertices forming
//    a face are assumed to be coplanar and form a convex
//    polygon.
//
//  In an ObjModel, faces are divided into face sets, each of
//    which is displayed with a material.  These correspond to
//    the "usemtl" statements in the OBJ file.
//
//  Class Invariant:
//	<1> m_name.compare("") != 0
//

class ObjModel
{
public:
//
//  NO_TEXTURE_COORDINATES
//
//  A constant used to indicate that there are no texture
//    coordinates associated with a vertex in a polyline or
//    face.
//

	static const unsigned int NO_TEXTURE_COORDINATES;

//
//  NO_NORMAL
//
//  A constant used to indicate that there is no normal vector
//    associated with a vertex in  face.
//

	static const unsigned int NO_NORMAL;

//
//  Class Function: loadDisplayTextures
//
//  Purpose: To load all textures that are actually used when
//	     displaying any Material in any MtlLibrary in the
//	     material library manager.  This includes all
//	     Materials used for all ObjModels.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All textures that would be needed to display
//		 any Material in any MtlLibrary in the material
//		 library manager are loaded.
//  WARNING: This command can be very expensive in video memory.
//

	static void loadDisplayTextures ();

//
//  Class Function: loadAllTextures
//
//  Purpose: To load all textures referenced by any Material in
//	     any MtlLibrary in the matwerial library manager,
//	     regardless of whether they are actually used.  To
//	     conserve video memory, consider using
//	     loadDisplayTextures() instead.  This includes all
//	     Materials used for all ObjModels.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All textures referenced by any Material in any
//		 MtlLibrary in this material library manager are
//		 loaded.
//  WARNING: This command can be very expensive in video memory.
//

	static void loadAllTextures ();

public:
//
//  Default Constructor
//
//  Purpose: To create a new ObjModel that contains no vertices
//	     or faces.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new empty ObjModel is created.
//

	ObjModel ();

//
//  Constructor
//
//  Purpose: To create a new ObjModel from the specified file.
//	     A logfile may be specified as a filename or an
//	     output stream.
//  Parameter(s):
//	<1> filename: The name of the file
//	<2> logfile: The file to write loading errors to
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s):
//	<1> filename.compare("") != 0
//	<2> File filename exists
//	<3> File filename is a valid OBJ file
//	<4> logfile.compare("") != 0
//  Returns: N/A
//  Side Effect: A new ObjModel is created, representing the
//		 model specified by file filename.  If a logfile
//		 or logging stream is specified, any loading
//		 errors are written to that file or stream.
//		 Otherwise, any loading errors are written to
//		 the standard error stream.
//

	ObjModel (const std::string& filename);
	ObjModel (const std::string& filename,
		  const std::string& logfile);
	ObjModel (const std::string& filename,
		  std::ostream& r_logstream);

//
//  Copy Constructor
//
//  Purpose: To create a new ObjModel as a copy of another.
//  Parameter(s):
//	<1> original: The ObjModel to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new ObjModel is created representing the same
//		 model as original.
//

	ObjModel (const ObjModel& original);


//
//  Assignment Operator
//
//  Purpose: To modify this ObjModel to be a copy of another.
//  Parameter(s):
//	<1> original: The ObjModel to copy
//  Precondition(s): N/A
//  Returns: A reference to this ObjModel.
//  Side Effect: This ObjModel is transformed into a deep copy
//		 of original.
//

	ObjModel& operator= (const ObjModel& original);

//
//  Destructor
//
//  Purpose: To safely destroy this ObjModel without memory
//	     leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	~ObjModel ();

//
//  isEmpty
//
//  Purpose: To determine if this ObjModel is empty.  An
//	     ObjModel is said to be empty if it contains no
//	     material libraries, vertexes, texture coordinates,
//	     normals, point sets, lines, or faces.  This is the
//	     state an ObjModel is in after it is created with
//	     the default constructir.  It is also the state of
//	     an ObjModel after the makeEmpty function is called
//	     on it.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this ObjModel is empty.
//  Side Effect: N/A
//

	bool isEmpty () const;

//
//  getName
//
//  Purpose: To determine the name of this ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The name of this ObjModel.
//  Side Effect: N/A
//

	const std::string& getName () const;

//
//  getVertexCount
//
//  Purpose: To determine the number of vertices in this
//	     ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of vertices in this ObjModel.
//  Side Effect: N/A
//

	unsigned int getVertexCount () const;

//
//  getVertexX
//  getVertexY
//  getVertexZ
//
//  Purpose: To determine the x/y/z coordinate of the specified
//	     vertex in this ObjModel.
//  Parameter(s):
//	<1> vertex: Which vertex
//  Precondition(s):
//	<1> vertex < getVertexCount()
//  Returns: The x/y/z coordinate of vertex vertex in this
//	     ObjModel.
//  Side Effect: N/A
//

	double getVertexX (unsigned int vertex) const;
	double getVertexY (unsigned int vertex) const;
	double getVertexZ (unsigned int vertex) const;

//
//  getVertexPosition
//
//  Purpose: To determine the position of the specified vertex
//	     in this ObjModel.
//  Parameter(s):
//	<1> vertex: Which vertex
//  Precondition(s):
//	<1> vertex < getVertexCount()
//  Returns: The position of vertex vertex in this ObjModel.
//  Side Effect: N/A
//

	const Vector3& getVertexPosition
				    (unsigned int vertex) const;

//
//  getTextureCoordinateCount
//
//  Purpose: To determine the number of texture coordinate pairs
//	     in this ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of texture coodinate pairs in this
//	     ObjModel.
//  Side Effect: N/A
//

	unsigned int getTextureCoordinateCount () const;

//
//  getTextureCoordinateU
//  getTextureCoordinateV
//
//  Purpose: To determine u/v component of the specified texture
//	     coordinate pair in this ObjModel.
//  Parameter(s):
//	<1> texture_coordinate: Which texture coordinate pair
//  Precondition(s):
//	<1> texture_coordinate < getTextureCoordinateCount()
//  Returns: The u/v coordinate of texture coodinate pair
//	     texture_coordinate in this ObjModel.
//  Side Effect: N/A
//

	double getTextureCoordinateU
			(unsigned int texture_coordinate) const;
	double getTextureCoordinateV
			(unsigned int texture_coordinate) const;

//
//  getNormalCount
//
//  Purpose: To determine the number of normal vectors in this
//	     ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of normal vectors in this ObjModel.
//  Side Effect: N/A
//

	unsigned int getNormalCount () const;

//
//  getNormalX
//  getNormalY
//  getNormalZ
//
//  Purpose: To determine the x/y/z component of the specified
//	     normal vector in this ObjModel.
//  Parameter(s):
//	<1> normal: Which normal vector
//  Precondition(s):
//	<1> normal < getNormalCount()
//  Returns: The x/y/z component of normal vector normal in this
//	     ObjModel.
//  Side Effect: N/A
//

	double getNormalX (unsigned int normal) const;
	double getNormalY (unsigned int normal) const;
	double getNormalZ (unsigned int normal) const;

//
//  getNormalVector
//
//  Purpose: To retrieve the specified normal vector from this
//	     ObjModel.
//  Parameter(s):
//	<1> normal: Which normal vector
//  Precondition(s):
//	<1> normal < getNormalCount()
//  Returns: Normal vector normal in this ObjModel.
//  Side Effect: N/A
//

	const Vector3& getNormalVector
				    (unsigned int normal) const;

//
//  getPointSetCount
//
//  Purpose: To determine the number of point sets in this
//	     ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of point sets in this ObjModel.
//  Side Effect: N/A
//

	unsigned int getPointSetCount () const;

//
//  getPointSetVertexCount
//
//  Purpose: To determine the number of vertices in a point set
//	     in this ObjModel.
//  Parameter(s):
//	<1> point_set: Which point set
//  Precondition(s):
//	<1> point_set < getPointSetCount()
//  Returns: The number of vertices in point set point_set of
//	     this ObjModel.
//  Side Effect: N/A
//

	unsigned int getPointSetVertexCount
				 (unsigned int point_set) const;

//
//  getPointSetVertexIndex
//
//  Purpose: To determine the index of the specified vertex in
//	     the specified point set in this ObjModel.
//  Parameter(s):
//	<1> point_set: Which point set
//	<2> vertex: Which vertex
//  Precondition(s):
//	<1> point_set < getPointSetCount()
//	<2> vertex < getPointSetVertexCount(point_set)
//  Returns: The index of vertex vertex in point set point_set
//	     in this ObjModel.
//  Side Effect: N/A
//

	unsigned int getPointSetVertexIndex
				(unsigned int point_set,
				 unsigned int vertex) const;

//
//  getLineCount
//
//  Purpose: To determine the number of lines in this ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of lines in this ObjModel.
//  Side Effect: N/A
//

	unsigned int getLineCount () const;

//
//  getLineVertexCount
//
//  Purpose: To determine the number of vertices in the
//	     specified line in this ObjModel.
//  Parameter(s):
//	<1> line: Which line
//  Precondition(s):
//	<1> line < getLineCount()
//  Returns: The number of vertices in line line of this
//	     ObjModel.
//  Side Effect: N/A
//

	unsigned int getLineVertexCount (unsigned int line)
							  const;

//
//  getLineVertexIndex
//
//  Purpose: To determine the index of the specified vertex in
//	     the specified line in this ObjModel.
//  Parameter(s):
//	<1> line: Which line
//	<2> vertex: Which vertex
//  Precondition(s):
//	<1> line < getLineCount()
//	<2> vertex < getLineVertexCount(line)
//  Returns: The index of vertex vertex in line line of this
//	     ObjModel.
//  Side Effect: N/A
//

	unsigned int getLineVertexIndex (unsigned int line,
					 unsigned int vertex)
							  const;

//
//  getLineVertexTextureCoordinates
//
//  Purpose: To determine the index of the texture coordinates
//	     to use when displaying the specified vertex in the
//	     specified line in this ObjModel.
//  Parameter(s):
//	<1> line: Which line
//	<2> vertex: Which vertex
//  Precondition(s):
//	<1> line < getLineCount()
//	<2> vertex < getLineVertexCount(line)
//  Returns: The index of the texture coordinates to use when
//	     displaying vertex vertex in line line of this
//	     ObjModel.
//  Side Effect: N/A
//

	unsigned int getLineVertexTextureCoordinates
				    (unsigned int line,
				     unsigned int vertex) const;

//
//  getFaceSetCount
//
//  Purpose: To determine the number of face sets in this
//	     ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The number of face sets in this ObjModel.
//  Side Effect: N/A
//

	unsigned int getFaceSetCount () const;

//
//  isFaceSetMaterial
//
//  Purpose: To determine if the specified face set in this
//	     ObjModel has an associated material.
//  Parameter(s):
//	<1> face_set: Which face set
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//  Returns: Whether face set face_set has an associated
//	     material.
//  Side Effect: N/A
//

	bool isFaceSetMaterial (unsigned int face_set) const;

//
//  getFaceSetMaterialName
//
//  Purpose: To determine the name of the material used to
//	     display faces in the specifed face set in this
//	     ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> isFaceSetMaterial(face_set)
//  Returns: The name of the material used to diaplay faces in
//	     face set face_set.
//  Side Effect: N/A
//

	const std::string& getFaceSetMaterialName (
				   unsigned int face_set) const;

//
//  isFaceSetAllTriangles
//
//  Purpose: To determine if the specified face set in this
//	     ObjModel contains only triangle faces.
//  Parameter(s):
//	<1> face_set: Which face set
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//  Returns: Whether face set face_set contains only triangle
//	     faces.  If face set face_set contains no faces,
//	     true is returned.
//  Side Effect: N/A
//

	bool isFaceSetAllTriangles(unsigned int face_set) const;

//
//  getFaceCount
//
//  Purpose: To determine the number of faces in the specified
//	     face set of this ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//  Returns: The number of faces in face set face_set of this
//	     ObjModel.
//  Side Effect: N/A
//

	unsigned int getFaceCount (unsigned int face_set) const;

//
//  getFaceVertexCount
//
//  Purpose: To determine the number of vertices in the
//	     specified face in the specified face set of this
//	     ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> face: Which face
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> face < getFaceCount(face_set)
//  Returns: The number of vertices in face face of face set
//	     face_set of this ObjModel.
//  Side Effect: N/A
//

	unsigned int getFaceVertexCount (unsigned int face_set,
					 unsigned int face)
							  const;

//
//  getFaceVertexIndex
//
//  Purpose: To determine the index of the specified vertex in
//	     the specified face in the specified face set of
//	     this ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> face: Which face
//	<3> vertex: Which vertex
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> face < getFaceCount(face_set)
//	<3> vertex < getFaceVertexCount(face_set, face)
//  Returns: The index of vertex vertex in face face of face set
//	     face_set of this ObjModel.
//  Side Effect: N/A
//

	unsigned int getFaceVertexIndex (unsigned int face_set,
					 unsigned int face,
					 unsigned int vertex)
							  const;

//
//  getFaceVertexTextureCoordinates
//
//  Purpose: To determine the index of the texture coordinates
//	     to use when displaying the specified vertex in the
//	     specified face in the specified face set in this
//	     ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> face: Which face
//	<3> vertex: Which vertex
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> face < getFaceCount(face_set)
//	<3> vertex < getFaceVertexCount(face_set, face)
//  Returns: The index of the texture coordinates to use when
//	     displaying vertex vertex in face face in face set
//	     face_set of this ObjModel.
//  Side Effect: N/A
//

	unsigned int getFaceVertexTextureCoordinates (
				     unsigned int face_set,
				     unsigned int line,
				     unsigned int vertex) const;

//
//  getFaceVertexNormal
//
//  Purpose: To determine the index of the normal vector to use
//	     when displaying the specified vertex in the
//	     specified face in the specified face set of this
//	     ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> face: Which face
//	<3> vertex: Which vertex
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> face < getFaceCount(face_set)
//	<3> vertex < getFaceVertexCount(face_set, face)
//  Returns: The index of the normal vector to use when
//	     displaying vertex vertex in face face in face set
//	     face_set of this ObjModel.
//  Side Effect: N/A
//

	unsigned int getFaceVertexNormal (unsigned int face_set,
					  unsigned int face,
					  unsigned int vertex)
							  const;

//
//  getFaceCountTotal
//
//  Purpose: To determine the total number of faces in all face
//	     sets in this ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The total number of faces in this ObjModel.
//  Side Effect: N/A
//

	unsigned int getFaceCountTotal () const;

//
//  isValid
//
//  Purpose: To determine if this ObjModel is valid.  An
//	     ObjModel is valid if no face references a
//	     nonexistant vertex, texture coordinate, or normal
//	     vector.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this ObjModel is valid.
//  Side Effect: N/A
//

	bool isValid () const;

//
//  print
//
//  Purpose: To print the contents of this ObjModel to standard
//	     output.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This ObjModel printed to standard output.
//

	void print () const;

//
//  printMtlLibraries
//
//  Purpose: To print a list of all MTL material libraries that
//	     are referenced by this ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A list of all MTL files used by this ObjModel
//		 is printed to standard output.
//

	void printMtlLibraries () const;

//
//  printBadMaterials
//
//  Purpose: To print a list of all materials that are
//	     referenced by this ObjModel but cannot be loaded.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A list of all invalid materials used by this
//		 ObjModel is printed to standard output.
//

	void printBadMaterials () const;

//
//  draw
//
//  Purpose: To display this ObjModel with OpenGL graphics.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isValid()
//  Returns: N/A
//  Side Effect: This ObjModel is displayed with OpenGL
//		 graphics.
//

	void draw () const;

//
//  drawShaders
///
//  Purpose: To display this ObjModel using OpenGL shaders.  If
//	     OBJ_LIBRARY_SHADER_DISPLAY is not defined, this
//	     function just calls draw().
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isValid()
//  Returns: N/A
//  Side Effect: This ObjModel is displayed with OpenGL shaders.
//

	void drawShaders () const;

//
//  drawVertices
//
//  Purpose: To display the vertices in this ObjModel with
//	     OpenGL graphics with the specified colour.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> red >= 0.0
//	<3> red <= 1.0
//	<4> green >= 0.0
//	<5> green <= 1.0
//	<6> blue >= 0.0
//	<7> blue <= 1.0
//  Returns: N/A
//  Side Effect: The vertices in this ObjModel are displayed
//		 with OpenGL graphics in colour
//		 (red, green, blue).
//

	void drawVertices (unsigned char red,
			   unsigned char green,
			   unsigned char blue) const;

//
//  drawVertices
//
//  Purpose: To display the vertices in this ObjModel with
//	     OpenGL graphics with the specified colour.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> red >= 0x00
//	<3> red <= 0xFF
//	<4> green >= 0x00
//	<5> green <= 0xFF
//	<6> blue >= 0x00
//	<7> blue <= 0xFF
//  Returns: N/A
//  Side Effect: The vertices in this ObjModel are displayed
//		 with OpenGL graphics in colour
//		 (red, green, blue).
//

	void drawVertices (int red, int green, int blue) const;

//
//  drawVertices
//
//  Purpose: To display the vertices in this ObjModel with
//	     OpenGL graphics with the specified colour.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> red >= 0.0
//	<3> red <= 1.0
//	<4> green >= 0.0
//	<5> green <= 1.0
//	<6> blue >= 0.0
//	<7> blue <= 1.0
//  Returns: N/A
//  Side Effect: The vertices in this ObjModel are displayed
//		 with OpenGL graphics in colour
//		 (red, green, blue).
//

	void drawVertices (double red,
			   double green,
			   double blue) const;

//
//  drawWireframe
//
//  Purpose: To display this ObjModel as a wireframe model of
//	     the specified colour with OpenGL graphics.  This
//	     function is slow, and should be used with care.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> red >= 0.0
//	<3> red <= 1.0
//	<4> green >= 0.0
//	<5> green <= 1.0
//	<6> blue >= 0.0
//	<7> blue <= 1.0
//  Returns: N/A
//  Side Effect: This ObjModel is displayed in wireframe using
//		 OpenGL graphics in colour (red, green, blue).
//

	void drawWireframe (unsigned char red,
			    unsigned char green,
			    unsigned char blue) const;

//
//  drawWireframe
//
//  Purpose: To display this ObjModel as a wireframe model of
//	     the specified colour with OpenGL graphics.  This
//	     function is slow, and should be used with care.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> red >= 0x00
//	<3> red <= 0xFF
//	<4> green >= 0x00
//	<5> green <= 0xFF
//	<6> blue >= 0x00
//	<7> blue <= 0xFF
//  Returns: N/A
//  Side Effect: This ObjModel is displayed in wireframe using
//		 OpenGL graphics in colour (red, green, blue).
//

	void drawWireframe (int red, int green, int blue) const;

//
//  drawWireframe
//
//  Purpose: To display this ObjModel as a wireframe model of
//	     the specified colour with OpenGL graphics.  This
//	     function is slow, and should be used with care.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> red >= 0.0
//	<3> red <= 1.0
//	<4> green >= 0.0
//	<5> green <= 1.0
//	<6> blue >= 0.0
//	<7> blue <= 1.0
//  Returns: N/A
//  Side Effect: This ObjModel is displayed in wireframe using
//		 OpenGL graphics in colour (red, green, blue).
//

	void drawWireframe (double red,
			    double green,
			    double blue) const;

//
//  drawSolid
//
//  Purpose: To display this ObjModel as the specified solid
//	     colour.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The colour
//  Precondition(s):
//	<1> isValid()
//  Returns: N/A
//  Side Effect: This ObjModel is displayed in solid colour
//		 (red, green, blue).
//

	void drawSolid (unsigned char red,
			unsigned char green,
			unsigned char blue) const;

//
//  drawSolid
//
//  Purpose: To display this ObjModel as the specified solid
//	     colour.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> red >= 0x00
//	<3> red <= 0xFF
//	<4> green >= 0x00
//	<5> green <= 0xFF
//	<6> blue >= 0x00
//	<7> blue <= 0xFF
//  Returns: N/A
//  Side Effect: This ObjModel is displayed in solid colour
//		 (red, green, blue).
//

	void drawSolid (int red, int green, int blue) const;

//
//  drawSolid
//
//  Purpose: To display this ObjModel as the specified solid
//	     colour.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> red >= 0.0
//	<3> red <= 1.0
//	<4> green >= 0.0
//	<5> green <= 1.0
//	<6> blue >= 0.0
//	<7> blue <= 1.0
//  Returns: N/A
//  Side Effect: This ObjModel is displayed in solid colour
//		 (red, green, blue).
//

	void drawSolid (double red,
			double green,
			double blue) const;

//
//  drawMaterial
//
//  Purpose: To display this ObjModel with the specified
//	     material using OpenGL graphics.
//  Parameter(s):
//	<1> material: The material to use
//  Precondition(s):
//	<1> isValid()
//  Returns: N/A
//  Side Effect: This ObjModel is displayed with material
//		 material using OpenGL graphics.
//

	void drawMaterial (const Material& material) const;

//
//  drawMaterialNone
//
//  Purpose: To display this ObjModel without any materials
//	     using OpenGL graphics.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isValid()
//  Returns: N/A
//  Side Effect: This ObjModel is displayed using OpenGL
//		 graphics as if it had no materials.
//

	void drawMaterialNone () const;

//
//  drawMaterials
//
//  Purpose: To display this ObjModel with the specified
//	     materials using OpenGL graphics.
//  Parameter(s):
//	<1> materials: A vector<> of the materials to use
//  Precondition(s):
//	<1> isValid()
//	<2> materials.size() == getFaceSetCount()
//  Returns: N/A
//  Side Effect: This ObjModel is displayed using OpenGL
//		 graphics.  Each face set is displayed with the
//		 corresponding material in materials.  For
//		 example, face set 0 is displayed with
//		 materials[0], face set 1 is displayed with
//		 materials[1], and so on.  If materials contains
//		 NULL values, the corresponding face set is
//		 displayed with whatever material was most
//		 recently active.  This is normally the most
//		 recent non-NULL material in materials.
//

	void drawMaterials (
		 const std::vector<Material*>& materials) const;

//
//  drawMtlLibrary
//
//  Purpose: To display this ObjModel with the materials in the
//	     specified MtlLibrary.
//  Parameter(s):
//	<1> library: The MtlLibrary to use
//  Precondition(s):
//	<1> isValid()
//  Returns: N/A
//  Side Effect: This ObjModel is displayed using OpenGL
//		 graphics.  Each face set is displayed with the
//		 material in library with the corresponding
//		 name.  If there is no such material, the
//		 material specified in this ObjModel is used.
//

	void drawMtlLibrary (const MtlLibrary& library) const;

//
//  drawMtlLibrary
//
//  Purpose: To display this ObjModel with the materials in the
//	     specified MtlLibrary.
//  Parameter(s):
//  Parameter(s):
//	<1> a_name: The name of the material library
//	<1> name: The name of the material library
//  Precondition(s):
//	<1> a_name != NULL
//  Returns: N/A
//  Side Effect: This ObjModel is displayed using OpenGL
//		 graphics.  Each face set is displayed with the
//		 material in MTL library a_name/name with the
//		 corresponding name.  If the MTL library cannot
//		 be loaded, or if there is no such material, the
//		 material specified in this ObjModel is used.
//

	void drawMtlLibrary (const char* a_name) const;
	void drawMtlLibrary (const std::string& name) const;

//
//  drawVertexNormals
//
//  Purpose: To display the vertex normals for this ObjModel.
//  Parameter(s):
//	<1> length: The length of the vertex normals
//	<2> red
//	<3> green
//	<4> blue: The colour for the normals
//  Precondition(s):
//	<1> isValid()
//	<2> length > 0.0
//  Returns: N/A
//  Side Effect: The vertex normals for this ObjModel are
//		 displayed with length length in colour
//		 (red, green, blue).  This does not include
//		 displaying this ObjModel itself.
//

	void drawVertexNormals (double length,
				unsigned char red,
				unsigned char green,
				unsigned char blue) const;

//
//  drawVertexNormals
//
//  Purpose: To display the vertex normals for this ObjModel.
//  Parameter(s):
//	<1> length: The length of the vertex normals
//	<2> red
//	<3> green
//	<4> blue: The colour for the normals
//  Precondition(s):
//	<1> isValid()
//	<2> length > 0.0
//	<4> red <= 0xFF
//	<5> green >= 0x00
//	<6> green <= 0xFF
//	<7> blue >= 0x00
//	<8> blue <= 0xFF
//  Returns: N/A
//  Side Effect: The vertex normals for this ObjModel are
//		 displayed with length length in colour
//		 (red, green, blue).  This does not include
//		 displaying this ObjModel itself.
//

	void drawVertexNormals (double length,
				int red,
				int green,
				int blue) const;

//
//  drawVertexNormals
//
//  Purpose: To display the vertex normals for this ObjModel.
//  Parameter(s):
//	<1> length: The length of the vertex normals
//	<2> red
//	<3> green
//	<4> blue: The colour for the normals
//  Precondition(s):
//	<1> isValid()
//	<2> length > 0.0
//	<3> red >= 0.0
//	<4> red <= 1.0
//	<5> green >= 0.0
//	<6> green <= 1.0
//	<7> blue >= 0.0
//	<8> blue <= 1.0
//  Returns: N/A
//  Side Effect: The vertex normals for this ObjModel are
//		 displayed with length length in colour
//		 (red, green, blue).  This does not include
//		 displaying this ObjModel itself.
//

	void drawVertexNormals (double length,
				double red,
				double green,
				double blue) const;

//
//  drawFaceNormals
//
//  Purpose: To display the face normals for this ObjModel.  The
//	     normal for a face is the normalized average for the
//	     normals of its vertices.
//  Parameter(s):
//	<1> length: The length of the face normals
//	<2> red
//	<3> green
//	<4> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> length > 0.0
//  Returns: N/A
//  Side Effect: The face normals for this ObjModel are
//		 displayed with length length in colour
//		 (red, green, blue).  If a face normal is the
//		 zero vector, that face normal is not displayed.
//		 This ObjModel is not displayed.
//

	void drawFaceNormals (double length,
			      unsigned char red,
			      unsigned char green,
			      unsigned char blue) const;

//
//  drawFaceNormals
//
//  Purpose: To display the face normals for this ObjModel.  The
//	     normal for a face is the normalized average for the
//	     normals of its vertices.
//  Parameter(s):
//	<1> length: The length of the face normals
//	<2> red
//	<3> green
//	<4> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> length > 0.0
//	<3> red >= 0x00
//	<4> red <= 0xFF
//	<5> green >= 0x00
//	<6> green <= 0xFF
//	<7> blue >= 0x00
//	<8> blue <= 0xFF
//  Returns: N/A
//  Side Effect: The face normals for this ObjModel are
//		 displayed with length length in colour
//		 (red, green, blue).  If a face normal is the
//		 zero vector, that face normal is not displayed.
//		 This ObjModel is not displayed.
//

	void drawFaceNormals (double length,
			      int red,
			      int green,
			      int blue) const;

//
//  drawFaceNormals
//
//  Purpose: To display the face normals for this ObjModel.  The
//	     normal for a face is the normalized average for the
//	     normals of its vertices.
//  Parameter(s):
//	<1> length: The length of the face normals
//	<2> red
//	<3> green
//	<4> blue: The colour
//  Precondition(s):
//	<1> isValid()
//	<2> length > 0.0
//	<3> red >= 0.0
//	<4> red <= 1.0
//	<5> green >= 0.0
//	<6> green <= 1.0
//	<7> blue >= 0.0
//	<8> blue <= 1.0
//  Returns: N/A
//  Side Effect: The face normals for this ObjModel are
//		 displayed with length length in colour
//		 (red, green, blue).  If a face normal is the
//		 zero vector, that face normal is not displayed.
//		 This ObjModel is not displayed.
//

	void drawFaceNormals (double length,
			      double red,
			      double green,
			      double blue) const;

//
//  getDisplayList()
//
//  Purpose: To generate a DisplayList for this ObjModel.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isValid()
//  Returns: A displayList for this ObjModel, with the current
//	     enviroment settings.
//  Side Effect: N/A
//

	DisplayList getDisplayList () const;

//
//  save
//
//  Purpose: To write the contents of this ObjModel to the
//	     specified file.  A logfile may be specified as a
//	     filename or an output stream.
//  Parameter(s):
//	<1> filename: The name of the file to create
//	<2> logfile: The file to write loading errors to
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s):
//	<1> filename.compare("") != 0
//	<2> logfile.compare("") != 0
//  Returns: N/A
//  Side Effect: A file named filename is created.  If a file by
//		 that name already exists, its contantes are
//		 lost.  This ObjModel written to that file in
//		 OBJ format.  If a logfile or logging stream is
//		 specified, any saving errors are written to
//		 that file or stream.  Otherwise, any loading
//		 errors are written to the standard error
//		 stream.
//

	void save (const std::string& filename) const;
	void save (const std::string& filename,
		   const std::string& logfile) const;
	void save (const std::string& filename,
		   std::ostream& r_logstream) const;

//
//  makeEmpty
//
//  Purpose: To replace this ObjModel with an empty model.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This ObjModel is set to represent an empty
//		 model.  The existing model is lost.
//

	void makeEmpty ();

//
//  load
//
//  Purpose: To replace this ObjModel with the model specified
//	     in the specified file.  A logfile may be specified
//	     as a filename or an output stream.
//  Parameter(s):
//	<1> filename: The name of the file containing the model
//	<2> logfile: The file to write loading errors to
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s):
//	<1> filename.compare("") != 0
//	<2> File filename exists
//	<3> File filename is a valid OBJ file
//	<4> logfile.compare("") != 0
//  Returns: N/A
//  Side Effect: This ObjModel is set to represent the ObjModel
//		 specified in the file filename.  The existing
//		 model, if any, is lost.  If a logfile or
//		 logging stream is specified, any loading errors
//		 are written to that file or stream.  Otherwise,
//		 any loading errors are written to the standard
//		 error stream.
//

	void load (const std::string& filename);
	void load (const std::string& filename,
		   const std::string& logfile);
	void load (const std::string& filename,
		   std::ostream& r_logstream);

//
//  setName
//
//  Purpose: To change the name of this ObjModel.
//  Parameter(s):
//	<1> name: The new name
//  Precondition(s):
//	<1> name.compare("") != 0
//  Returns: N/A
//  Side Effect: This ObjModel is set to have name name.
//

	void setName (const std::string& name);

//
//  setVertexCount
//
//  Purpose: To change the number of vertices in this ObjModel.
//  Parameter(s):
//	<1> count: The new number of vertices
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This ObjModel is set contain count vertices.
//		 If count is greater than the current number of
//		 vertices, additional vertices are added at
//		 position (0, 0, 0).  If count is less than the
//		 current number of vertices, the vertices with
//		 the highest indices are lost and this model is
//		 marked as invalid.
//

	void setVertexCount (unsigned int count);

//
//  setVertexX
//  setVertexY
//  setVertexZ
//
//  Purpose: To change the x/y/z-coordinate of the specified
//	     vertex in this ObjModel.
//  Parameter(s):
//	<1> vertex: Which vertex
//	<2> x
//	<2> y
//	<2> x: The new x/y/z coordinate
//  Precondition(s):
//	<1> vertex < getVertexCount()
//  Returns: N/A
//  Side Effect: Vertex vertex in this ObjModel is set be at
//		 have a x/y/z-coordinate of x/y/z.
//

	void setVertexX (unsigned int vertex, double x);
	void setVertexY (unsigned int vertex, double y);
	void setVertexZ (unsigned int vertex, double z);

//
//  setVertexPosition
//
//  Purpose: To change the position of the specified vertex in
//	     this ObjModel.
//  Parameter(s):
//	<1> vertex: Which vertex
//	<2> x
//	<3> y
//	<4> x: The new coordinates
//  Precondition(s):
//	<1> vertex < getVertexCount()
//  Returns: N/A
//  Side Effect: Vertex vertex in this ObjModel is set be at
//		 position (x, y, z).
//

	void setVertexPosition (unsigned int vertex,
				double x, double y, double z);

//
//  setVertexPosition
//
//  Purpose: To change the position of the specified vertex in
//	     this ObjModel.
//  Parameter(s):
//	<1> vertex: Which vertex
//	<2> position: The new vertex position
//  Precondition(s):
//	<1> vertex < getVertexCount()
//  Returns: N/A
//  Side Effect: Vertex vertex in this ObjModel is set be at
//		 position position.
//

	void setVertexPosition (unsigned int vertex,
				const Vector3& position);

//
//  setTextureCoordinateCount
//
//  Purpose: To change the number of texture coordinate pairs in
//	     this ObjModel.
//  Parameter(s):
//	<1> count: The new number of texture coordinate pairs
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This ObjModel is set contain count texture
//		 coordinate pairs. If count is greater than the
//		 current number of texture ccordinate pairs,
//		 additional texture coordinate pairs are added
//		 with a value of (0, 0).  If count is less than
//		 the current number of texture coordinate pairs,
//		 the texture coordinate pairs with the highest
//		 indices are lost and this model is marked as
//		 invalid.
//

	void setTextureCoordinateCount (unsigned int count);

//
//  setTextureCoordinateU
//  setTextureCoordinateV
//
//  Purpose: To change the u/v component of the specified
//	     texture coordinate pair in this ObjModel.
//  Parameter(s):
//	<1> texture_coordinate: Which texture coordinate pair
//	<2> u
//	<2> v: The new u/v component
//  Precondition(s):
//	<1> texture_coordinate < getTextureCoordinateCount()
//  Returns: N/A
//  Side Effect: Texture coordinate pair texture_coordinate in
//		 this ObjModel is set be at have a
//		 u/v-coordinate of u/v.
//

	void setTextureCoordinateU
			(unsigned int texture_coordinate,
			 double u);
	void setTextureCoordinateV
			(unsigned int texture_coordinate,
			 double v);

//
//  setTextureCoordinate
//
//  Purpose: To change the components of the specified texture
//	     coordinate pair in this ObjModel.
//  Parameter(s):
//	<1> vertex: Which vertex
//	<1> texture_coordinate: Which texture coordinate pair
//	<2> u
//	<3> v: The new u/v components
//  Precondition(s):
//	<1> texture_coordinate < getTextureCoordinateCount()
//  Returns: N/A
//  Side Effect: Texture coordinate pair texture_coordinate in
//		 this ObjModel is set be at have a value of
//		 (u, v).
//

	void setTextureCoordinate
			(unsigned int texture_coordinate,
			 double u, double v);

//
//  setNormalCount
//
//  Purpose: To change the number of normal vectors in this
//	     ObjModel.
//  Parameter(s):
//	<1> count: The new number of normal vectors
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This ObjModel is set contain count normal
//		 vectors.  If count is greater than the current
//		 number of normal vectors, additional normal
//		 vectors with a value of (0, 0, 1) are added.
//		 If count is less than the current number of
//		 normal vectors, the normal vectors with the
//		 highest indices are lost and this model is
//		 marked as invalid.
//

	void setNormalCount (unsigned int count);

//
//  setNormalX
//
//  Purpose: To change the x-coordinate of the specified normal
//	     vector in this ObjModel.
//  Parameter(s):
//	<1> normal: Which normal vector
//	<2> x: The new x coordinate
//  Precondition(s):
//	<1> normal < getNormalCount()
//	<2> x != 0.0 ||
//	    getNormalY(normal) != 0.0 ||
//	    getNormalZ(normal) != 0.0
//  Returns: N/A
//  Side Effect: Normal vector normal in this ObjModel is set to
//		 have an x-coordinate of x.  Normal vector
//		 normal is than scaled to have a length of 1.0.
//

	void setNormalX (unsigned int normal, double x);

//
//  setNormalY
//
//  Purpose: To change the y-coordinate of the specified normal
//	     vector in this ObjModel.
//  Parameter(s):
//	<1> normal: Which normal vector
//	<2> y: The new y coordinate
//  Precondition(s):
//	<1> normal < getNormalCount()
//	<2> getNormalX(normal) != 0.0 ||
//	    y != 0.0 ||
//	    getNormalZ(normal) != 0.0
//  Returns: N/A
//  Side Effect: Normal vector normal in this ObjModel is set to
//		 have an y-coordinate of y.  Normal vector
//		 normal is than scaled to have a length of 1.0.
//

	void setNormalY (unsigned int normal, double y);

//
//  setNormalZ
//
//  Purpose: To change the z-coordinate of the specified normal
//	     vector in this ObjModel.
//  Parameter(s):
//	<1> normal: Which normal vector
//	<2> z: The new z coordinate
//  Precondition(s):
//	<1> normal < getNormalCount()
//	<2> getNormalX(normal) != 0.0 ||
//	    getNormalY(normal) != 0.0 ||
//	    z != 0.0
//  Returns: N/A
//  Side Effect: Normal vector normal in this ObjModel is set to
//		 have an z-coordinate of z.  Normal vector
//		 normal is than scaled to have a length of 1.0.
//

	void setNormalZ (unsigned int normal, double z);

//
//  setNormalVector
//
//  Purpose: To change the value of the specified normal vector
//	     in this ObjModel.
//  Parameter(s):
//	<1> normal: Which normal vector
//	<2> x
//	<3> y
//	<4> x: The new normal vector
//  Precondition(s):
//	<1> normal < getNormalCount()
//	<2> x != 0.0 || y != 0.0 || z != 0.0
//  Returns: N/A
//  Side Effect: Normal vector normal in this ObjModel is set
//		 have a value of (x, y, z).  The new normal
//		 vector is scaled to have a length of 1.0.
//

	void setNormalVector (unsigned int normal,
			      double x, double y, double z);

//
//  setNormalVector
//
//  Purpose: To change the value of the specified normal vector
//	     in this ObjModel.
//  Parameter(s):
//	<1> normal: Which normal vector
//	<2> vector: The new normla vector
//  Precondition(s):
//	<1> normal < getNormalCount()
//	<2> !vector.isZero()
//  Returns: N/A
//  Side Effect: Normal vector normal in this ObjModel is set
//		 have a value of vector.  The new normal vector
//		 is scaled to have a length of 1.0.
//

	void setNormalVector (unsigned int normal,
			      const Vector3& vector);

//
//  setPointSetVertexIndex
//
//  Purpose: To change the index of the vertex refered to by the
//	     specified vertex in the specified point set in this
//	     ObjModel.
//  Parameter(s):
//	<1> point_set: Which point set
//	<2> vertex: Which vertex
//	<3> index: The index of the new vertex
//  Precondition(s):
//	<1> point_set < getPointSetCount()
//	<2> vertex < getPointSetVertexCount(point_set)
//  Returns: N/A
//  Side Effect: Vertex vertex in point set point_set in this
//		 ObjModel is set to refer to the vertex with
//		 index index.  If index >= getVertexCount(),
//		 this ObjModel is marked as invalid.
//

	void setPointSetVertexIndex (unsigned int point_set,
				     unsigned int vertex,
				     unsigned int index);

//
//  setLineVertexIndex
//
//  Purpose: To change the index of the vertex refered to by the
//	     specified vertex in the specified line in this
//	     ObjModel.
//  Parameter(s):
//	<1> line: Which line
//	<2> vertex: Which vertex
//	<3> index: The index of the new vertex
//  Precondition(s):
//	<1> line < getLineCount()
//	<2> vertex < getLineVertexCount(line)
//  Returns: N/A
//  Side Effect: Vertex vertex in line line in this ObjModel is
//		 set to refer to the vertex with index index.
//		 If index >= getVertexCount(), this ObjModel is
//		 marked as invalid.
//

	void setLineVertexIndex (unsigned int line,
				 unsigned int vertex,
				 unsigned int index);

//
//  setLineVertexTextureCoordinates
//
//  Purpose: To change the index of the texture coordinate pair
//	     refered to by the specified vertex in the specified
//	     line in this ObjModel.
//  Parameter(s):
//	<1> line: Which line
//	<2> vertex: Which vertex
//	<3> index: The index of the new texture coordinate pair
//  Precondition(s):
//	<1> line < getLineCount()
//	<2> vertex < getLineVertexCount(line)
//  Returns: N/A
//  Side Effect: Vertex vertex in line line in this ObjModel is
//		 set to refer to the texture coordinate pair
//		 with index index.  If
//		 index >= getTextureCoordinateCount(), this
//		 ObjModel is marked as invalid.
//

	void setLineVertexTextureCoordinates
					(unsigned int line,
					 unsigned int vertex,
					 unsigned int index);

//
//  setFaceSetMaterial
//
//  Purpose: To change the material used to display the faces in
//	     the specified face set in this ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> material: The name of the new material
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> material.compare("") != 0
//  Returns: N/A
//  Side Effect: The faces in face set face_set in this ObjModel
//		 are set to be displayed with the material with
//		 the name material.
//

	void setFaceSetMaterial (unsigned int face_set,
				 const std::string& material);

//
//  setFaceSetMaterialNone
//
//  Purpose: To mark the faces in the specified face set in this
//	     ObjModel as not having a material to be displayed
//	     with set.
//  Parameter(s):
//	<1> face_set: Which face set
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//  Returns: N/A
//  Side Effect: The faces in face set face_set in this ObjModel
//		 are set to be displayed without a material.
//

	void setFaceSetMaterialNone (unsigned int face_set);

//
//  setFaceVertexIndex
//
//  Purpose: To change the index of the vertex refered to by the
//	     specified vertex in the specified face in the
//	     specified face set of this ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> face: Which face
//	<3> vertex: Which vertex
//	<4> index: The index of the new vertex
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> face < getFaceCount(face_set)
//	<3> vertex < getFaceVertexCount(face_set, face)
//  Returns: N/A
//  Side Effect: Vertex vertex in face face in face set face_set
//		 of this ObjModel is set to refer to the vertex
//		 with index index.  If
//		 index >= getVertexCount(), this ObjModel is
//		 marked as invalid.
//

	void setFaceVertexIndex (unsigned int face_set,
				 unsigned int face,
				 unsigned int vertex,
				 unsigned int index);

//
//  setFaceVertexTextureCoordinates
//
//  Purpose: To change the index of the texture coordinate pair
//	     refered to by the specified vertex in the specified
//	     face in the specified face_set of this ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> face: Which face
//	<3> vertex: Which vertex
//	<4> index: The index of the new texture coordinate pair
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> face < getFaceCount(face_set)
//	<3> vertex < getFaceVertexCount(face_set, face)
//  Returns: N/A
//  Side Effect: Vertex vertex in face face in face set face_set
//		 of  this ObjModel is set to refer to the
//		 texture coordinate pair with index index.  If
//		 index >= getTextureCoordinateCount(), this
//		 ObjModel is marked as invalid.
//

	void setFaceVertexTextureCoordinates
					(unsigned int face_set,
					 unsigned int face,
					 unsigned int vertex,
					 unsigned int index);

//
//  setFaceVertexNormal
//
//  Purpose: To change the index of the normal vector referenced
//	     by the specified vertex in the specified face in
//	     the specified face set of this ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> face: Which face
//	<3> vertex: Which vertex
//	<4> index: The index of the new normal
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> face < getFaceCount(face_set)
//	<3> vertex < getFaceVertexCount(face_set, face)
//  Returns: N/A
//  Side Effect: Vertex vertex in face face in face set face_set
//		 of this ObjModel is set to refer to the normal
//		 vector with index index.  If
//		 index >= getNormalCount(), this ObjModel is
//		 marked as invalid.
//

	void setFaceVertexNormal (unsigned int face_set,
				  unsigned int face,
				  unsigned int vertex,
				  unsigned int index);

//
//  addMaterialLibary
//
//  Purpose: To add a material library to this ObjModel.
//  Parameter(s):
//	<1> library: The name of the library file
//	<2> logfile: The file to write loading errors to
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s):
//	<1> library.compare("") != 0
//	<2> logfile.compare("") != 0
//  Returns: N/A
//  Side Effect: libary is added to the list of files where this
//		 ObjModel will look for materials.  If a logfile
//		 or logging stream is specified, any loading
//		 errors are written to that file or stream.
//		 Otherwise, any loading errors are written to
//		 the standard error stream.
//

	void addMaterialLibrary (const std::string& library);
	void addMaterialLibrary (const std::string& library,
				 const std::string& logfile);
	void addMaterialLibrary (const std::string& library,
				 std::ostream& r_logstream);

//
//  addVertex
//
//  Purpose: To add a vertex to this ObjModel.
//  Parameter(s):
//	<1> x
//	<2> y
//	<3> z: The coordinates of the vertex
//  Precondition(s): N/A
//  Returns: The index of the new vertex.
//  Side Effect: A vertex at (x, y, z) is added to this
//		 ObjModel.
//

	unsigned int addVertex (double x, double y, double z);

//
//  addVertex
//
//  Purpose: To add a vertex to this ObjModel.
//  Parameter(s):
//	<1> position: The coordinates of the vertex
//  Precondition(s): N/A
//  Returns: The index of the new vertex.
//  Side Effect: A vertex at position is added to this
//		 ObjModel.
//

	unsigned int addVertex (const Vector3& position);

//
//  addTextureCoordinate
//
//  Purpose: To add a texture coordinate pair to this ObjModel.
//  Parameter(s):
//	<1> u
//	<2> v: The texture coordinates
//  Precondition(s): N/A
//  Returns: The index of the new texture coordinates.
//  Side Effect: A texture coordinate pair of (u, v) is added to
//		 this ObjModel.
//

	unsigned int addTextureCoordinate (double u, double v);

//
//  addTextureCoordinate
//
//  Purpose: To add a texture coordinate pair to this ObjModel.
//  Parameter(s):
//	<1> texture_coordinates: The texture coordinates
//  Precondition(s): N/A
//  Returns: The index of the new texture coordinates.
//  Side Effect: A texture coordinate of texture_coordinates is
//		 added to this ObjModel.
//

	unsigned int addTextureCoordinate (
			    const Vector2& texture_coordinates);

//
//  addNormal
//
//  Purpose: To add a normal vector to this ObjModel.
//  Parameter(s):
//	<1> x
//	<2> y
//	<3> z: The components of the normal vector
//  Precondition(s):
//	<1> x != 0.0 || y != 0.0 || z != 0.0
//  Returns: The index of the new normal vector.
//  Side Effect: A normal vector of value (x, y, z) is added to
//		 this ObjModel.  The new normal vector is scaled
//		 to have a length of 1.0.
//

	unsigned int addNormal (double x, double y, double z);

//
//  addNormal
//
//  Purpose: To add a normal vector to this ObjModel.
//  Parameter(s):
//	<1> normal: The normal vector
//  Precondition(s):
//	<1> !normal.isZero()
//  Returns: The index of the new normal vector.
//  Side Effect: A normal vector of size (x, y, z) is added to
//		 this ObjModel.  The new normal vector is scaled
//		 to have a length of 1.0.
//

	unsigned int addNormal (const Vector3& normal);

//
//  addPointSet
//
//  Purpose: To add a new point set to this ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The index of the new point set.
//  Side Effect: A new point set with no vertices is added to
//		 this ObjModel and this ObjModel is marked as
//		 invalid.
//

	unsigned int addPointSet ();

//
//  addPointSetVertex
//
//  Purpose: To add a vertex to a point set in this ObjModel.
//  Parameter(s):
//	<1> point_set: The index of the face
//	<2> vertex: The index of the vertex
//  Precondition(s):
//	<1> point_set < getPointSetCount()
//	<2> vertex >= 1
//  Returns: N/A
//  Side Effect: A point with the coordinates specified by
//		 vertex is added to face face.  If vertex refers
//		 to a non-existant vertex this ObjModel is
//		 marked as invalid.
//

	unsigned int addPointSetVertex (unsigned int point_set,
					unsigned int vertex);

//
//  addLine
//
//  Purpose: To add a new line to this ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The index of the new line.
//  Side Effect: A new line with no vertices is added to this
//		 ObjModel and this ObjModel is marked as
//		 invalid.
//

	unsigned int addLine ();

//
//  addLineVertex
//
//  Purpose: To add a vertex to a line in this ObjModel.
//  Parameter(s):
//	<1> line: The index of the line
//	<2> vertex: The index of the vertex
//	<3> texture_coordinates: The index of the texture
//				 coordinate pair
//  Precondition(s):
//	<1> line < getLineCount()
//	<2> vertex >= 1
//	<3> texture_coordinates >= 1 ||
//	    texture_coordinates ==
//				ObjMOdel::NO_TEXTURE_COORDINATES
//  Returns: N/A
//  Side Effect: A vertex with the coordinates specified by
//		 vertex and the texture coordinates specified by
//		 texture_coordinates is added to face face.  If
//		 vertex refers to a non-existant vertex or
//		 texture_coordinate refers to an non-existant
//		 texture coordinate this ObjModel is marked as
//		 invalid.
//

	unsigned int addLineVertex
			     (unsigned int line,
			      unsigned int vertex,
			      unsigned int texture_coordinates);

//
//  addFaceSet
//
//  Purpose: To add a new face set to this ObjModel.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The index of the new face set.
//  Side Effect: A new face set with no material and containing
//		 no faces is added to this ObjModel.
//

	unsigned int addFaceSet ();

//
//  addFace
//
//  Purpose: To add a new face to the specified face set of this
//	     ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//  Returns: The index of the new face.
//  Side Effect: A new face with no vertices is added to face
//		 set face_set of this ObjModel and this ObjModel
//		 is marked as invalid.
//

	unsigned int addFace (unsigned int face_set);

//
//  addFaceVertex
//
//  Purpose: To add a vertex to a face in this ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> face: The index of the face
//	<3> vertex: The index of the vertex
//	<4> texture_coordinates: The index of the texture
//				 coordinate pair
//	<4> normal: The index of the normal vector
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> face < getFaceCount(face_set)
//	<3> vertex >= 1
//	<4> texture_coordinates >= 1 ||
//	    texture_coordinates ==
//				ObjMOdel::NO_TEXTURE_COORDINATES
//	<5> normal >= 1 || normal == ObjMOdel::NO_NORMAL
//  Returns: N/A
//  Side Effect: A vertex with the coordinates specified by
//		 vertex, the texture coordinates specified by
//		 texture_coordinates, and the normal vector
//		 specified by normal is added to face face in
//		 face set face_set.  If vertex refers to a
//		 non-existant vertex, texture_coordinate refers
//		 to an non-existant texture coordinate, or
//		 normal refers to a non-existant normal vector,
//		 this ObjModel is marked as invalid.
//

	unsigned int addFaceVertex (
			       unsigned int face_set,
			       unsigned int face,
			       unsigned int vertex,
			       unsigned int texture_coordinates,
			       unsigned int normal);

//
//  validate
//
//  Purpose: To recalculate whether this ObjModel is valid.
//	     This function should be called once after modifying
//	     a model.  Do not call it after each change, it is
//	     too slow.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The validity of this model is recalculated.
//		 This will never result in a model marked as
//		 valid being marked as invalid.
//

	void validate ();

private:
//
//  drawPointSets
//  drawLines
//
//  Purpose: To display the point sets/polylines for this
//	     ObjModel.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isValid()
//  Returns: N/A
//  Side Effect: The point sets/polylines are displayed with
//		 OpenGL graphics.
//

	void drawPointSets () const;
	void drawLines () const;

//
//  drawFaceSet
//
//  Purpose: To display all the faces in the specified face sets
//	     with OpenGL graphics.
//  Parameter(s):
//	<1> face_set: Which face set
//  Precondition(s):
//	<1> isValid()
//	<2> face_set < getFaceSetCount()
//  Returns: N/A
//  Side Effect: The faces in face set face_set in this ObjModel
//		 are displayed.
//

	void drawFaceSet (unsigned int face_set) const;

//
//  drawFaceSetMaterial
//
//  Purpose: To display all the faces in the specified face sets
//	     with the specified Material using OpenGL graphics.
//  Parameter(s):
//	<1> face_set: Which face set
//	<2> p_material: A pointer to the material
//  Precondition(s):
//	<1> isValid()
//	<2> face_set < getFaceSetCount()
//	<3> !Material::isMaterialActive()
//  Returns: N/A
//  Side Effect: The faces in face set face_set in this ObjModel
//		 are displayed using p_material.  If p_material
//		 is NULL, the current material is used
//

	void drawFaceSetMaterial (
			      unsigned int face_set,
			      const Material* p_material) const;

//
//  readMaterialLibrary
//
//  Purpose: To add the material libaries corresponding to the
//	     information in a string to this ObjModel.
//  Parameter(s):
//	<1> str: The string containing the material libraries
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s): N/A
//  Returns: Whether the string specifies one or more material
//	     libaries.
//  Side Effect: If string str specifies one or more material
//		 libraries, those material libraries are added
//		 to the end of the list this ObjModel checks
//		 when searching for a material.  Otherwise,
//		 there is no effect.
//

	bool readMaterialLibrary (const std::string& str,
				  std::ostream& r_logstream);

//
//  readMaterial
//
//  Purpose: To set the current material for this ObjModel
//	     corresponding to the information in a string.
//  Parameter(s):
//	<1> str: The string containing the material name
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s): N/A
//  Returns: Whether the string specifies a material.
//  Side Effect: If string str specifies a material, that
//		 material is set to be the current material for
//		 this ObjModel.  Otherwise, there is no effect.
//

	bool readMaterial (const std::string& str,
			   std::ostream& r_logstream);

//
//  readVertex
//
//  Purpose: To add a vertex to this ObjModel corresponding to
//	     the information in a string.
//  Parameter(s):
//	<1> str: The string containing the vertex information
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s): N/A
//  Returns: Whether the string specifies a vertex.
//  Side Effect: If string str specifies a vertex, that vertex.
//		 is added to this ObjModel.  Otherwise, there is
//		 no effect.
//

	bool readVertex (const std::string& str,
			 std::ostream& r_logstream);

//
//  readTextureCoordinates
//
//  Purpose: To add a pair of texture coordinates to this
//	     ObjModel corresponding to the information in a
//	     string.
//  Parameter(s):
//	<1> str: The string containing the texture coordinate
//		 information
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s): N/A
//  Returns: Whether the string specifies a pair of texture
//	     coordinates.
//  Side Effect: If string str specifies a pair of texture
//		 coordinates, that pair is added to this
//		 ObjModel.  Otherwise, there is no effect.
//

	bool readTextureCoordinates (const std::string& str,
				     std::ostream& r_logstream);

//
//  readNormal
//
//  Purpose: To add a normal vector to this ObjModel
//	     corresponding to the information in a string.
//  Parameter(s):
//	<1> str: The string containing the normal vector
//		 information
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s): N/A
//  Returns: Whether the string specifies a normal vector.
//  Side Effect: If string str specifies a normal vector, that
//		 normal vector is added to this ObjModel.
//		 Otherwise, there is no effect.
//

	bool readNormal (const std::string& str,
			 std::ostream& r_logstream);

//
//  readPointSet
//  readLine
//  readFace
//
//  Purpose: To add a point set/line/face to this ObjModel
//	     corresponding to the information in a string.
//  Parameter(s):
//	<1> str: The string containing the point set/line/face
//		 information
//	<2> r_logstream: The stream to write loading errors to
//  Precondition(s): N/A
//  Returns: Whether the string specifies a point set/line/face.
//  Side Effect: If string str specifies a point set/line/face,
//		 that point set/line/face is added to this
//		 ObjModel and this ObjModel is marked as
//		 invalid.  Otherwise, there is no effect.
//

	bool readPointSet (const std::string& str,
			   std::ostream& r_logstream);
	bool readLine (const std::string& str,
		       std::ostream& r_logstream);
	bool readFace (const std::string& str,
		       std::ostream& r_logstream);

//
//  removeLastPointSet
//  removeLastLine
//
//  Purpose: To remove the most recently added
//	     point set/line from this ObjModel.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> getPointSetCount() >= 1
//	<1> getLineCount() >= 1
//  Returns: N/A
//  Side Effect: The most recently added point set/line is
//		 removed from this ObjModel.  This ObjModel is
//		 marked as invalid.
//

	void removeLastPointSet ();
	void removeLastLine ();

//
//  removeLastFace
//
//  Purpose: To remove the most recently added face from the
//	     specified face set of this ObjModel.
//  Parameter(s):
//	<1> face_set: Which face set
//  Precondition(s):
//	<1> face_set < getFaceSetCount()
//	<2> getFaceCount(face_set) >= 1
//  Returns: N/A
//  Side Effect: The most recently added face is removed from
//		 face set face_set of this ObjModel.  This
//		 ObjModel is marked as invalid.
//

	void removeLastFace (unsigned int face_set);

//
//  invariant
//
//  Purpose: To determine if the class invariant is true.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether the class invariant is true.
//  Side Effect: N/A
//

	bool invariant () const;

private:
	struct MaterialLibrary;
	struct PointSet;
	struct LineVertex;
	struct Line;
	struct FaceVertex;
	struct Face;
	struct FaceSet;

private:
	std::vector<MaterialLibrary> m_material_libraries;
	std::vector<Vector3> m_vertices;
	std::vector<Vector2> m_texture_coordinates;
	std::vector<Vector3> m_normals;
	std::vector<PointSet> m_point_sets;
	std::vector<Line> m_lines;
	std::vector<FaceSet> m_face_sets;

	std::string m_name;
	bool m_valid;

private:
	//
	//  MaterialLibrary
	//
	//  A record to represent a material library.  A
	//    MaterialLibrary contains a name and a pointer.  If
	//    the corresponding material library cannot be found
	//    (e.g. because the MTL file does not exist), the
	//    pointer should be set to NULL.
	//

	struct MaterialLibrary
	{
		MaterialLibrary ();
		MaterialLibrary (const std::string& name,
				 std::ostream& r_logstream);
		MaterialLibrary
			      (const MaterialLibrary& original);
		MaterialLibrary& operator=
			      (const MaterialLibrary& original);

		std::string m_name;
		MtlLibrary* mp_mtl_library;
	};

	//
	//  PointSet
	//
	//  A record to represent a set of points in an
	//    ObjModel.  An PointSet contains the indices of
	//    the vertices to be treated as points.
	//

	struct PointSet
	{
		std::vector<unsigned int> m_vertices;
	};

	//
	//  LineVertex
	//
	//  A record to represent a vertex of a polyline in an
	//    ObjModel.  An LineVertex contains the index of the
	//    vertex indicating the position of this point on
	//    the polyline and the index of the texture
	//    coordinates used to texture this point on the
	//    polyline.  If there are no texture coordinates at
	//    this vertex of the polyline, the
	//    texture_coordinates field should be set to
	//    ObjMOdel::NO_TEXTURE_COORDINATES.
	//

	struct LineVertex
	{
		LineVertex ();
		LineVertex (unsigned int vertex,
			    unsigned int texture_coordinates);
		LineVertex (const LineVertex& original);
		LineVertex& operator=
				   (const LineVertex& original);

		unsigned int m_vertex;
		unsigned int m_texture_coordinates;
	};

	//
	//  Line
	//
	//  A record to represent a polyline in an ObjModel.  An
	//    Line contains the indices of the vertices that
	//    form the polyline.
	//

	struct Line
	{
		std::vector<LineVertex> m_vertices;
	};

	//
	//  FaceVertex
	//
	//  A record to represent a vertex of a face in an
	//    ObjModel.  An FaceVertex contains the index of the
	//    vertex indicating the position of this point on
	//    the polyline, the index of the texture coordinates
	//    used to texture this point on the face, and the
	//    index of a normal vector for this point on the
	//    face.  If there are no texture coordinates at this
	//    vertex of the face, the texture_coordinates field
	//    should be set to ObjModel::NO_TEXTURE_COORDINATES.
	//    If there is no normal vector at this vertex in the
	//    face, the normal field should be set to
	//    ObjModel::NO_NORMAL.
	//

	struct FaceVertex
	{
		FaceVertex ();
		FaceVertex (unsigned int vertex,
			    unsigned int texture_coordinates,
			    unsigned int normal);
		FaceVertex (const FaceVertex& original);
		FaceVertex& operator=
				   (const FaceVertex& original);

		unsigned int m_vertex;
		unsigned int m_texture_coordinates;
		unsigned int m_normal;
	};

	//
	//  Face
	//
	//  A record to represent a face in an ObjModel.  A Face
	//    contains the indices of the vertices that form
	//    the outline of the face.
	//

	struct Face
	{
		std::vector<FaceVertex> m_vertices;
	};

	//
	//  FaceSet
	//
	//  A record to represent a group of faces in an 
	//    ObjModel that all use the same material.  A
	//    FaceSet contains a pointer to the Material and the
	//    faces to be drawn with it.  If no material has
	//    been specified, the Material pointer should be set
	//    to NULL and the material name to the empty string.
	//

	struct FaceSet
	{
		FaceSet ();
		FaceSet (const std::string& material_name,
			 Material* p_material);
		FaceSet (const FaceSet& original);
		FaceSet& operator= (const FaceSet& original);

		std::string m_material_name;
		Material* mp_material;
		std::vector<Face> m_faces;
		bool m_all_triangles;
	};
};

#endif

