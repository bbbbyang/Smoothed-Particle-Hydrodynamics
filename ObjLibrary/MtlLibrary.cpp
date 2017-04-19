//
//  MtlLibrary.cpp
//
//  This file is part of the ObjLibrary, by Richard Hamilton,
//    which is copyright Hamilton 2009-2014.
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
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cctype>
#include <cstdlib>
#include <vector>

#include "ObjStringParsing.h"
#include "Material.h"
#include "MtlLibrary.h"

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

	const unsigned int COLOUR_TARGET_TYPES    = 3;
	const unsigned int COLOUR_TARGET_AMBIENT  = 0;
	const unsigned int COLOUR_TARGET_DIFFUSE  = 1;
	const unsigned int COLOUR_TARGET_SPECULAR = 2;

	const unsigned int CHANNEL_TARGET_TYPES             = 5;
	const unsigned int CHANNEL_TARGET_SPECULAR_EXPONENT = 0;
	const unsigned int CHANNEL_TARGET_TRANSPARENCY      = 1;
	const unsigned int CHANNEL_TARGET_DECAL             = 2;
	const unsigned int CHANNEL_TARGET_DISPLACEMENT      = 3;
	const unsigned int CHANNEL_TARGET_BUMP              = 4;

}



const unsigned int MtlLibrary :: NO_SUCH_MATERIAL = ~0u;



MtlLibrary :: MtlLibrary () : m_materials ()
{
	m_name = DEFAULT_NAME;

	assert(invariant());
}

MtlLibrary :: MtlLibrary (const string& filename)
{
	assert(filename.compare("") != 0);

	m_name = DEFAULT_NAME;
	load(filename);

	assert(invariant());
}

MtlLibrary :: MtlLibrary (const string& filename, const string& logfile)
{
	assert(filename.compare("") != 0);
	assert(logfile.compare("") != 0);

	m_name = DEFAULT_NAME;
	load(filename, logfile);

	assert(invariant());
}

MtlLibrary :: MtlLibrary (const string& filename, ostream& r_logstream)
{
	assert(filename.compare("") != 0);

	m_name = DEFAULT_NAME;
	load(filename, r_logstream);

	assert(invariant());
}

MtlLibrary :: MtlLibrary (const MtlLibrary& original) : m_materials()
{
	copy(original);

	assert(invariant());
}

MtlLibrary& MtlLibrary :: operator= (const MtlLibrary& original)
{
	if(&original != this)
	{
		removeAll();
		copy(original);
	}

	assert(invariant());
	return *this;
}

MtlLibrary :: ~MtlLibrary ()
{
	removeAll();
}



const string& MtlLibrary :: getName () const
{
	return m_name;
}

unsigned int MtlLibrary :: getMaterialCount () const
{
	return m_materials.size();
}

bool MtlLibrary :: isMaterial (const string& name) const
{
	assert(name.compare("") != 0);

	string lower = toLowercase(name);

	for(unsigned int i = 0; i < m_materials.size(); i++)
		if(m_materials[i]->getName().compare(lower) == 0)
			return true;

	return false;
}

unsigned int MtlLibrary :: getMaterialIndex (const string& name) const
{
	assert(name.compare("") != 0);

	string lower = toLowercase(name);

	for(unsigned int i = 0; i < m_materials.size(); i++)
		if(m_materials[i]->getName().compare(lower) == 0)
			return i;

	return NO_SUCH_MATERIAL;
}

const string& MtlLibrary :: getMaterialName (unsigned int index) const
{
	assert(index < getMaterialCount());

	return m_materials[index]->getName();
}

Material* MtlLibrary :: getMaterial (unsigned int index)
{
	assert(index < getMaterialCount());

	return m_materials[index];
}

const Material* MtlLibrary :: getMaterial (unsigned int index) const
{
	assert(index < getMaterialCount());

	return m_materials[index];
}

Material* MtlLibrary :: getMaterial (const string& name)
{
	assert(name.compare("") != 0);

	unsigned int index = getMaterialIndex(name);

	if(index == NO_SUCH_MATERIAL)
		return NULL;
	else
		return m_materials[index];
}

const Material* MtlLibrary :: getMaterial (const string& name) const
{
	assert(name.compare("") != 0);

	unsigned int index = getMaterialIndex(name);

	if(index == NO_SUCH_MATERIAL)
		return NULL;
	else
		return m_materials[index];
}

void MtlLibrary :: print () const
{
	print(cout);
}

void MtlLibrary :: print (ostream& r_outstream) const
{
	r_outstream << m_name << ":" << endl;

	for(unsigned int i = 0; i < m_materials.size(); i++)
	{
		m_materials[i]->print();
		r_outstream << endl;
	}
}

