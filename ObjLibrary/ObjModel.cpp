//
//  ObjModel.cpp
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
#include <cstdlib>	// for atoi
#include <string>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <vector>

#include "GetGlut.h"
#include "ObjStringParsing.h"
#include "DisplayList.h"
#include "Material.h"
#include "MtlLibrary.h"
#include "MtlLibraryManager.h"
#include "ObjModel.h"

using namespace std;
namespace
{
	//
	//  Is this gaurenteed to be initialized before it is
	//    needed?  That would include either of:
	//    -> at compile time
	//    -> at run time, but before any function in this
	//       file is executed.
	//
	const char* DEFAULT_NAME = "Unnamed";

	const bool DEBUGGING = false;
	const bool DEBUGGING_VERTEX_BUFFER = false;
}



const unsigned int ObjModel :: NO_TEXTURE_COORDINATES = 0xFFFFFFFF;
const unsigned int ObjModel :: NO_NORMAL = 0xFFFFFFFF;

void ObjModel :: loadDisplayTextures ()
{
	MtlLibraryManager::loadDisplayTextures();
}

void ObjModel :: loadAllTextures ()
{
	MtlLibraryManager::loadAllTextures();
}



ObjModel :: ObjModel () : m_material_libraries(), m_vertices(), m_texture_coordinates(), m_normals(), m_point_sets(), m_lines(), m_face_sets()
{
	m_name = DEFAULT_NAME;
	m_valid = true;

	assert(isEmpty());
	assert(invariant());
}

ObjModel :: ObjModel (const string& filename) : m_material_libraries(), m_vertices(), m_texture_coordinates(), m_normals(), m_point_sets(), m_lines(), m_face_sets()
{
	m_name = DEFAULT_NAME;
	m_valid = true;

	load(filename);
	// load calls validate()

	assert(invariant());
}

ObjModel :: ObjModel (const string& filename, const string& logfile) : m_material_libraries(), m_vertices(), m_texture_coordinates(), m_normals(), m_point_sets(), m_lines(), m_face_sets()
{
	m_name = DEFAULT_NAME;
	m_valid = true;

	load(filename, logfile);
	// load calls validate()

	assert(invariant());
}

ObjModel :: ObjModel (const string& filename, ostream& r_logstream) : m_material_libraries(), m_vertices(), m_texture_coordinates(), m_normals(), m_point_sets(), m_lines(), m_face_sets()
{
	m_name = DEFAULT_NAME;
	m_valid = true;

	load(filename, r_logstream);
	// load calls validate()

	assert(invariant());
}

ObjModel :: ObjModel (const ObjModel& original) : m_material_libraries(original.m_material_libraries), m_vertices(original.m_vertices), m_texture_coordinates(original.m_texture_coordinates), m_normals(original.m_normals), m_point_sets(original.m_point_sets), m_lines(original.m_lines), m_face_sets(original.m_face_sets)
{
	m_name = original.m_name;
	m_valid = original.m_valid;

	assert(invariant());
}

ObjModel& ObjModel :: operator= (const ObjModel& original)
{
	if(&original != this)
	{
		m_material_libraries = original.m_material_libraries;
		m_vertices = original.m_vertices;
		m_texture_coordinates = original.m_texture_coordinates;
		m_normals = original.m_normals;
		m_point_sets = original.m_point_sets;
		m_lines = original.m_lines;
		m_face_sets = original.m_face_sets;

		m_name = original.m_name;
		m_valid = original.m_valid;
	}

	assert(invariant());
	return *this;
}

ObjModel :: ~ObjModel ()
{
}



bool ObjModel :: isEmpty () const
{
	if(!m_material_libraries.empty()) return false;
	if(!m_vertices.empty()) return false;
	if(!m_texture_coordinates.empty()) return false;
	if(!m_normals.empty()) return false;
	if(!m_point_sets.empty()) return false;
	if(!m_lines.empty()) return false;
	if(!m_face_sets.empty()) return false;
	return true;
}

const string& ObjModel :: getName () const
{
	return m_name;
}

unsigned int ObjModel :: getVertexCount () const
{
	return m_vertices.size();
}

double ObjModel :: getVertexX (unsigned int vertex) const
{
	assert(vertex < getVertexCount());

	return m_vertices[vertex].x;
}

double ObjModel :: getVertexY (unsigned int vertex) const
{
	assert(vertex < getVertexCount());

	return m_vertices[vertex].y;
}

double ObjModel :: getVertexZ (unsigned int vertex) const
{
	assert(vertex < getVertexCount());

	return m_vertices[vertex].z;
}

const Vector3& ObjModel :: getVertexPosition (unsigned int vertex) const
{
	assert(vertex < getVertexCount());

	return m_vertices[vertex];
}


unsigned int ObjModel :: getTextureCoordinateCount () const
{
	return m_texture_coordinates.size();
}

double ObjModel :: getTextureCoordinateU (unsigned int texture_coordinate) const
{
	assert(texture_coordinate < getTextureCoordinateCount());

	return m_texture_coordinates[texture_coordinate].x;
}

double ObjModel :: getTextureCoordinateV (unsigned int texture_coordinate) const
{
	assert(texture_coordinate < getTextureCoordinateCount());

	return m_texture_coordinates[texture_coordinate].y;
}

unsigned int ObjModel :: getNormalCount () const
{
	return m_normals.size();
}

double ObjModel :: getNormalX (unsigned int normal) const
{
	assert(normal < getNormalCount());

	return m_normals[normal].x;
}

double ObjModel :: getNormalY (unsigned int normal) const
{
	assert(normal < getNormalCount());

	return m_normals[normal].y;
}

double ObjModel :: getNormalZ (unsigned int normal) const
{
	assert(normal < getNormalCount());

	return m_normals[normal].z;
}

const Vector3& ObjModel :: getNormalVector (unsigned int normal) const
{
	assert(normal < getNormalCount());

	return m_normals[normal];
}

unsigned int ObjModel :: getPointSetCount () const
{
	return m_point_sets.size();
}

unsigned int ObjModel :: getPointSetVertexCount (unsigned int point_set) const
{
	assert(point_set < getPointSetCount());

	return m_point_sets[point_set].m_vertices.size();
}

unsigned int ObjModel :: getPointSetVertexIndex (unsigned int point_set, unsigned int vertex) const
{
	assert(point_set < getPointSetCount());
	assert(vertex < getPointSetVertexCount(point_set));

	return m_point_sets[point_set].m_vertices[vertex];
}

unsigned int ObjModel :: getLineCount () const
{
	return m_lines.size();
}

unsigned int ObjModel :: getLineVertexCount (unsigned int line) const
{
	assert(line < getLineCount());

	return m_lines[line].m_vertices.size();
}

unsigned int ObjModel :: getLineVertexIndex (unsigned int line, unsigned int vertex) const
{
	assert(line < getLineCount());
	assert(vertex < getLineVertexCount(line));

	return m_lines[line].m_vertices[vertex].m_vertex;
}

unsigned int ObjModel :: getLineVertexTextureCoordinates (unsigned int line, unsigned int vertex) const
{
	assert(line < getLineCount());
	assert(vertex < getLineVertexCount(line));

	return m_lines[line].m_vertices[vertex].m_texture_coordinates;
}

unsigned int ObjModel :: getFaceSetCount () const
{
	return m_face_sets.size();
}

bool ObjModel :: isFaceSetMaterial (unsigned int face_set) const
{
	assert(face_set < getFaceSetCount());

	if(m_face_sets[face_set].m_material_name.compare(""))
		return true;
	else
		return false;
}

const string& ObjModel :: getFaceSetMaterialName (unsigned int face_set) const
{
	assert(face_set < getFaceSetCount());
	assert(isFaceSetMaterial(face_set));

	return m_face_sets[face_set].m_material_name;
}

bool ObjModel :: isFaceSetAllTriangles(unsigned int face_set) const
{
	assert(face_set < getFaceSetCount());

	return m_face_sets[face_set].m_all_triangles;
}

unsigned int ObjModel :: getFaceCount (unsigned int face_set) const
{
	assert(face_set < getFaceSetCount());

	return m_face_sets[face_set].m_faces.size();
}

unsigned int ObjModel :: getFaceVertexCount (unsigned int face_set, unsigned int face) const
{
	assert(face_set < getFaceSetCount());
	assert(face < getFaceCount(face_set));

	return m_face_sets[face_set].m_faces[face].m_vertices.size();
}

unsigned int ObjModel :: getFaceVertexIndex (unsigned int face_set, unsigned int face, unsigned int vertex) const
{
	assert(face_set < getFaceSetCount());
	assert(face < getFaceCount(face_set));
	assert(vertex < getFaceVertexCount(face_set, face));

	return m_face_sets[face_set].m_faces[face].m_vertices[vertex].m_vertex;
}

unsigned int ObjModel :: getFaceVertexTextureCoordinates (unsigned int face_set, unsigned int face, unsigned int vertex) const
{
	assert(face_set < getFaceSetCount());
	assert(face < getFaceCount(face_set));
	assert(vertex < getFaceVertexCount(face_set, face));

	return m_face_sets[face_set].m_faces[face].m_vertices[vertex].m_texture_coordinates;
}

unsigned int ObjModel :: getFaceVertexNormal (unsigned int face_set, unsigned int face, unsigned int vertex) const
{
	assert(face_set < getFaceSetCount());
	assert(face < getFaceCount(face_set));
	assert(vertex < getFaceVertexCount(face_set, face));

	return m_face_sets[face_set].m_faces[face].m_vertices[vertex].m_normal;
}

unsigned int ObjModel :: getFaceCountTotal () const
{
	unsigned int total = 0;

	for(unsigned int i = 0; i < m_face_sets.size(); i++)
		total += m_face_sets[i].m_faces.size();
	return total;
}

bool ObjModel :: isValid () const
{
	return m_valid;
}



void ObjModel :: print () const
{
	if(m_valid)
		cout << m_name << " (valid)" << endl;
	else
		cout << m_name << " (invalid)" << endl;

	if(m_material_libraries.size() > 0)
	{
		cout << "    " << m_material_libraries.size() << " material libraries" << endl;
		for(unsigned int m = 0; m < m_material_libraries.size(); m++)
			cout << "        " << m_material_libraries[m].m_name << endl;
	}

	cout << "    Vertices: " << getVertexCount() << endl;
	for(unsigned int v = 0; v < getVertexCount(); v++)
		cout << "    " << setw(6) << v << ": " << m_vertices[v] << endl;

	cout << "    Texture Coordinate Pairs: " << getTextureCoordinateCount() << endl;
	for(unsigned int t = 0; t < getTextureCoordinateCount(); t++)
		cout << "    " << setw(6) << t << ": (" << m_texture_coordinates[t].x << ", " << m_texture_coordinates[t].y << ")" << endl;

	cout << "    Normals: " << getNormalCount() << endl;
	for(unsigned int n = 0; n < getNormalCount(); n++)
		cout << "    " << setw(6) << n << ": " << m_normals[n] << endl;

	cout << "    Point Sets: " << getPointSetCount() << endl;
	for(unsigned int p = 0; p < getPointSetCount(); p++)
	{
		cout << "    " << setw(6) << p << ": " << getPointSetVertexCount(p) << endl;
		for(unsigned int i = 0; i < getPointSetVertexCount(p); i++)
			cout << "        " << setw(6) << i << ": " << getPointSetVertexIndex(p, i) << endl;
	}

	cout << "    Lines: " << getLineCount() << endl;
	for(unsigned int l = 0; l < getLineCount(); l++)
	{
		cout << "    " << setw(6) << l << ": " << getLineVertexCount(l) << endl;
		for(unsigned int i = 0; i < getLineVertexCount(l); i++)
		{
			cout << "        " << setw(6) << i << ": ";
			cout << getLineVertexIndex(l, i);
			if(getLineVertexTextureCoordinates(l, i) != NO_TEXTURE_COORDINATES)
				cout << "\t" << getLineVertexTextureCoordinates(l, i);
			cout << endl;
		}
	}

	cout << "    Face Sets: " << getFaceSetCount() << endl;
	for(unsigned int s = 0; s < getFaceSetCount(); s++)
	{
		cout << "        Set " << s << ": " << getFaceCount(s);
		if(isFaceSetAllTriangles(s))
			cout << " (triangles)";
		cout << endl;

		if(isFaceSetMaterial(s))
			cout << "            Material: \"" << getFaceSetMaterialName(s) << "\"" << endl;

		for(unsigned int f = 0; f < getFaceCount(s); f++)
		{
			cout << "        " << setw(6) << f << ": " << getFaceVertexCount(s, f) << endl;
			for(unsigned int i = 0; i < getFaceVertexCount(s, f); i++)
			{
				cout << "            " << setw(6) << i << ": ";
				cout << getFaceVertexIndex(s, f, i);
				if(getFaceVertexTextureCoordinates(s, f, i) != NO_TEXTURE_COORDINATES)
				{
					cout << "\t" << getFaceVertexTextureCoordinates(s, f, i);
					if(getFaceVertexNormal(s, f, i) != NO_NORMAL)
						cout << "\t" << getFaceVertexNormal(s, f, i);
				}
				else if(getFaceVertexNormal(s, f, i) != NO_NORMAL)
					cout << "\t\t" << getFaceVertexNormal(s, f, i);
				cout << endl;
			}
		}
	}

	cout << endl;
}

void ObjModel :: printMtlLibraries () const
{
		cout << "Libraries referenced by \"" << m_name << "\":" << endl;
	for(unsigned int m = 0; m < m_material_libraries.size(); m++)
		cout << "\t\"" << m_material_libraries[m].m_name << "\"" << endl;
}

void ObjModel :: printBadMaterials () const
{
	for(unsigned int s = 0; s < getFaceSetCount(); s++)
		if(m_face_sets[s].m_material_name.compare("") != 0 && m_face_sets[s].mp_material == NULL)
			cout << "Invalid material referenced in \"" << m_name << "\": \"" << m_face_sets[s].m_material_name << "\"" << endl;
}

void ObjModel :: draw () const
{
	assert(isValid());
	assert(!Material::isMaterialActive());

	drawPointSets();
	drawLines();

	for(unsigned int s = 0; s < getFaceSetCount(); s++)
		drawFaceSetMaterial(s, m_face_sets[s].mp_material);

	assert(!Material::isMaterialActive());
}

#ifdef OBJ_LIBRARY_SHADER_DISPLAY
void ObjModel :: drawShaders () const
{
	const unsigned int VERTEXES_PER_TRIANGLE = 3;

	//
	//  Do not change these without changing the code to
	//    fill the arrays below!  Otherwise, the program
	//    will crash.
	//

	const unsigned int ELEMENTS_PER_INDEX = 1;
	const unsigned int ELEMENTS_PER_VERTEX = 3;
	const unsigned int ELEMENTS_PER_TEXTURE_COORDINATE = 2;
	const unsigned int ELEMENTS_PER_NORMAL = 3;

	const Vector2 DEFAULT_TEXTURE_COORDINATE = Vector2::ZERO;
	const Vector3 DEFAULT_NORMAL = Vector3::ZERO;

	assert(isValid());
	assert(!Material::isMaterialActive());

	// count the required triangles
	unsigned int total_triangle_count = 0;
	unsigned int* d_triangle_count_per_face = new unsigned int[getFaceSetCount()];
	unsigned int* d_first_triangle_for_face = new unsigned int[getFaceSetCount()];
	for(unsigned int s1 = 0; s1 < getFaceSetCount(); s1++)
	{
		d_triangle_count_per_face[s1] = 0;
		d_first_triangle_for_face[s1] = total_triangle_count;
		for(unsigned int f = 0; f < getFaceCount(s1); f++)
		{
			unsigned int vertex_count = getFaceVertexCount(s1, f);
			assert(vertex_count >= 3);
			unsigned int triangle_count = vertex_count - 2;
			total_triangle_count += triangle_count;
			d_triangle_count_per_face[s1] += triangle_count;
		}
	}

	unsigned int total_vertex_count = total_triangle_count * VERTEXES_PER_TRIANGLE;
	unsigned int index_array_size              = total_vertex_count * ELEMENTS_PER_INDEX;
	unsigned int vertex_array_size             = total_vertex_count * ELEMENTS_PER_VERTEX;
	unsigned int texture_coordinate_array_size = total_vertex_count * ELEMENTS_PER_TEXTURE_COORDINATE;
	unsigned int normal_array_size             = total_vertex_count * ELEMENTS_PER_NORMAL;

	unsigned int* d_indexes     = new unsigned int[total_vertex_count];
	float* d_vertexes           = new float[vertex_array_size];
	float* d_texture_coordintes = new float[texture_coordinate_array_size];
	float* d_normals            = new float[normal_array_size];

	unsigned int next_index = 0;
	for(unsigned int s2 = 0; s2 < getFaceSetCount(); s2++)
	{
		for(unsigned int f = 0; f < getFaceCount(s2); f++)
		{
			for(unsigned int v = 0; v < getFaceVertexCount(s2, f); v++)
			{
				unsigned int index_index              = next_index * ELEMENTS_PER_INDEX;
				unsigned int index_vertex             = next_index * ELEMENTS_PER_VERTEX;
				unsigned int index_texture_coordinate = next_index * ELEMENTS_PER_TEXTURE_COORDINATE;
				unsigned int index_normal             = next_index * ELEMENTS_PER_NORMAL;

				assert(index_index              < index_array_size);
				assert(index_vertex             < vertex_array_size);
				assert(index_texture_coordinate < texture_coordinate_array_size);
				assert(index_normal             < normal_array_size);

				assert(ELEMENTS_PER_INDEX == 1);
				assert(ELEMENTS_PER_VERTEX == 3);
				assert(ELEMENTS_PER_TEXTURE_COORDINATE == 2);
				assert(ELEMENTS_PER_NORMAL == 3);

				d_indexes[index_index] = next_index;

				unsigned int face_vertex_index = getFaceVertexIndex(s2, f, v);
				d_vertexes[index_vertex]     = getVertexPosition(face_vertex_index).x;
				d_vertexes[index_vertex + 1] = getVertexPosition(face_vertex_index).y;
				d_vertexes[index_vertex + 2] = getVertexPosition(face_vertex_index).z;

				unsigned int face_vertex_texture_coordinate = getFaceVertexTextureCoordinates(s2, f, v);
				if(face_vertex_texture_coordinate == NO_TEXTURE_COORDINATES)
				{
					d_texture_coordintes[index_texture_coordinate]     = DEFAULT_TEXTURE_COORDINATE.x;
					d_texture_coordintes[index_texture_coordinate + 1] = DEFAULT_TEXTURE_COORDINATE.y;
				}
				else
				{
					d_texture_coordintes[index_texture_coordinate]     = getTextureCoordinateU(face_vertex_texture_coordinate);
					d_texture_coordintes[index_texture_coordinate + 1] = getTextureCoordinateV(face_vertex_texture_coordinate);
				}

				unsigned int face_vertex_normal = getFaceVertexNormal(s2, f, v);
				if(face_vertex_normal == NO_NORMAL)
				{
					d_normals[index_normal]     = DEFAULT_NORMAL.x;
					d_normals[index_normal + 1] = DEFAULT_NORMAL.y;
					d_normals[index_normal + 2] = DEFAULT_NORMAL.z;
				}
				else
				{
					d_normals[index_normal]     = getNormalVector(face_vertex_normal).x;
					d_normals[index_normal + 1] = getNormalVector(face_vertex_normal).y;
					d_normals[index_normal + 2] = getNormalVector(face_vertex_normal).z;
				}

				next_index++;
			}
		}
	}

	for(unsigned int s3 = 0; s3 < getFaceSetCount(); s3++)
	{
		if(m_face_sets[s3].mp_material != NULL)
			m_face_sets[s3].mp_material->activate();

		glBegin(GL_TRIANGLES);
			if(DEBUGGING_VERTEX_BUFFER)
				cout << s3 << ":\t" << d_first_triangle_for_face[s3] << " -> " << d_triangle_count_per_face[s3] << endl;
			for(unsigned int t = 0; t < d_triangle_count_per_face[s3]; t++)
			{
				if(DEBUGGING_VERTEX_BUFFER)
					cout << "  " << t << ":\t" << endl;
				for(unsigned int v = 0; v < VERTEXES_PER_TRIANGLE; v++)
				{
					unsigned int position = (d_first_triangle_for_face[s3] + t) * VERTEXES_PER_TRIANGLE + v;
					if(DEBUGGING_VERTEX_BUFFER)
					{
						cout << "    " << position << endl;
						cout << "      Vertex: (" << d_vertexes[position * ELEMENTS_PER_VERTEX]
						     << ", " << d_vertexes[position * ELEMENTS_PER_VERTEX + 1]
						     << ", " << d_vertexes[position * ELEMENTS_PER_VERTEX + 2] << ")" << endl;
						cout << "      TextureCoordinates: (" << d_texture_coordintes[position * ELEMENTS_PER_TEXTURE_COORDINATE]
						     << ", " << d_texture_coordintes[position * ELEMENTS_PER_TEXTURE_COORDINATE + 1] << ")" << endl;
						cout << "      Normal: (" << d_normals[position * ELEMENTS_PER_NORMAL]
						     << ", " << d_normals[position * ELEMENTS_PER_NORMAL + 1]
						     << ", " << d_normals[position * ELEMENTS_PER_NORMAL + 2] << ")" << endl;
					}
					glNormal3fv(d_normals + position * ELEMENTS_PER_NORMAL);
					glTexCoord2fv(d_texture_coordintes + position * ELEMENTS_PER_TEXTURE_COORDINATE);
					glVertex3fv(d_vertexes + position * ELEMENTS_PER_VERTEX);
				}
			}
		glEnd();

		if(m_face_sets[s3].mp_material != NULL)
			Material::deactivate();
	}

	delete[] d_indexes;
	delete[] d_vertexes;
	delete[] d_texture_coordintes;
	delete[] d_normals;

	delete[] d_triangle_count_per_face;
	delete[] d_first_triangle_for_face;

	assert(!Material::isMaterialActive());
}
#else
void ObjModel :: drawShaders () const
{
	draw();
}
#endif	// #ifdef OBJ_LIBRARY_SHADER_DISPLAY