void MtlLibrary :: save (const string& filename) const
{
	assert(filename.compare("") != 0);

	save(filename, cerr);
}

void MtlLibrary :: save (const string& filename, const string& logfile) const
{
	assert(filename.compare("") != 0);
	assert(logfile.compare("") != 0);

	ofstream logstream;

	logstream.open(logfile.c_str());
	save(filename, logstream);
	logstream.close();
}

void MtlLibrary :: save (const string& filename, ostream& r_logstream) const
{
	assert(filename.compare("") != 0);

	ofstream output_file;

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
	//    -> how many materials
	//  Materials
	//  Footer
	//
	//  Sections are seperated by 3 blank lines.
	//

	output_file << "#" << endl;
	output_file << "# " << getName() << endl;
	output_file << "#" << endl;
	output_file << "# " << getMaterialCount() << " materials" << endl;
	for(unsigned int i = 0; i < m_materials.size(); i++)
		output_file << "#   " << m_materials[i]->getName() << endl;
	output_file << "#" << endl;
	output_file << endl;
	output_file << endl;
	output_file << endl;

	for(unsigned int i2 = 0; i2 < m_materials.size(); i2++)
	{
		m_materials[i2]->save(output_file);
		output_file << endl;
		output_file << endl;
	}

	output_file << "# End of " << getName() << endl;
	output_file << endl;

	output_file.close();
}



void MtlLibrary :: load (const string& filename)
{
	assert(filename.compare("") != 0);

	load(filename, cerr);

	assert(invariant());
}

void MtlLibrary :: load (const string& filename, const string& logfile)
{
	assert(filename.compare("") != 0);
	assert(logfile.compare("") != 0);

	ofstream logstream;

	logstream.open(logfile.c_str());
	load(filename, logstream);
	logstream.close();

	assert(invariant());
}

void MtlLibrary :: load (const string& filename, ostream& r_logstream)
{
	assert(filename.compare("") != 0);

	ifstream input_file;
	unsigned int line_count;

	removeAll();

	m_name = toLowercase(filename);
	input_file.open(filename.c_str());
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
	//  http://paulbourke.net/dataformats/mtl/
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

		if(isspace(line[0]))
			line = line.substr(nextToken(line, 0));

		valid = true;
		if(length < 3)
			valid = false;
		else
		{
			if(line.length() >= 7 && line.substr(0, 6).compare("newmtl") == 0 && isspace(line[6]))
				valid = readMaterialStart(line.substr(7), r_logstream);
			else if(line.length() >= 6 && line.substr(0, 5).compare("illum") == 0 && isspace(line[5]))
				valid = readIlluminationMode(line.substr(6), r_logstream);
			else if(line[0] == 'K' && line[1] == 'a' && isspace(line[2]))
				valid = readColour(line.substr(3), COLOUR_TARGET_AMBIENT, r_logstream);
			else if(line[0] == 'K' && line[1] == 'd' && isspace(line[2]))
				valid = readColour(line.substr(3), COLOUR_TARGET_DIFFUSE, r_logstream);
			else if(line[0] == 'K' && line[1] == 's' && isspace(line[2]))
				valid = readColour(line.substr(3), COLOUR_TARGET_SPECULAR, r_logstream);
			else if(line[0] == 'N' && line[1] == 's' && isspace(line[2]))
				valid = readSpecularExponent(line.substr(3), r_logstream);
			else if(line[0] == 'd' && isspace(line[1]))
				valid = readTransparency(line.substr(2), r_logstream);
			else if(line[0] == 'T' && line[1] == 'r' && isspace(line[2]))
				valid = readTransparency(line.substr(3), r_logstream);
			else if(line[0] == 'N' && line[1] == 'i' && isspace(line[2]))
				valid = readOpticalDensity(line.substr(3), r_logstream);
			else if(line[0] == 'T' && line[1] == 'f' && isspace(line[2]))
				valid = readTransmissionFilter(line.substr(3), r_logstream);
			else if(line.length() >= 7 && line.substr(0, 6).compare("map_Ka") == 0 && isspace(line[6]))
				valid = readMapColour(line.substr(7), COLOUR_TARGET_AMBIENT, r_logstream);
			else if(line.length() >= 7 && line.substr(0, 6).compare("map_Kd") == 0 && isspace(line[6]))
				valid = readMapColour(line.substr(7), COLOUR_TARGET_DIFFUSE, r_logstream);
			else if(line.length() >= 7 && line.substr(0, 6).compare("map_Ks") == 0 && isspace(line[6]))
				valid = readMapColour(line.substr(7), COLOUR_TARGET_SPECULAR, r_logstream);
			else if(line.length() >= 7 && line.substr(0, 6).compare("map_Ns") == 0 && isspace(line[6]))
				valid = readMapChannel(line.substr(7), CHANNEL_TARGET_SPECULAR_EXPONENT, r_logstream);
			else if(line.length() >= 7 && line.substr(0, 6).compare("map_Ns") == 0 && isspace(line[6]))
				valid = readMapChannel(line.substr(7), CHANNEL_TARGET_SPECULAR_EXPONENT, r_logstream);
			else if(line.length() >= 6 && line.substr(0, 5).compare("map_d") == 0 && isspace(line[5]))
				valid = readMapChannel(line.substr(6), CHANNEL_TARGET_TRANSPARENCY, r_logstream);
			else if(line.length() >= 7 && line.substr(0, 6).compare("map_Tr") == 0 && isspace(line[6]))
				valid = readMapChannel(line.substr(7), CHANNEL_TARGET_TRANSPARENCY, r_logstream);
			else if(line.length() >= 6 && line.substr(0, 5).compare("decal") == 0 && isspace(line[5]))
				valid = readMapChannel(line.substr(6), CHANNEL_TARGET_DECAL, r_logstream);
			else if(line.length() >= 5 && line.substr(0, 4).compare("disp") == 0 && isspace(line[4]))
				valid = readMapChannel(line.substr(5), CHANNEL_TARGET_DISPLACEMENT, r_logstream);
			else if(line.length() >= 5 && line.substr(0, 4).compare("bump") == 0 && isspace(line[4]))
				valid = readMapChannel(line.substr(5), CHANNEL_TARGET_BUMP, r_logstream);
			else if(line[0] == 'K' && line[1] == 'm' && isspace(line[2]))	// non-standard - is this what it does?
				valid = readBumpMapMultiplier(line.substr(3), r_logstream);
			else
				valid = false;
		}

		if(!valid)
			r_logstream << "Line " << setw(6) << line_count << " of file \"" << filename << "\" is invalid: \"" << line << "\"" << endl;
	}

	warnIfLastMaterialIsInvisible(r_logstream);
	input_file.close();

	assert(invariant());
}