void ObjModel :: drawVertices (unsigned char red, unsigned char green, unsigned char blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());

	drawVertices(red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawVertices (int red, int green, int blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(red >= 0x00);
	assert(red <= 0xFF);
	assert(green >= 0x00);
	assert(green <= 0xFF);
	assert(blue >= 0x00);
	assert(blue <= 0xFF);

	drawVertices(red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawVertices (double red, double green, double blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(red >= 0.0);
	assert(red <= 1.0);
	assert(green >= 0.0);
	assert(green <= 1.0);
	assert(blue >= 0.0);
	assert(blue <= 1.0);

	Material material;

	material.setIlluminationMode(Material::ILLUMINATION_CONSTANT);
	material.setDiffuseColour(red, green, blue);
	material.activate();

	glBegin(GL_POINTS);
		for(unsigned int v = 0; v < getVertexCount(); v++)
		{
			glVertex3d(m_vertices[v].x,
				   m_vertices[v].y,
				   m_vertices[v].z);
		}
	glEnd();

	material.deactivate();

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawWireframe (unsigned char red, unsigned char green, unsigned char blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());

	drawWireframe(red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawWireframe (int red, int green, int blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(red >= 0x00);
	assert(red <= 0xFF);
	assert(green >= 0x00);
	assert(green <= 0xFF);
	assert(blue >= 0x00);
	assert(blue <= 0xFF);

	drawWireframe(red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawWireframe (double red, double green, double blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(red >= 0.0);
	assert(red <= 1.0);
	assert(green >= 0.0);
	assert(green <= 1.0);
	assert(blue >= 0.0);
	assert(blue <= 1.0);

	Material material;

	material.setIlluminationMode(Material::ILLUMINATION_CONSTANT);
	material.setDiffuseColour(red, green, blue);
	material.activate();

	drawPointSets();
	drawLines();

	for(unsigned int s = 0; s < getFaceSetCount(); s++)
		for(unsigned int f = 0; f < getFaceCount(s); f++)
		{
			glBegin(GL_LINE_LOOP);
				for(unsigned int v = 0; v < getFaceVertexCount(s, f); v++)
				{
					unsigned int vertex = m_face_sets[s].m_faces[f].m_vertices[v].m_vertex;

					glVertex3d(m_vertices[vertex].x,
						   m_vertices[vertex].y,
						   m_vertices[vertex].z);
				}
			glEnd();
		}

	material.deactivate();

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawSolid (unsigned char red, unsigned char green, unsigned char blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());

	drawSolid(red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawSolid (int red, int green, int blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(red >= 0x00);
	assert(red <= 0xFF);
	assert(green >= 0x00);
	assert(green <= 0xFF);
	assert(blue >= 0x00);
	assert(blue <= 0xFF);

	drawSolid(red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawSolid (double red, double green, double blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(red >= 0.0);
	assert(red <= 1.0);
	assert(green >= 0.0);
	assert(green <= 1.0);
	assert(blue >= 0.0);
	assert(blue <= 1.0);

	Material material;

	material.setIlluminationMode(Material::ILLUMINATION_CONSTANT);
	material.setDiffuseColour(red, green, blue);
	material.activate();

	drawPointSets();
	drawLines();

	for(unsigned int s = 0; s < getFaceSetCount(); s++)
		drawFaceSet(s);

	material.deactivate();

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawMaterial (const Material& material) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());

	material.activate();

	drawPointSets();
	drawLines();
	for(unsigned int s = 0; s < getFaceSetCount(); s++)
		drawFaceSet(s);

	Material::deactivate();

	if(material.isSeperateSpecular())
	{
		material.activateSeperateSpecular();
		for(unsigned int s2 = 0; s2 < getFaceSetCount(); s2++)
			drawFaceSet(s2);
		Material::deactivate();
	}

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawMaterialNone () const
{
	assert(isValid());

	drawPointSets();
	drawLines();
	for(unsigned int s = 0; s < getFaceSetCount(); s++)
		drawFaceSet(s);
}

void ObjModel :: drawMaterials (const vector<Material*>& materials) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(materials.size() == getFaceSetCount());

	drawPointSets();
	drawLines();

	for(unsigned int s = 0; s < getFaceSetCount(); s++)
		drawFaceSetMaterial(s, materials[s]);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawMtlLibrary (const MtlLibrary& library) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());

	drawPointSets();
	drawLines();

	for(unsigned int s = 0; s < getFaceSetCount(); s++)
	{
		const Material* p_material = NULL;

		if(m_face_sets[s].m_material_name.compare("") != 0)
			p_material = library.getMaterial(m_face_sets[s].m_material_name);
		if(p_material == NULL)
			p_material = m_face_sets[s].mp_material;

		drawFaceSetMaterial(s, p_material);
	}

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawMtlLibrary (const char* a_name) const
{
	assert(a_name != NULL);

	drawMtlLibrary(MtlLibraryManager::get(a_name));
}

void ObjModel :: drawMtlLibrary (const string& name) const
{
	drawMtlLibrary(MtlLibraryManager::get(name));
}

void ObjModel :: drawVertexNormals (double length, unsigned char red, unsigned char green, unsigned char blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(length > 0.0);

	drawVertexNormals(length, red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawVertexNormals (double length, int red, int green, int blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(length > 0.0);
	assert(red >= 0x00);
	assert(red <= 0xFF);
	assert(green >= 0x00);
	assert(green <= 0xFF);
	assert(blue >= 0x00);
	assert(blue <= 0xFF);

	drawVertexNormals(length, red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawVertexNormals (double length, double red, double green, double blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(length > 0.0);
	assert(red >= 0.0);
	assert(red <= 1.0);
	assert(green >= 0.0);
	assert(green <= 1.0);
	assert(blue >= 0.0);
	assert(blue <= 1.0);

	Material material;

	material.setIlluminationMode(Material::ILLUMINATION_CONSTANT);
	material.setDiffuseColour(red, green, blue);
	material.activate();

	glBegin(GL_LINES);
		for(unsigned int s = 0; s < getFaceSetCount(); s++)
			for(unsigned int f = 0; f < getFaceCount(s); f++)
				for(unsigned int v = 0; v < getFaceVertexCount(s, f); v++)
				{
					unsigned int vertex = m_face_sets[s].m_faces[f].m_vertices[v].m_vertex;
					unsigned int normal = m_face_sets[s].m_faces[f].m_vertices[v].m_normal;

					if(normal != NO_NORMAL)
					{
						assert(vertex < getVertexCount());
						assert(normal < getNormalCount());

						glVertex3d(m_vertices[vertex].x,
							   m_vertices[vertex].y,
							   m_vertices[vertex].z);

						glVertex3d(m_vertices[vertex].x + m_normals[normal].x * length,
							   m_vertices[vertex].y + m_normals[normal].y * length,
							   m_vertices[vertex].z + m_normals[normal].z * length);
					}
				}
	glEnd();

	material.deactivate();

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawFaceNormals (double length, unsigned char red, unsigned char green, unsigned char blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(length > 0.0);

	drawFaceNormals(length, red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawFaceNormals (double length, int red, int green, int blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(length > 0.0);
	assert(red >= 0x00);
	assert(red <= 0xFF);
	assert(green >= 0x00);
	assert(green <= 0xFF);
	assert(blue >= 0x00);
	assert(blue <= 0xFF);

	drawFaceNormals(length, red / 255.0, green / 255.0, blue / 255.0);

	assert(!Material::isMaterialActive());
}

void ObjModel :: drawFaceNormals (double length, double red, double green, double blue) const
{
	assert(isValid());
	assert(!Material::isMaterialActive());
	assert(length > 0.0);
	assert(red >= 0.0);
	assert(red <= 1.0);
	assert(green >= 0.0);
	assert(green <= 1.0);
	assert(blue >= 0.0);
	assert(blue <= 1.0);

	Material material;

	material.setIlluminationMode(Material::ILLUMINATION_CONSTANT);
	material.setDiffuseColour(red, green, blue);
	material.activate();

	glBegin(GL_LINES);
		for(unsigned int s = 0; s < getFaceSetCount(); s++)
			for(unsigned int f = 0; f < getFaceCount(s); f++)
			{
				Vector3 center = Vector3::ZERO;
				Vector3 face_normal = Vector3::ZERO;

				for(unsigned int v = 0; v < getFaceVertexCount(s, f); v++)
				{
					unsigned int vertex = m_face_sets[s].m_faces[f].m_vertices[v].m_vertex;
					unsigned int normal = m_face_sets[s].m_faces[f].m_vertices[v].m_normal;

					assert(vertex < getVertexCount());
					center += m_vertices[vertex];

					if(normal != NO_NORMAL)
					{
						assert(normal < getNormalCount());
						face_normal += m_normals[normal];
					}
				}

				if(!face_normal.isZero())
				{
					center /= getFaceVertexCount(s, f);
					face_normal.setNorm(length);

					glVertex3d(center.x,
						   center.y,
						   center.z);

					glVertex3d(center.x + face_normal.x,
						   center.y + face_normal.y,
						   center.z + face_normal.z);
				}
			}
	glEnd();

	material.deactivate();

	assert(!Material::isMaterialActive());
}

DisplayList ObjModel :: getDisplayList () const
{
	assert(isValid());
	assert(!Material::isMaterialActive());

	DisplayList list;

	//
	//  We are using a loop to load textures instead of
	//
	//	loadDisplayTextures();
	//
	//    so that we only load textures used by this model.
	//    Otherwise, any texture required by any material in
	//    an MTL file referenced by an ObjModel would be
	//    loaded, even if that material - and thus that
	//    texture - was never used.  This way, we avoid
	//    cluttering up video memory with textures we never
	//    use.
	//

	for(unsigned int i = 0; i < m_face_sets.size(); i++)
		if(m_face_sets[i].mp_material != NULL)
			m_face_sets[i].mp_material->loadDisplayTextures();

	list.begin();
		draw();
	list.end();

	assert(!Material::isMaterialActive());
	return list;
}

void ObjModel ::  save (const string& filename) const
{
	assert(filename.compare("") != 0);

	save(filename, cerr);

	assert(invariant());
}

void ObjModel ::  save (const string& filename, const string& logfile) const
{
	assert(filename.compare("") != 0);
	assert(logfile.compare("") != 0);

	ofstream logstream;

	logstream.open(logfile.c_str());
	save(filename, logstream);
	logstream.close();

	assert(invariant());
}

void ObjModel ::  save (const string& filename, ostream& r_logstream) const
{
	assert(filename.compare("") != 0);

	ofstream output_file;

	if(DEBUGGING)
		cout << "About to save " << filename << endl;

	output_file.open(filename.c_str());
	if(!output_file.is_open())
	{
		r_logstream << "ERROR: cannot write to file \"" << filename << "\" - ABORTING" << endl;
		return;
	}

	//
	//  Format of file:
	//
	//  Header
	//    -> how many of each element
	//  Material libraries
	//  Vertices
	//  Texture coordinate pairs
	//  Normals
	//  Point sets
	//  Lines
	//  Faces
	//    -> Divided into face sets with associated materials
	//  Footer
	//
	//  Major sections are seperated by 3 blank lines.  Minor
	//    sections are seperated by 1 blank line.
	//

	output_file << "#" << endl;
	output_file << "# " << getName() << endl;
	output_file << "#" << endl;
	if(m_vertices.size() > 0)
		output_file << "# " << getVertexCount() << " vertices" << endl;
	if(m_texture_coordinates.size() > 0)
		output_file << "# " << getTextureCoordinateCount() << " texture coordinate pairs" << endl;
	if(m_normals.size() > 0)
		output_file << "# " << getNormalCount() << " vertex normals" << endl;
	if(m_point_sets.size() > 0)
		output_file << "# " << getPointSetCount() << " point sets" << endl;
	if(m_lines.size() > 0)
		output_file << "# " << getLineCount() << " polylines" << endl;
	if(m_face_sets.size() > 0)
		output_file << "# " << getFaceCountTotal() << " in " << getFaceSetCount() << " face sets" << endl;
	output_file << "#" << endl;
	output_file << endl;
	output_file << endl;
	output_file << endl;

	if(DEBUGGING)
		cout << "Wrote file header" << endl;

	if(m_material_libraries.size() > 0)
	{
		output_file << "# " << m_material_libraries.size() << " material libraries" << endl;
		output_file << "mtllib";
		for(unsigned int m = 0; m < m_material_libraries.size(); m++)
			output_file << " " << m_material_libraries[m].m_name;
		output_file << endl;
		output_file << endl;
		output_file << endl;
		output_file << endl;

		if(DEBUGGING)
			cout << "Wrote material libraries" << endl;
	}

	if(m_vertices.size() > 0)
	{
		output_file << "# " << getVertexCount() << " vertices" << endl;
		for(unsigned int v = 0; v < m_vertices.size(); v++)
			output_file << "v " << m_vertices[v].x << " " << m_vertices[v].y << " " << m_vertices[v].z << endl;
		output_file << endl;
		output_file << endl;
		output_file << endl;

		if(DEBUGGING)
			cout << "Wrote vertices" << endl;
	}

	if(m_texture_coordinates.size() > 0)
	{
		output_file << "# " << getTextureCoordinateCount() << " texture coordinate pairs" << endl;
		for(unsigned int t = 0; t < m_texture_coordinates.size(); t++)
			output_file << "vt " << m_texture_coordinates[t].x << " " << m_texture_coordinates[t].y << endl;
		output_file << endl;
		output_file << endl;
		output_file << endl;

		if(DEBUGGING)
			cout << "Wrote texture coordinates" << endl;
	}

	if(m_normals.size() > 0)
	{
		output_file << "# " << getNormalCount() << " vertex normals" << endl;
		for(unsigned int n = 0; n < m_normals.size(); n++)
			output_file << "vn " << m_normals[n].x << " " << m_normals[n].y << " " << m_normals[n].z << endl;
		output_file << endl;
		output_file << endl;
		output_file << endl;

		if(DEBUGGING)
			cout << "Wrote normals" << endl;
	}

	if(m_point_sets.size() > 0)
	{
		output_file << "# " << getPointSetCount() << " point sets" << endl;
		for(unsigned int p = 0; p < m_point_sets.size(); p++)
		{
			output_file << "p";
			for(unsigned int i = 0; i < m_point_sets[p].m_vertices.size(); i++)
				output_file << " " << (m_point_sets[p].m_vertices[i] + 1);
			output_file << endl;
		}
		output_file << endl;
		output_file << endl;
		output_file << endl;

		if(DEBUGGING)
			cout << "Wrote point sets" << endl;
	}

	if(m_lines.size() > 0)
	{
		output_file << "# " << getLineCount() << " polylines" << endl;
		for(unsigned int l = 0; l < m_lines.size(); l++)
		{
			output_file << "l";
			for(unsigned int i = 0; i < m_lines[l].m_vertices.size(); i++)
			{
				output_file << " " << (m_lines[l].m_vertices[i].m_vertex + 1);

				if(m_lines[l].m_vertices[i].m_texture_coordinates != NO_TEXTURE_COORDINATES)
					output_file << "/" << (m_lines[l].m_vertices[i].m_texture_coordinates + 1);
			}
			output_file << endl;
		}
		output_file << endl;
		output_file << endl;
		output_file << endl;

		if(DEBUGGING)
			cout << "Wrote polylines" << endl;
	}

	if(m_face_sets.size() > 0)
	{
		output_file << "# " << getFaceSetCount() << " face sets" << endl;
		output_file << endl;
		for(unsigned int s = 0; s < m_face_sets.size(); s++)
		{
			output_file << "# " << getFaceCount(s) << " faces" << endl;
			if(isFaceSetMaterial(s))
				output_file << "usemtl " << m_face_sets[s].m_material_name << endl;

			for(unsigned int f = 0; f < m_face_sets[s].m_faces.size(); f++)
			{
				output_file << "f";
				for(unsigned int i = 0; i < m_face_sets[s].m_faces[f].m_vertices.size(); i++)
				{
					output_file << " " << (m_face_sets[s].m_faces[f].m_vertices[i].m_vertex + 1);

					if(m_face_sets[s].m_faces[f].m_vertices[i].m_texture_coordinates != NO_TEXTURE_COORDINATES)
					{
						output_file << "/" << (m_face_sets[s].m_faces[f].m_vertices[i].m_texture_coordinates + 1);

						if(m_face_sets[s].m_faces[f].m_vertices[i].m_normal != NO_NORMAL)
							output_file << "/" << (m_face_sets[s].m_faces[f].m_vertices[i].m_normal + 1);
					}
					else if(m_face_sets[s].m_faces[f].m_vertices[i].m_normal != NO_NORMAL)
						output_file << "//" << (m_face_sets[s].m_faces[f].m_vertices[i].m_normal + 1);

				}
				output_file << endl;
			}
			output_file << endl;
		}
		output_file << endl;
		output_file << endl;
		output_file << endl;

		if(DEBUGGING)
			cout << "Wrote faces" << endl;
	}

	output_file << "# End of " << getName() << endl;
	output_file << endl;

	if(DEBUGGING)
		cout << "Wrote footer" << endl;

	output_file.close();
}



void ObjModel :: makeEmpty ()
{
	m_vertices.clear();
	m_texture_coordinates.clear();
	m_normals.clear();
	m_point_sets.clear();
	m_lines.clear();
	m_face_sets.clear();

	m_name = DEFAULT_NAME;
	m_valid = true;

	assert(isEmpty());
	assert(invariant());
}

void ObjModel :: load (const string& filename)
{
	assert(filename.compare("") != 0);

	load(filename, cerr);

	assert(invariant());
}

void ObjModel :: load (const string& filename, const string& logfile)
{
	assert(filename.compare("") != 0);
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	load(filename, logstream);
	logstream.close();

	assert(invariant());
}

void ObjModel :: load (const string& filename, ostream& r_logstream)
{
	assert(filename.compare("") != 0);

	ifstream input_file;
	unsigned int line_count;

	if(DEBUGGING)
		cout << "About to remove any existing contents" << endl;

	makeEmpty();

	if(DEBUGGING)
		cout << "About to load " << filename << endl;

	m_name = filename;
	input_file.open(filename.c_str(), ios::in);
	if(!input_file.is_open())
	{
		r_logstream << "Error: File \"" << filename << "\" does not exist" << endl;
		input_file.close();

		assert(invariant());
		return;
	}

	//
	//  Format is available at
	//
	//  http://www.martinreddy.net/gfx/3d/OBJ.spec
	//

	line_count = 0;
	while(true)	// drops out at EOF below
	{
		string line;
		size_t length;
		bool valid;

		getline(input_file, line);
		if(input_file.eof())
			break;	// drop out after last line

		length = line.length();
		line_count++;

		if(length < 1 || line[0] == '#' || line[0] == '\r')
			continue;	// skip blank lines and comments

		valid = true;
		if(length < 1)
			valid = false;
		else
		{
			if(line.length() >= 7 && line.substr(0, 6).compare("mtllib") == 0 && isspace(line[6]))
				valid = readMaterialLibrary(line.substr(7), r_logstream);
			else if(line.length() >= 7 && line.substr(0, 6).compare("usemtl") == 0 && isspace(line[6]))
				valid = readMaterial(line.substr(7), r_logstream);
			else if(line.length() >= 2 && line[0] == 'v' && isspace(line[1]))
				valid = readVertex(line.substr(2), r_logstream);
			else if(line.length() >= 3 && line[0] == 'v' && line[1] == 't' && isspace(line[2]))
				valid = readTextureCoordinates(line.substr(3), r_logstream);
			else if(line.length() >= 3 && line[0] == 'v' && line[1] == 'n' && isspace(line[2]))
				valid = readNormal(line.substr(3), r_logstream);
			else if(line.length() >= 2 && line[0] == 'p' && isspace(line[1]))
				valid = readPointSet(line.substr(2), r_logstream);
			else if(line.length() >= 2 && line[0] == 'l' && isspace(line[1]))
				valid = readLine(line.substr(2), r_logstream);
			else if(line.length() >= 2 && line[0] == 'f' && isspace(line[1]))
				valid = readFace(line.substr(2), r_logstream);
			else if(line[0] == 'g' && (line.length() == 1 || isspace(line[1])))
			{
				if(DEBUGGING)
					r_logstream << "In file \"" << filename << "\": ignoring groupings \"" << line.substr(1) << "\"" << endl;
			}
			else if(line[0] == 's' && (line.length() == 1 || isspace(line[1])))
			{
				if(DEBUGGING)
					r_logstream << "In file \"" << filename << "\": ignoring smoothing group \"" << line.substr(1) << "\"" << endl;
			}
			else if(line[0] == 'o' && (line.length() == 1 || isspace(line[1])))
			{
				if(DEBUGGING)
					r_logstream << "In file \"" << filename << "\": ignoring object name \"" << line.substr(1) << "\"" << endl;
			}
			else
				valid = false;
		}

		if(!valid)
			r_logstream << "Line " << setw(6) << line_count << " of file \"" << filename << "\" is invalid: \"" << line << "\"" << endl;
	}

	input_file.close();

	validate();

	assert(invariant());
}



void ObjModel :: setName (const string& name)
{
	assert(name.compare("") != 0);

	m_name = name;

	assert(invariant());
}

void ObjModel :: setVertexCount (unsigned int count)
{
	if(count < getVertexCount())
	{
		m_valid = false;
		m_vertices.resize(count);
	}
	else if(count > getVertexCount())
		m_vertices.resize(count, Vector3::ZERO);

	assert(invariant());
}

void ObjModel :: setVertexX (unsigned int vertex, double x)
{
	assert(vertex < getVertexCount());

	m_vertices[vertex].x = x;

	assert(invariant());
}

void ObjModel :: setVertexY (unsigned int vertex, double y)
{
	assert(vertex < getVertexCount());

	m_vertices[vertex].y = y;

	assert(invariant());
}

void ObjModel :: setVertexZ (unsigned int vertex, double z)
{
	assert(vertex < getVertexCount());

	m_vertices[vertex].z = z;

	assert(invariant());
}

void ObjModel :: setVertexPosition (unsigned int vertex, double x, double y, double z)
{
	assert(vertex < getVertexCount());

	m_vertices[vertex].set(x, y, z);

	assert(invariant());
}

void ObjModel :: setVertexPosition (unsigned int vertex, const Vector3& position)
{
	assert(vertex < getVertexCount());

	m_vertices[vertex] = position;

	assert(invariant());
}

void ObjModel :: setTextureCoordinateCount (unsigned int count)
{
	if(count < getTextureCoordinateCount())
	{
		m_valid = false;
		m_texture_coordinates.resize(count);
	}
	else if(count > getTextureCoordinateCount())
		m_texture_coordinates.resize(count, Vector2::ZERO);

	assert(invariant());
}

void ObjModel :: setTextureCoordinateU (unsigned int texture_coordinate, double u)
{
	assert(texture_coordinate < getTextureCoordinateCount());

	m_texture_coordinates[texture_coordinate].x = u;

	assert(invariant());
}
void ObjModel :: setTextureCoordinateV (unsigned int texture_coordinate, double v)
{
	assert(texture_coordinate < getTextureCoordinateCount());

	m_texture_coordinates[texture_coordinate].y = v;

	assert(invariant());
}

void ObjModel :: setTextureCoordinate (unsigned int texture_coordinate, double u, double v)
{
	assert(texture_coordinate < getTextureCoordinateCount());

	m_texture_coordinates[texture_coordinate].x = u;
	m_texture_coordinates[texture_coordinate].y = v;

	assert(invariant());
}

void ObjModel :: setNormalCount (unsigned int count)
{
	if(count < getNormalCount())
	{
		m_valid = false;
		m_normals.resize(count);
	}
	else if(count > getNormalCount())
		m_normals.resize(count, Vector3::UNIT_Z_PLUS);

	assert(invariant());
}

void ObjModel :: setNormalX (unsigned int normal, double x)
{
	assert(normal < getNormalCount());
	assert(x != 0.0 || getNormalY(normal) != 0.0 || getNormalZ(normal) != 0.0);

	m_normals[normal].x = x;
	assert(!m_normals[normal].isZero());
	m_normals[normal].normalize();

	assert(invariant());
}

void ObjModel :: setNormalY (unsigned int normal, double y)
{
	assert(normal < getNormalCount());
	assert(getNormalX(normal) != 0.0 || y != 0.0 || getNormalZ(normal) != 0.0);

	m_normals[normal].y = y;
	assert(!m_normals[normal].isZero());
	m_normals[normal].normalize();

	assert(invariant());
}

void ObjModel :: setNormalZ (unsigned int normal, double z)
{
	assert(normal < getNormalCount());
	assert(getNormalX(normal) != 0.0 || getNormalY(normal) != 0.0 || z != 0.0);

	m_normals[normal].z = z;
	assert(!m_normals[normal].isZero());
	m_normals[normal].normalize();

	assert(invariant());
}

void ObjModel :: setNormalVector (unsigned int normal, double x, double y, double z)
{
	assert(normal < getNormalCount());
	assert(x != 0.0 || y != 0.0 || z != 0.0);

	m_normals[normal].set(x, y, z);
	assert(!m_normals[normal].isZero());
	m_normals[normal].normalize();

	assert(invariant());
}

void ObjModel :: setNormalVector (unsigned int normal, const Vector3& vector)
{
	assert(normal < getNormalCount());
	assert(!vector.isZero());

	m_normals[normal] = vector.getNormalized();

	assert(invariant());
}

void ObjModel :: setPointSetVertexIndex (unsigned int point_set, unsigned int vertex, unsigned int index)
{
	assert(point_set < getPointSetCount());
	assert(vertex < getPointSetVertexCount(point_set));

	m_point_sets[point_set].m_vertices[vertex] = index;
	if(index >= getVertexCount())
		m_valid = false;

	assert(invariant());
}

void ObjModel :: setLineVertexIndex (unsigned int line, unsigned int vertex, unsigned int index)
{
	assert(line < getLineCount());
	assert(vertex < getLineVertexCount(line));

	m_lines[line].m_vertices[vertex].m_vertex = index;
	if(index >= getVertexCount())
		m_valid = false;

	assert(invariant());
}

void ObjModel :: setLineVertexTextureCoordinates (unsigned int line, unsigned int vertex, unsigned int index)
{
	assert(line < getLineCount());
	assert(vertex < getLineVertexCount(line));

	m_lines[line].m_vertices[vertex].m_texture_coordinates = index;
	if(index >= getTextureCoordinateCount() && index != NO_TEXTURE_COORDINATES)
		m_valid = false;

	assert(invariant());
}

void ObjModel :: setFaceSetMaterial (unsigned int face_set, const string& material)
{
	assert(face_set < getFaceSetCount());
	assert(material.compare("") != 0);

	if(DEBUGGING)
		cout << "    Setting face set " << face_set << " to use material " << material << endl;

	m_face_sets[face_set].m_material_name = material;

	m_face_sets[face_set].mp_material = NULL;
	for(unsigned int i = 0; i < m_material_libraries.size(); i++)
	{
		if(m_material_libraries[i].mp_mtl_library == NULL)
			continue;

		unsigned int index = m_material_libraries[i].mp_mtl_library->getMaterialIndex(material);

		if(index == MtlLibrary::NO_SUCH_MATERIAL)
			continue;

		m_face_sets[face_set].mp_material = m_material_libraries[i].mp_mtl_library->getMaterial(index);
	}

	assert(invariant());
}

void ObjModel :: setFaceSetMaterialNone (unsigned int face_set)
{
	assert(face_set < getFaceSetCount());

	m_face_sets[face_set].m_material_name = "";
	m_face_sets[face_set].mp_material = NULL;

	assert(invariant());
}

void ObjModel :: setFaceVertexIndex (unsigned int face_set, unsigned int face, unsigned int vertex, unsigned int index)
{
	assert(face_set < getFaceSetCount());
	assert(face < getFaceCount(face_set));
	assert(vertex < getFaceVertexCount(face_set, face));

	m_face_sets[face_set].m_faces[face].m_vertices[vertex].m_vertex = index;
	if(index >= getVertexCount())
		m_valid = false;

	assert(invariant());
}

void ObjModel :: setFaceVertexTextureCoordinates (unsigned int face_set, unsigned int face, unsigned int vertex, unsigned int index)
{
	assert(face_set < getFaceSetCount());
	assert(face < getFaceCount(face_set));
	assert(vertex < getFaceVertexCount(face_set, face));

	m_face_sets[face_set].m_faces[face].m_vertices[vertex].m_texture_coordinates = index;
	if(index >= getTextureCoordinateCount() && index != NO_TEXTURE_COORDINATES)
		m_valid = false;

	assert(invariant());
}

void ObjModel :: setFaceVertexNormal (unsigned int face_set, unsigned int face, unsigned int vertex, unsigned int index)
{
	assert(face_set < getFaceSetCount());
	assert(face < getFaceCount(face_set));
	assert(vertex < getFaceVertexCount(face_set, face));

	m_face_sets[face_set].m_faces[face].m_vertices[vertex].m_normal = index;
	if(index >= getVertexCount() && index != NO_NORMAL)
		m_valid = false;

	assert(invariant());
}



void ObjModel :: addMaterialLibrary (const string& library)
{
	assert(library.compare("") != 0);

	addMaterialLibrary(library, cerr);

	assert(invariant());
}

void ObjModel :: addMaterialLibrary (const string& library, const string& logfile)
{
	assert(library.compare("") != 0);
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	load(library, logstream);
	logstream.close();

	assert(invariant());
}

void ObjModel :: addMaterialLibrary (const string& library, ostream& r_logstream)
{
	assert(library.compare("") != 0);

	m_material_libraries.push_back(MaterialLibrary(library, r_logstream));

	if(DEBUGGING)
	{
		unsigned int index = m_material_libraries.size() - 1;
		cout << "Added Material Libary \"" << m_material_libraries[index].m_name << "\"" << endl;
		if(m_material_libraries[index].mp_mtl_library == NULL)
			cout << "But couldn't load it" << endl;
	}

	assert(invariant());
}

unsigned int ObjModel :: addVertex (double x, double y, double z)
{
	return addVertex(Vector3(x, y, z));
}

unsigned int ObjModel :: addVertex (const Vector3& position)
{
	unsigned int id = m_vertices.size();
	m_vertices.push_back(position);

	if(DEBUGGING)
		cout << "Added Vertex #" << (id + 1) << " " << position << endl;

	assert(invariant());
	return id;
}

unsigned int ObjModel :: addTextureCoordinate (double u, double v)
{
	return addTextureCoordinate(Vector2(u, v));
}

unsigned int ObjModel :: addTextureCoordinate (const Vector2& texture_coordinates)
{
	unsigned int id = m_texture_coordinates.size();
	m_texture_coordinates.push_back(texture_coordinates);

	if(DEBUGGING)
		cout << "Added Texture Coordinate #" << (id + 1) << " " << texture_coordinates << endl;

	assert(invariant());
	return id;
}

unsigned int ObjModel :: addNormal (double x, double y, double z)
{
	assert(x != 0.0 || y != 0.0 || z != 0.0);

	return addNormal(Vector3(x, y, z));
}

unsigned int ObjModel :: addNormal (const Vector3& normal)
{
	assert(!normal.isZero());

	unsigned int id = m_normals.size();
	m_normals.push_back(normal.getNormalized());

	if(DEBUGGING)
		cout << "Added Normal #" << (id + 1) << " " << normal << endl;

	assert(invariant());
	return id;
}

unsigned int ObjModel :: addPointSet ()
{
	unsigned int id = m_point_sets.size();
	m_point_sets.push_back(PointSet());
	m_valid = false;

	if(DEBUGGING)
		cout << "Added Point Set #" << (id + 1) << endl;

	assert(invariant());
	return id;
}

unsigned int ObjModel :: addPointSetVertex (unsigned int point_set, unsigned int vertex)
{
	assert(point_set < getPointSetCount());
	assert(vertex >= 1);

	vertex--;

	assert(vertex >= 0);
	unsigned int id = m_point_sets[point_set].m_vertices.size();
	m_point_sets[point_set].m_vertices.push_back(vertex);

	if(vertex >= getVertexCount())
		m_valid = false;

	if(DEBUGGING)
		cout << "    Added vertex #" << (id + 1) << " (" << (vertex + 1) << ")" << endl;

	assert(invariant());
	return id;
}

unsigned int ObjModel :: addLine ()
{
	unsigned int id = m_lines.size();
	m_lines.push_back(Line());
	m_valid = false;

	if(DEBUGGING)
		cout << "Added Line #" << (id + 1) << endl;

	assert(invariant());
	return id;
}

unsigned int ObjModel :: addLineVertex (unsigned int line, unsigned int vertex, unsigned int texture_coordinates)
{
	assert(line < getLineCount());
	assert(vertex >= 1);
	assert(texture_coordinates == NO_TEXTURE_COORDINATES || texture_coordinates >= 1);

	vertex--;
	if(texture_coordinates != NO_TEXTURE_COORDINATES)
		texture_coordinates--;

	assert(vertex >= 0);
	assert(texture_coordinates >= 0 || texture_coordinates == NO_TEXTURE_COORDINATES);
	unsigned int id = m_lines[line].m_vertices.size();
	m_lines[line].m_vertices.push_back(LineVertex(vertex, texture_coordinates));

	if(vertex >= getVertexCount())
		m_valid = false;
	if(texture_coordinates != NO_TEXTURE_COORDINATES && texture_coordinates >= getTextureCoordinateCount())
		m_valid = false;

	if(DEBUGGING)
	{
		cout << "    Added vertex #" << (id + 1) << " (" << (vertex + 1) << "/";

		if(texture_coordinates == NO_TEXTURE_COORDINATES)
			cout << " )" << endl;
		else
			cout << (texture_coordinates + 1) << ")" << endl;
	}

	assert(invariant());
	return id;
}

unsigned int ObjModel :: addFaceSet ()
{
	unsigned int id = m_face_sets.size();
	m_face_sets.push_back(FaceSet());

	if(DEBUGGING)
		cout << "Added Face Set #" << (id + 1) << endl;

	assert(invariant());
	return id;
}

unsigned int ObjModel :: addFace (unsigned int face_set)
{
	assert(face_set < getFaceSetCount());

	unsigned int id = m_face_sets[face_set].m_faces.size();
	m_face_sets[face_set].m_faces.push_back(Face());
	m_valid = false;

	if(DEBUGGING)
		cout << "    Added Face #" << (id + 1) << endl;

	assert(invariant());
	return id;
}

unsigned int ObjModel :: addFaceVertex (unsigned int face_set, unsigned int face, unsigned int vertex, unsigned int texture_coordinates, unsigned int normal)
{
	assert(face_set < getFaceSetCount());
	assert(face < getFaceCount(face_set));
	assert(vertex >= 1);
	assert(texture_coordinates == NO_TEXTURE_COORDINATES || texture_coordinates >= 1);
	assert(vertex == NO_NORMAL || normal >= 1);

	vertex--;
	if(texture_coordinates != NO_TEXTURE_COORDINATES)
		texture_coordinates--;
	if(normal != NO_NORMAL)
		normal--;

	assert(vertex >= 0);
	assert(texture_coordinates == NO_TEXTURE_COORDINATES || texture_coordinates >= 0);
	assert(vertex == NO_NORMAL || normal >= 0);
	unsigned int id = m_face_sets[face_set].m_faces[face].m_vertices.size();
	m_face_sets[face_set].m_faces[face].m_vertices.push_back(FaceVertex(vertex, texture_coordinates, normal));

	if(vertex >= getVertexCount())
		m_valid = false;
	if(texture_coordinates != NO_TEXTURE_COORDINATES && texture_coordinates >= getTextureCoordinateCount())
		m_valid = false;
	if(normal != NO_NORMAL && normal >= getNormalCount())
		m_valid = false;
	if(id > 3)
		m_face_sets[face_set].m_all_triangles = false;

	if(DEBUGGING)
	{
		cout << "        Added vertex #" << (id + 1) << " (" << (vertex + 1) << "/";

		if(texture_coordinates == NO_TEXTURE_COORDINATES)
			cout << "/";
		else
			cout << (texture_coordinates + 1) << "/";

		if(normal == NO_NORMAL)
			cout << " )" << endl;
		else
			cout << (normal + 1) << ")" << endl;
	}

	assert(invariant());
	return id;
}



void ObjModel :: validate ()
{
	bool old_valid = m_valid;

	m_valid = true;

	if(DEBUGGING)
	{
		cout << "Vertices: " << getVertexCount() << endl;
		cout << "Texture Coordinates: " << getTextureCoordinateCount() << endl;
		cout << "Vertex Normals: " << getNormalCount() << endl;

		if(m_material_libraries.size() > 0)
		{
			cout << "Material Libraries:" << endl;
			for(unsigned int i = 0; i < m_material_libraries.size(); i++)
				cout << "    " << m_material_libraries[i].m_name << endl;
		}
	}

	for(unsigned int p = 0; p < m_point_sets.size(); p++)
	{
		if(m_point_sets[p].m_vertices.size() < 1)
		{
			m_valid = false;

			assert(old_valid == false);
			assert(invariant());
			return;
		}

		for(unsigned int v = 0; v < m_point_sets[p].m_vertices.size(); v++)
		{
			unsigned int vertex = m_point_sets[p].m_vertices[v];

			if(vertex >= getVertexCount())
			{
				if(DEBUGGING)
					cout << "Invalid vertex: Point Set " << p << ", vertex " << v << " has vertex " << (vertex + 1) << endl;
				m_valid = false;

				assert(old_valid == false);
				assert(invariant());
				return;
			}
		}
	}

	for(unsigned int l = 0; l < m_lines.size(); l++)
	{
		if(m_lines[l].m_vertices.size() < 2)
		{
			m_valid = false;

			assert(old_valid == false);
			assert(invariant());
			return;
		}

		for(unsigned int v = 0; v < m_lines[l].m_vertices.size(); v++)
		{
			unsigned int vertex = m_lines[l].m_vertices[v].m_vertex;
			unsigned int texture_coordinates = m_lines[l].m_vertices[v].m_texture_coordinates;

			if(vertex >= getVertexCount())
			{
				if(DEBUGGING)
					cout << "Invalid vertex: Line " << l << ", vertex " << v << " has vertex " << (vertex + 1) << endl;
				m_valid = false;

				assert(old_valid == false);
				assert(invariant());
				return;
			}

			if(texture_coordinates >= getTextureCoordinateCount() && texture_coordinates != NO_TEXTURE_COORDINATES)
			{
				if(DEBUGGING)
					cout << "Invalid texture coordinates: Line " << l << ", vertex " << v << " has texture coordinates " << (texture_coordinates + 1) << endl;
				m_valid = false;

				assert(old_valid == false);
				assert(invariant());
				return;
			}
		}
	}

	for(unsigned int s = 0; s < m_face_sets.size(); s++)
	{
/*
		// bad materials cause too many crashes
		if(m_face_sets[s].m_material_name.compare("") != 0 && m_face_sets[s].mp_material == NULL)
		{
			if(DEBUGGING)
				cout << "Invalid material: Face set " << s << " has non-existant material \"" << m_face_sets[s].m_material_name << "\"" << endl;
			m_valid = false;

			assert(old_valid == false);
			assert(invariant());
			return;
		}
*/
		m_face_sets[s].m_all_triangles = true;
		for(unsigned int f = 0; f < m_face_sets[s].m_faces.size(); f++)
		{
			unsigned int face_vertex_count = m_face_sets[s].m_faces[f].m_vertices.size();
			if(face_vertex_count < 3)
			{
				m_valid = false;

				assert(old_valid == false);
				assert(invariant());
				return;
			}
			else if(face_vertex_count > 3)
				m_face_sets[s].m_all_triangles = false;

			for(unsigned int v = 0; v < face_vertex_count; v++)
			{
				unsigned int vertex = m_face_sets[s].m_faces[f].m_vertices[v].m_vertex;
				unsigned int texture_coordinates = m_face_sets[s].m_faces[f].m_vertices[v].m_texture_coordinates;
				unsigned int normal = m_face_sets[s].m_faces[f].m_vertices[v].m_normal;

				if(vertex >= getVertexCount())
				{
					if(DEBUGGING)
						cout << "Invalid vertex: Face set " << s << ", face " << f << ", vertex " << v << " has vertex " << (vertex + 1) << endl;
					m_valid = false;

					assert(old_valid == false);
					assert(invariant());
					return;
				}

				if(texture_coordinates >= getTextureCoordinateCount() && texture_coordinates != NO_TEXTURE_COORDINATES)
				{
					if(DEBUGGING)
						cout << "Invalid texture coordinates: Face set " << s << ", face " << f << ", vertex " << v << " has texture coordinates " << (texture_coordinates + 1) << endl;
					m_valid = false;

					assert(old_valid == false);
					assert(invariant());
					return;
				}

				if(normal >= getNormalCount() && normal != NO_NORMAL)
				{
					if(DEBUGGING)
						cout << "Invalid normal: Face set " << s << ", face " << f << ", vertex " << v << " has normal " << (normal + 1) << endl;
					m_valid = false;

					assert(old_valid == false);
					assert(invariant());
					return;
				}
			}
		}
	}

	assert(invariant());
}



void ObjModel :: drawPointSets () const
{
	if(getPointSetCount() > 0)
	{
		glBegin(GL_POINTS);
			for(unsigned int p = 0; p < getPointSetCount(); p++)
				for(unsigned int v = 0; v < getPointSetVertexCount(p); v++)
				{
					unsigned int vertex = m_point_sets[p].m_vertices[v];

					glVertex3d(m_vertices[vertex].x,
						   m_vertices[vertex].y,
						   m_vertices[vertex].z);
				}
		glEnd();
	}
}

void ObjModel :: drawLines () const
{
	for(unsigned int l = 0; l < getLineCount(); l++)
	{
		glBegin(GL_LINE_STRIP);
			for(unsigned int v = 0; v < getLineVertexCount(l); v++)
			{
				unsigned int vertex = m_lines[l].m_vertices[v].m_vertex;
				unsigned int texture_coordinates = m_lines[l].m_vertices[v].m_texture_coordinates;

				if(texture_coordinates != NO_TEXTURE_COORDINATES)
				{
					glTexCoord2d(m_texture_coordinates[texture_coordinates].x,
						     m_texture_coordinates[texture_coordinates].y);
				}

				glVertex3d(m_vertices[vertex].x,
					   m_vertices[vertex].y,
					   m_vertices[vertex].z);
			}
		glEnd();
	}
}

void ObjModel :: drawFaceSet (unsigned int face_set) const
{
	assert(isValid());
	assert(face_set < getFaceSetCount());

	bool is_all_triangles = m_face_sets[face_set].m_all_triangles;

	// if everything is triangles, draw everything as one triangle group
	if(is_all_triangles)
		glBegin(GL_TRIANGLES);

	for(unsigned int f = 0; f < getFaceCount(face_set); f++)
	{
		// otherwise, draw as lots of trinagle fans
		if(!is_all_triangles)
			glBegin(GL_TRIANGLE_FAN);

		for(unsigned int v = 0; v < getFaceVertexCount(face_set, f); v++)
		{
			unsigned int vertex = m_face_sets[face_set].m_faces[f].m_vertices[v].m_vertex;
			unsigned int texture_coordinates = m_face_sets[face_set].m_faces[f].m_vertices[v].m_texture_coordinates;
			unsigned int normal = m_face_sets[face_set].m_faces[f].m_vertices[v].m_normal;

			if(normal != NO_NORMAL)
			{
				glNormal3d(m_normals[normal].x,
					   m_normals[normal].y,
					   m_normals[normal].z);
			}

			if(texture_coordinates != NO_TEXTURE_COORDINATES)
			{
				// flip texture coordinates to match Maya <|>
				glTexCoord2d(      m_texture_coordinates[texture_coordinates].x,
				             1.0 - m_texture_coordinates[texture_coordinates].y);
			}

			glVertex3d(m_vertices[vertex].x,
				   m_vertices[vertex].y,
				   m_vertices[vertex].z);
		}

		// end of current trinagle fan
		if(!is_all_triangles)
			glEnd();
	}

	// end of trinagles
	if(is_all_triangles)
		glEnd();
}

void ObjModel :: drawFaceSetMaterial (unsigned int face_set, const Material* p_material) const
{
	assert(isValid());
	assert(face_set < getFaceSetCount());
	assert(!Material::isMaterialActive());

	if(p_material == NULL)
		drawFaceSet(face_set);
	else
	{
		p_material->activate();
		drawFaceSet(face_set);
		Material::deactivate();

		if(p_material->isSeperateSpecular())
		{
			p_material->activateSeperateSpecular();
			drawFaceSet(face_set);
			Material::deactivate();
		}
	}

	assert(!Material::isMaterialActive());
}


bool ObjModel :: readMaterialLibrary (const string& str, ostream& r_logstream)
{
	string::size_type start_index;

	if(isspace(str[0]))
		start_index = nextToken(str, 0);
	else
		start_index = 0;

	for(string::size_type token_index = start_index; token_index != string::npos; token_index = nextToken(str, token_index))
	{
		string library;

		size_t token_length = getTokenLength(str, token_index);

		if(token_length == 0)
			return false;

		library = str.substr(token_index, token_length);

		assert(library.compare("") != 0);
		addMaterialLibrary(library, r_logstream);
	}

	// if we get here, all tokens were valid
	return true;
}

bool ObjModel :: readMaterial (const string& str, ostream& r_logstream)
{
	string material;
	unsigned int face_set_index;

	size_t length;
	size_t start_index;
	size_t end_index;

	if(isspace(str[0]))
		start_index = nextToken(str, 0);
	else
		start_index = 0;

	length = str.length();
	for(end_index = start_index; end_index < length && !isspace(str[end_index]); end_index++)
		; // do nothing

	if(end_index < length)
		material = str.substr(start_index, end_index - start_index);
	else
		material = str.substr(start_index);

	face_set_index = addFaceSet();
	setFaceSetMaterial(face_set_index, material);
	return true;
}

bool ObjModel :: readVertex (const string& str, ostream& r_logstream)
{
	double x;
	double y;
	double z;

	size_t index;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	x = atof(str.c_str() + index);

	index = nextToken(str, index);
	if(index == string::npos)
		return false;

	y = atof(str.c_str() + index);

	index = nextToken(str, index);
	if(index == string::npos)
		return false;

	z = atof(str.c_str() + index);

	addVertex(x, y, z);
	return true;
}

bool ObjModel :: readTextureCoordinates (const string& str, ostream& r_logstream)
{
	double u;
	double v;

	size_t index;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	u = atof(str.c_str() + index);

	index = nextToken(str, index);
	if(index == string::npos)
		return false;

	v = atof(str.c_str() + index);

	addTextureCoordinate(u, v);
	return true;
}

bool ObjModel :: readNormal (const string& str, ostream& r_logstream)
{
	double x;
	double y;
	double z;

	size_t index;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	x = atof(str.c_str() + index);

	index = nextToken(str, index);
	if(index == string::npos)
		return false;

	y = atof(str.c_str() + index);

	index = nextToken(str, index);
	if(index == string::npos)
		return false;

	z = atof(str.c_str() + index);

	if(x == 0.0 && y == 0.0 && z == 0.0)
	{
		r_logstream << "Found a bad normal: #" << getNormalCount() << ", using (0, 0, 1)" << endl;
		addNormal(0.0, 0.0, 1.0);
	}
	else
		addNormal(x, y, z);
	return true;
}

bool ObjModel :: readPointSet (const string& str, ostream& r_logstream)
{
	const unsigned int NO_POINT_SET = ~0u;

	unsigned int point_set_index = NO_POINT_SET;
	string::size_type start_index;

	if(isspace(str[0]))
		start_index = nextToken(str, 0);
	else
		start_index = 0;

	for(string::size_type token_index = start_index; token_index != string::npos; token_index = nextToken(str, token_index))
	{
		int vertex;

		vertex = atoi(str.c_str() + token_index);
		if(vertex < 0)
			vertex += getVertexCount() + 1;
		if(vertex <= 0)
		{
			if(point_set_index != NO_POINT_SET)
				removeLastPointSet();
			return false;
		}

		if(point_set_index == NO_POINT_SET)
			point_set_index = addPointSet();

		assert(vertex >= 1);
		addPointSetVertex(point_set_index, vertex);
	}

	// if we get here, all tokens were valid
	return true;
}

bool ObjModel :: readLine (const string& str, ostream& r_logstream)
{
	//
	//  This function reads a line of vertices in the model,
	//    not a line of the input file.
	//

	const unsigned int NO_LINE = ~0u;

	unsigned int line_index = NO_LINE;
	string::size_type start_index;

	if(isspace(str[0]))
		start_index = nextToken(str, 0);
	else
		start_index = 0;

	for(string::size_type token_index = start_index; token_index != string::npos; token_index = nextToken(str, token_index))
	{
		size_t number_index;

		int vertex;
		int texture_coordinates;

		number_index = token_index;

		vertex = atoi(str.c_str() + number_index);
		if(vertex < 0)
			vertex += getVertexCount() + 1;
		if(vertex <= 0)
		{
			if(line_index != NO_LINE)
				removeLastLine();
			return false;
		}

		number_index = nextSlashInToken(str, number_index);
		if(number_index == string::npos)
		{
			texture_coordinates = NO_TEXTURE_COORDINATES;
		}
		else
		{
			number_index++;

			if(isspace(str[number_index]))
				texture_coordinates = NO_TEXTURE_COORDINATES;
			else
			{
				texture_coordinates = atoi(str.c_str() + number_index);
				if(texture_coordinates < 0)
					texture_coordinates += getTextureCoordinateCount() + 1;
				if(texture_coordinates <= 0)
					return false;
			}
		}

		if(line_index == NO_LINE)
			line_index = addLine();

		assert(vertex >= 1);
		assert(texture_coordinates >= 1 || texture_coordinates == NO_TEXTURE_COORDINATES);
		addLineVertex(line_index, vertex, texture_coordinates);
	}

	// if we get here, all tokens were valid
	return true;
}

bool ObjModel :: readFace (const string& str, ostream& r_logstream)
{
	const unsigned int NO_FACE = ~0u;

	unsigned int face_index = NO_FACE;
	unsigned int face_set_index;

	string::size_type start_index;

	if(isspace(str[0]))
		start_index = nextToken(str, 0);
	else
		start_index = 0;

	if(m_face_sets.empty())
		face_set_index = addFaceSet();
	else
		face_set_index = m_face_sets.size() - 1;

	for(string::size_type token_index = start_index; token_index != string::npos; token_index = nextToken(str, token_index))
	{
		size_t number_index;

		int vertex;
		int texture_coordinates;
		int normal;

		number_index = token_index;

		vertex = atoi(str.c_str() + number_index);
		if(vertex < 0)
			vertex += getVertexCount() + 1;
		if(vertex <= 0)
		{
			if(face_index != NO_FACE)
				removeLastFace(face_set_index);
			return false;
		}

		number_index = nextSlashInToken(str, number_index);
		if(number_index == string::npos)
		{
			texture_coordinates = NO_TEXTURE_COORDINATES;
			normal = NO_NORMAL;
		}
		else
		{
			number_index++;

			if(str[number_index] == '/')
				texture_coordinates = NO_TEXTURE_COORDINATES;
			else
			{
				texture_coordinates = atoi(str.c_str() + number_index);
				if(texture_coordinates < 0)
					texture_coordinates += getTextureCoordinateCount() + 1;
				if(texture_coordinates <= 0)
					return false;
			}

			number_index = nextSlashInToken(str, number_index);
			if(number_index == string::npos)
				normal = NO_NORMAL;
			else
			{
				number_index++;

				if(isspace(str[number_index]))
					normal = NO_NORMAL;
				else
				{
					normal = atoi(str.c_str() + number_index);
					if(normal < 0)
						normal += getNormalCount() + 1;
					if(normal <= 0)
						return false;
				}
			}
		}

		if(face_index == NO_FACE)
			face_index = addFace(face_set_index);

		assert(vertex >= 1);
		assert(texture_coordinates >= 1 || texture_coordinates == NO_TEXTURE_COORDINATES);
		assert(normal >= 1 || normal == NO_NORMAL);
		addFaceVertex(face_set_index, face_index, vertex, texture_coordinates, normal);
	}

	// if we get here, all tokens were valid
	return true;
}

void ObjModel :: removeLastPointSet ()
{
	assert(getPointSetCount() >= 1);

	m_point_sets.pop_back();
	m_valid = false;
}

void ObjModel :: removeLastLine ()
{
	assert(getLineCount() >= 1);

	m_lines.pop_back();
	m_valid = false;
}

void ObjModel :: removeLastFace (unsigned int face_set)
{
	assert(face_set < getFaceSetCount());
	assert(getFaceCount(face_set) >= 1);

	m_face_sets[face_set].m_faces.pop_back();
	m_valid = false;
}

bool ObjModel :: invariant () const
{
	if(m_name.compare("") == 0) return false;
	return true;
}





ObjModel :: MaterialLibrary :: MaterialLibrary ()
{
	m_name = "";
	mp_mtl_library = NULL;
}

ObjModel :: MaterialLibrary :: MaterialLibrary (const string& name, ostream& r_logstream)
{
	m_name = name;
	mp_mtl_library = &(MtlLibraryManager::get(name, r_logstream));
}

ObjModel :: MaterialLibrary :: MaterialLibrary (const ObjModel :: MaterialLibrary& original)
{
	m_name = original.m_name;
	mp_mtl_library = original.mp_mtl_library;
}

ObjModel :: MaterialLibrary& ObjModel :: MaterialLibrary :: operator= (const ObjModel :: MaterialLibrary& original)
{
	if(&original != this)
	{
		m_name = original.m_name;
		mp_mtl_library = original.mp_mtl_library;
	}

	return *this;
}





ObjModel :: LineVertex :: LineVertex ()
{
	m_vertex = 0;
	m_texture_coordinates = NO_TEXTURE_COORDINATES;
}

ObjModel :: LineVertex :: LineVertex (unsigned int vertex, unsigned int texture_coordinates)
{
	m_vertex = vertex;
	m_texture_coordinates = texture_coordinates;
}

ObjModel :: LineVertex :: LineVertex (const ObjModel :: LineVertex& original)
{
	m_vertex = original.m_vertex;
	m_texture_coordinates = original.m_texture_coordinates;
}

ObjModel :: LineVertex& ObjModel :: LineVertex :: operator= (const ObjModel :: LineVertex& original)
{
	if(&original != this)
	{
		m_vertex = original.m_vertex;
		m_texture_coordinates = original.m_texture_coordinates;
	}

	return *this;
}





ObjModel :: FaceVertex :: FaceVertex ()
{
	m_vertex = 0;
	m_texture_coordinates = NO_TEXTURE_COORDINATES;
	m_normal = NO_NORMAL;
}

ObjModel :: FaceVertex :: FaceVertex (unsigned int vertex, unsigned int texture_coordinates, unsigned int normal)
{
	m_vertex = vertex;
	m_texture_coordinates = texture_coordinates;
	m_normal = normal;
}

ObjModel :: FaceVertex :: FaceVertex (const ObjModel :: FaceVertex& original)
{
	m_vertex = original.m_vertex;
	m_texture_coordinates = original.m_texture_coordinates;
	m_normal = original.m_normal;
}

ObjModel :: FaceVertex& ObjModel :: FaceVertex :: operator= (const ObjModel :: FaceVertex& original)
{
	if(&original != this)
	{
		m_vertex = original.m_vertex;
		m_texture_coordinates = original.m_texture_coordinates;
		m_normal = original.m_normal;
	}

	return *this;
}




ObjModel :: FaceSet :: FaceSet () : m_faces()
{
	m_material_name = "";
	mp_material = NULL;
	m_all_triangles = true;
}

ObjModel :: FaceSet :: FaceSet (const string& material_name, Material* p_material) : m_faces()
{
	m_material_name = material_name;
	mp_material = p_material;
	m_all_triangles = true;
}

ObjModel :: FaceSet :: FaceSet (const ObjModel :: FaceSet& original) : m_faces(original.m_faces)
{
	m_material_name = original.m_material_name;
	mp_material = original.mp_material;
	m_all_triangles = original.m_all_triangles;
}

ObjModel :: FaceSet& ObjModel :: FaceSet :: operator= (const ObjModel :: FaceSet& original)
{
	if(&original != NULL)
	{
		m_material_name = original.m_material_name;
		mp_material = original.mp_material;
		m_faces = original.m_faces;
		m_all_triangles = original.m_all_triangles;
	}

	return *this;
}