void MtlLibrary :: setName (const string& name)
{
	assert(name.compare("") != 0);

	m_name = toLowercase(name);

	assert(invariant());
}

unsigned int MtlLibrary :: add (Material* p_material)
{
	assert(p_material != NULL);
	assert(!isMaterial(p_material->getName()));

	unsigned int index = m_materials.size();
	m_materials.push_back(p_material);

	assert(invariant());
	return index;
}

void MtlLibrary :: removeAll ()
{
	for(unsigned int i = 0; i < m_materials.size(); i++)
		delete m_materials[i];
	m_materials.clear();

	assert(m_materials.size() == 0);
	assert(invariant());
}

void MtlLibrary :: loadDisplayTextures ()
{
	for(unsigned int i = 0; i < m_materials.size(); i++)
		m_materials[i]->loadDisplayTextures();

	assert(invariant());
}

void MtlLibrary :: loadAllTextures ()
{
	for(unsigned int i = 0; i < m_materials.size(); i++)
		m_materials[i]->loadAllTextures();

	assert(invariant());
}



void MtlLibrary :: warnIfLastMaterialIsInvisible (ostream& r_outstream) const
{
	if(m_materials.empty())
		return;

	unsigned int current_material;

	assert(m_materials.size() >= 1);
	current_material = m_materials.size() - 1;
	if(m_materials[current_material]->getTransparency() == 0.0)
		r_outstream << "Material " << m_materials[current_material]->getName() << " in file " << m_name << " is entirely transprent" << endl;
}

bool MtlLibrary :: readMaterialStart (const string& str, ostream& r_logstream)
{
	size_t start;
	size_t length;
	string name;

	warnIfLastMaterialIsInvisible(r_logstream);

	if(isspace(str[0]))
		start = nextToken(str, 0);
	else
		start = 0;

	length = getTokenLength(str, start);
	name = str.substr(start, length);

	if(name.compare("") == 0 || isMaterial(name))
		return false;

	add(new Material(toLowercase(name)));
	return true;
}

bool MtlLibrary :: readIlluminationMode (const string& str, ostream& r_logstream)
{
	size_t index;

	unsigned int illumination_mode;
	unsigned int current_material;

	current_material = m_materials.size() - 1;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	switch(atoi(str.c_str() + index))
	{
	case 0: illumination_mode = Material::ILLUMINATION_CONSTANT; break;
	case 1: illumination_mode = Material::ILLUMINATION_PHONG_NO_SPECULAR; break;
	case 2: illumination_mode = Material::ILLUMINATION_PHONG; break;
	case 3: illumination_mode = Material::ILLUMINATION_RAY_REFLECTIVE; break;
	case 4: illumination_mode = Material::ILLUMINATION_RAY_GLASS; break;
	case 5: illumination_mode = Material::ILLUMINATION_RAY_FRESNEL; break;
	case 6: illumination_mode = Material::ILLUMINATION_RAY_REFRACTION; break;
	case 7: illumination_mode = Material::ILLUMINATION_RAY_FRESNEL_REFRACTION; break;
	case 8: illumination_mode = Material::ILLUMINATION_REFLECTIVE; break;
	case 9: illumination_mode = Material::ILLUMINATION_GLASS; break;
	case 10: illumination_mode = Material::ILLUMINATION_RAY_INVISIBLE_SHADOWS; break;
	default: return false;
	}

	m_materials[current_material]->setIlluminationMode(illumination_mode);
	return true;
}

bool MtlLibrary :: readColour (const string& str, unsigned int target, ostream& r_logstream)
{
	assert(target < COLOUR_TARGET_TYPES);

	double red;
	double green;
	double blue;

	size_t index;
	unsigned int current_material;

	current_material = m_materials.size() - 1;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	red = atof(str.c_str() + index);

	index = nextToken(str, index);
	if(index == string::npos)
	{
		//  Providing only one value for a colour is legal.
		//    Greyscale is assumed.

		switch(target)
		{
		case COLOUR_TARGET_AMBIENT:
			m_materials[current_material]->setAmbientColour(red, red, red);
			break;
		case COLOUR_TARGET_DIFFUSE:
			m_materials[current_material]->setDiffuseColour(red, red, red);
			break;
		case COLOUR_TARGET_SPECULAR:
			m_materials[current_material]->setSpecularColour(red, red, red);
			break;
		}

		return true;
	}

	green = atof(str.c_str() + index);

	index = nextToken(str, index);
	if(index == string::npos)
		return false;

	blue = atof(str.c_str() + index);

	switch(target)
	{
	case COLOUR_TARGET_AMBIENT:
		m_materials[current_material]->setAmbientColour(red, green, blue);
		break;
	case COLOUR_TARGET_DIFFUSE:
		m_materials[current_material]->setDiffuseColour(red, green, blue);
		break;
	case COLOUR_TARGET_SPECULAR:
		m_materials[current_material]->setSpecularColour(red, green, blue);
		break;
	}

	return true;
}

bool MtlLibrary :: readSpecularExponent (const string& str, ostream& r_logstream)
{
	double exponent;

	size_t index;
	unsigned int current_material;

	current_material = m_materials.size() - 1;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	exponent = atof(str.c_str() + index);

	m_materials[current_material]->setSpecularExponent(exponent);
	return true;
}

bool MtlLibrary :: readTransparency (const string& str, ostream& r_logstream)
{
	double transparency;

	size_t index;
	unsigned int current_material;

	current_material = m_materials.size() - 1;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	transparency = atof(str.c_str() + index);
	if(transparency < 0.0)
		return false;
	if(transparency > 1.0)
		return false;

	m_materials[current_material]->setTransparency(transparency);
	return true;
}

bool MtlLibrary :: readOpticalDensity (const std::string& str, std::ostream& r_logstream)
{
	double optical_density;

	size_t index;
	unsigned int current_material;

	current_material = m_materials.size() - 1;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	optical_density = atof(str.c_str() + index);

	m_materials[current_material]->setOpticalDensity(optical_density);
	return true;
}

bool MtlLibrary :: readTransmissionFilter (const string& str, ostream& r_logstream)
{
	double red;
	double green;
	double blue;

	size_t index;
	unsigned int current_material;

	current_material = m_materials.size() - 1;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	red = atof(str.c_str() + index);

	index = nextToken(str, index);
	if(index == string::npos)
	{
		//  Providing only one value for a colour is legal.
		//    Greyscale is assumed.

		m_materials[current_material]->setTransmissionFilter(red, red, red);
		return true;
	}

	green = atof(str.c_str() + index);

	index = nextToken(str, index);
	if(index == string::npos)
		return false;

	blue = atof(str.c_str() + index);

	m_materials[current_material]->setTransmissionFilter(red, green, blue);
	return true;
}

bool MtlLibrary :: readBumpMapMultiplier (const std::string& str, std::ostream& r_logstream)
{
	double multiplier;

	size_t index;
	unsigned int current_material;

	current_material = m_materials.size() - 1;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	multiplier = atof(str.c_str() + index);

	m_materials[current_material]->setBumpMapMultiplier(multiplier);
	return true;
}



bool MtlLibrary :: readMapColour (const string& str, unsigned int target, ostream& r_logstream)
{
	assert(target < COLOUR_TARGET_TYPES);

	string filename;

	size_t index;
	unsigned int current_material;

	current_material = m_materials.size() - 1;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	filename = str.substr(index, getTokenLength(str, index));
	if(filename.compare("") == 0 || isMaterial(filename))
		return false;

	switch(target)
	{
	case COLOUR_TARGET_AMBIENT:
		m_materials[current_material]->setAmbientMap(filename);
		break;
	case COLOUR_TARGET_DIFFUSE:
		m_materials[current_material]->setDiffuseMap(filename);
		break;
	case COLOUR_TARGET_SPECULAR:
		m_materials[current_material]->setSpecularMap(filename);
		break;
	}

	return true;
}

bool MtlLibrary :: readMapChannel (const string& str, unsigned int target, ostream& r_logstream)
{
	assert(target < CHANNEL_TARGET_TYPES);

	string filename;
	unsigned char channel;
	double bump_multiplier = 1.0;

	size_t index;
	unsigned int current_material;

	current_material = m_materials.size() - 1;

	if(target == CHANNEL_TARGET_DECAL)
		channel = Material::CHANNEL_MATTE;
	else
		channel = Material::CHANNEL_LUMINANCE;

	if(isspace(str[0]))
		index = nextToken(str, 0);
	else
		index = 0;

	filename = str.substr(index, getTokenLength(str, index));
	if(filename.compare("") == 0 || isMaterial(filename))
		return false;

	index = nextToken(str, index);
	while(index != string::npos)
	{
		string token;

		token = str.substr(index, getTokenLength(str, index));
		if(token.compare("-imfchan") == 0)
		{
			index = nextToken(str, index);
			if(index == string::npos)
				return false;

			token = str.substr(index, getTokenLength(str, index));
			if(token.compare("r") == 0)
				channel = Material::CHANNEL_RED;
			else if(token.compare("g") == 0)
				channel = Material::CHANNEL_GREEN;
			else if(token.compare("b") == 0)
				channel = Material::CHANNEL_BLUE;
			else if(token.compare("m") == 0)
				channel = Material::CHANNEL_MATTE;
			else if(token.compare("l") == 0)
				channel = Material::CHANNEL_LUMINANCE;
			else if(token.compare("z") == 0)
				channel = Material::CHANNEL_Z_DEPTH;
			else
				return false;
		}
		else if(token.compare("-bm") == 0)
		{
			if(channel != CHANNEL_TARGET_BUMP)
				return false;

			index = nextToken(str, index);
			if(index == string::npos)
				return false;

			bump_multiplier = atof(str.c_str() + index);
		}

		index = nextToken(str, index);
	}

	switch(target)
	{
	case CHANNEL_TARGET_SPECULAR_EXPONENT:
		m_materials[current_material]->setSpecularExponentMap(filename, channel);
		break;
	case CHANNEL_TARGET_TRANSPARENCY:
		m_materials[current_material]->setTransparencyMap(filename, channel);
		break;
	case CHANNEL_TARGET_DECAL:
		m_materials[current_material]->setDecalMap(filename, channel);
		break;
	case CHANNEL_TARGET_DISPLACEMENT:
		m_materials[current_material]->setDisplacementMap(filename, channel);
		break;
	case CHANNEL_TARGET_BUMP:
		m_materials[current_material]->setBumpMap(filename, channel, bump_multiplier);
		break;
	}

	return true;
}

void MtlLibrary :: copy (const MtlLibrary& original)
{
	assert(m_materials.size() == 0);

	unsigned int material_count = original.m_materials.size();

	m_name = original.m_name;
	m_materials.resize(material_count);
	for(unsigned int i = 0; i < material_count; i++)
	{
		assert(i < m_materials.size());
		assert(i < original.m_materials.size());
		assert(original.m_materials[i] != NULL);
		m_materials[i] = new Material(*(original.m_materials[i]));
	}

	assert(invariant());
}

bool MtlLibrary :: invariant () const
{
	if(m_name.compare("") == 0) return false;

	for(unsigned int i = 0; i < m_materials.size(); i++)
		if(m_materials[i] == NULL)
			return false;

	return true;
}
