//
//  Material.cpp
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
#include <fstream>
#include <iostream>

#include "GetGlut.h"
#include "ObjStringParsing.h"
#include "Texture.h"
#include "TextureManager.h"
#include "Material.h"

using namespace std;
namespace
{
	const unsigned int ILLUMINATION_TYPE_COUNT = 11;
	const unsigned int DEFAULT_ILLUMINATION_TYPE = 1;

	//
	//  Is this gaurenteed to be initialized before it is
	//    needed?  That would include either of:
	//    -> at compile time
	//    -> at run time, but before any function in this
	//       file is executed.
	//
	const char* DEFAULT_NAME = "Unnamed";

	const double DEFAULT_AMBIENT		= 0.2;
	const double DEFAULT_DIFFUSE		= 0.8;
	const double DEFAULT_SPECULAR		= 1.0;
	const double DEFAULT_SPECULAR_EXPONENT	= 20.0;
	const double DEFAULT_TRANSPARENCY	= 1.0;
	const double DEFAULT_OPTICAL_DENSITY	= 1.0;
	const double DEFAULT_TRANSMISSION_FILTER= 1.0;
	const double DEFAULT_BUMP_MULTIPLIER	= 1.0;

	const GLfloat BLACK[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	const GLfloat DEFAULT_LIGHT_AMBIENT[4] = { 0.5, 0.5, 0.5, 1.0 };
	const GLfloat DEFAULT_LIGHT_POSITION[4] = { 50.0, 100.0, 150.0, 1.0 };



	const char TEXTURE_TYPE_UNSPECIFIED	= '\0';
	const char TEXTURE_TYPE_AMBIENT		= 'a';
	const char TEXTURE_TYPE_DIFFUSE		= 'd';
	const char TEXTURE_TYPE_SPECULAR	= 's';
	const char TEXTURE_TYPE_NONE		= 'x';

	//
	//  isValidTextureType
	//
	//  Purpose: To determine if the specified texture type
	//	     is a vlid texture type.
	//  Parameter(s):
	//	<1> type: The texture type
	//  Precondition(s):  N/A
	//  Returns: Whether texture type type is valid.
	//  Side Effect: N/A
	//

	bool isValidTextureType (char type)
	{
		switch(type)
		{
		case TEXTURE_TYPE_UNSPECIFIED:
		case TEXTURE_TYPE_AMBIENT:
		case TEXTURE_TYPE_DIFFUSE:
		case TEXTURE_TYPE_SPECULAR:
		case TEXTURE_TYPE_NONE:
			return true;
		default:
			return false;
		}
	}



	bool g_is_material_active = false;

}	// end of anonymous namespace



const char Material :: CHANNEL_UNSPECIFIED = '\0';
const char Material :: CHANNEL_RED         = 'r';
const char Material :: CHANNEL_GREEN       = 'g';
const char Material :: CHANNEL_BLUE        = 'b';
const char Material :: CHANNEL_ALPHA       = 'a';
const char Material :: CHANNEL_MATTE       = 'm';
const char Material :: CHANNEL_LUMINANCE   = 'l';
const char Material :: CHANNEL_Z_DEPTH     = 'z';

const unsigned int Material :: ILLUMINATION_CONSTANT               = 0;
const unsigned int Material :: ILLUMINATION_PHONG_NO_SPECULAR      = 1;
const unsigned int Material :: ILLUMINATION_PHONG                  = 2;
const unsigned int Material :: ILLUMINATION_RAY_REFLECTIVE         = 3;
const unsigned int Material :: ILLUMINATION_RAY_GLASS              = 4;
const unsigned int Material :: ILLUMINATION_RAY_FRESNEL            = 5;
const unsigned int Material :: ILLUMINATION_RAY_REFRACTION         = 6;
const unsigned int Material :: ILLUMINATION_RAY_FRESNEL_REFRACTION = 7;
const unsigned int Material :: ILLUMINATION_REFLECTIVE             = 8;
const unsigned int Material :: ILLUMINATION_GLASS                  = 9;
const unsigned int Material :: ILLUMINATION_RAY_INVISIBLE_SHADOWS  = 10;

bool Material :: isValidChannel (char channel)
{
	switch(channel)
	{
	case CHANNEL_UNSPECIFIED:
	case CHANNEL_RED:
	case CHANNEL_GREEN:
	case CHANNEL_BLUE:
	case CHANNEL_ALPHA:
	case CHANNEL_MATTE:
	case CHANNEL_LUMINANCE:
	case CHANNEL_Z_DEPTH:
		return true;
	default:
		return false;
	}
}

bool Material :: isValidIlluminationMode (unsigned int mode)
{
	switch(mode)
	{
	case ILLUMINATION_CONSTANT:
	case ILLUMINATION_PHONG_NO_SPECULAR:
	case ILLUMINATION_PHONG:
	case ILLUMINATION_RAY_REFLECTIVE:
	case ILLUMINATION_RAY_GLASS:
	case ILLUMINATION_RAY_FRESNEL:
	case ILLUMINATION_RAY_REFRACTION:
	case ILLUMINATION_RAY_FRESNEL_REFRACTION:
	case ILLUMINATION_REFLECTIVE:
	case ILLUMINATION_GLASS:
	case ILLUMINATION_RAY_INVISIBLE_SHADOWS:
		return true;
	default:
		return false;
	}
}

bool Material :: isMaterialActive ()
{
	return g_is_material_active;
}

void Material :: deactivate ()
{
	if(g_is_material_active)
		glPopAttrib();

	g_is_material_active = false;

	assert(!isMaterialActive());
}



Material :: Material () : m_ambient_colour(), m_diffuse_colour(), m_specular_colour(), m_transmission_filter()
{
	makeDefault();

	assert(invariant());
}

Material :: Material (const string& name) : m_ambient_colour(), m_diffuse_colour(), m_specular_colour(), m_transmission_filter()
{
	makeDefault();
	m_name = toLowercase(name);

	assert(invariant());
}

Material :: Material (const Material& original) : m_ambient_colour(original.m_ambient_colour), m_diffuse_colour(original.m_diffuse_colour), m_specular_colour(original.m_specular_colour), m_transmission_filter(original.m_transmission_filter)
{
	mp_ambient_map = NULL;
	mp_diffuse_map = NULL;
	mp_specular_map = NULL;
	mp_specular_exponent_map = NULL;
	mp_transparency_map = NULL;
	mp_decal_map = NULL;
	mp_displacement_map = NULL;
	mp_bump_map = NULL;

	copy(original);

	assert(invariant());
}

Material& Material :: operator= (const Material& original)
{
	if(&original != this)
	{
		destroy();

		copy(original);
		m_ambient_colour  = original.m_ambient_colour;
		m_diffuse_colour  = original.m_diffuse_colour;
		m_specular_colour = original.m_specular_colour;
		m_transmission_filter = original.m_transmission_filter;
	}

	assert(invariant());
	return *this;
}

Material :: ~Material ()
{
	destroy();
}



const string& Material :: getName () const
{
	return m_name;
}

unsigned int Material :: getIlluminationMode () const
{
	return m_illumination_mode;
}

bool Material :: isAmbientDefault () const
{
	if(m_ambient_colour.m_red   != DEFAULT_AMBIENT) return false;
	if(m_ambient_colour.m_green != DEFAULT_AMBIENT) return false;
	if(m_ambient_colour.m_blue  != DEFAULT_AMBIENT) return false;
	return true;
}

double Material :: getAmbientRed () const
{
	return m_ambient_colour.m_red;
}
double Material :: getAmbientGreen () const
{
	return m_ambient_colour.m_green;
}
double Material :: getAmbientBlue () const
{
	return m_ambient_colour.m_blue;
}

bool Material :: isAmbientMap () const
{
	if(m_ambient_filename.compare("") != 0)
		return true;
	else
		return false;
}

const string& Material :: getAmbientMapFilename () const
{
	assert(isAmbientMap());

	return m_ambient_filename;
}

bool Material :: isAmbientMapLoaded () const
{
	assert(isAmbientMap());

	if(mp_ambient_map != NULL)
		return true;
	else
		return false;
}

const Texture* Material :: getAmbientMap ()
{
	assert(isAmbientMap());

	if(!isAmbientMapLoaded())
		mp_ambient_map = &(TextureManager::get(m_ambient_filename));

	return mp_ambient_map;
}

const Texture* Material :: getAmbientMap () const
{
	assert(isAmbientMap());

	return mp_ambient_map;
}

bool Material :: isDiffuseDefault () const
{
	if(m_diffuse_colour.m_red   != DEFAULT_DIFFUSE) return false;
	if(m_diffuse_colour.m_green != DEFAULT_DIFFUSE) return false;
	if(m_diffuse_colour.m_blue  != DEFAULT_DIFFUSE) return false;
	return true;
}

double Material :: getDiffuseRed () const
{
	return m_diffuse_colour.m_red;
}
double Material :: getDiffuseGreen () const
{
	return m_diffuse_colour.m_green;
}
double Material :: getDiffuseBlue () const
{
	return m_diffuse_colour.m_blue;
}

bool Material :: isDiffuseMap () const
{
	if(m_diffuse_filename.compare("") != 0)
		return true;
	else
		return false;
}

const std::string& Material :: getDiffuseMapFilename () const
{
	assert(isDiffuseMap());

	return m_diffuse_filename;
}

bool Material :: isDiffuseMapLoaded () const
{
	assert(isDiffuseMap());

	if(mp_diffuse_map != NULL)
		return true;
	else
		return false;
}

const Texture* Material :: getDiffuseMap ()
{
	assert(isDiffuseMap());

	if(!isDiffuseMapLoaded())
		mp_diffuse_map = &(TextureManager::get(m_diffuse_filename));

	return mp_diffuse_map;
}

const Texture* Material :: getDiffuseMap () const
{
	assert(isDiffuseMap());

	return mp_diffuse_map;
}

bool Material :: isSpecularDefault () const
{
	if(m_specular_colour.m_red   != DEFAULT_SPECULAR) return false;
	if(m_specular_colour.m_green != DEFAULT_SPECULAR) return false;
	if(m_specular_colour.m_blue  != DEFAULT_SPECULAR) return false;
	return true;
}

double Material :: getSpecularRed () const
{
	return m_specular_colour.m_red;
}
double Material :: getSpecularGreen () const
{
	return m_specular_colour.m_green;
}
double Material :: getSpecularBlue () const
{
	return m_specular_colour.m_blue;
}

bool Material :: isSpecularMap () const
{
	if(m_specular_filename.compare("") != 0)
		return true;
	else
		return false;
}

const std::string& Material :: getSpecularMapFilename () const
{
	assert(isSpecularMap());

	return m_specular_filename;
}

bool Material :: isSpecularMapLoaded () const
{
	assert(isSpecularMap());

	if(mp_specular_map != NULL)
		return true;
	else
		return false;
}

const Texture* Material :: getSpecularMap ()
{
	assert(isSpecularMap());

	if(!isSpecularMapLoaded())
		mp_specular_map = &(TextureManager::get(m_specular_filename));

	return mp_specular_map;
}

const Texture* Material :: getSpecularMap () const
{
	assert(isSpecularMap());

	return mp_specular_map;
}

bool Material :: isSpecularExponentDefault () const
{
	if(m_specular_exponent == DEFAULT_SPECULAR_EXPONENT)
		return true;
	else
		return false;
}

double Material :: getSpecularExponent () const
{
	return m_specular_exponent;
}

bool Material :: isSpecularExponentMap () const
{
	if(m_specular_exponent_filename.compare("") != 0)
		return true;
	else
		return false;
}

const std::string& Material :: getSpecularExponentMapFilename () const
{
	assert(isSpecularExponentMap());

	return m_specular_exponent_filename;
}

bool Material :: isSpecularExponentMapLoaded () const
{
	assert(isSpecularExponentMap());

	if(mp_specular_exponent_map != NULL)
		return true;
	else
		return false;
}

const Texture* Material :: getSpecularExponentMap ()
{
	assert(isSpecularExponentMap());

	if(!isSpecularExponentMapLoaded())
		mp_specular_exponent_map = &(TextureManager::get(m_specular_exponent_filename));

	return mp_specular_exponent_map;
}

const Texture* Material :: getSpecularExponentMap () const
{
	assert(isSpecularExponentMap());

	return mp_specular_exponent_map;
}

bool Material :: isSpecularExponentMapChannelSet () const
{
	assert(isSpecularExponentMap());

	if(m_specular_exponent_channel != CHANNEL_UNSPECIFIED)
		return true;
	else
		return false;
}

char Material :: getSpecularExponentMapChannel () const
{
	assert(isSpecularExponentMap());

	return m_specular_exponent_channel;
}

bool Material :: isTransparencyDefault () const
{
	if(m_transparency == DEFAULT_TRANSPARENCY)
		return true;
	else
		return false;
}

double Material :: getTransparency () const
{
	return m_transparency;
}

bool Material :: isTransparencyMap () const
{
	if(m_transparency_filename.compare("") != 0)
		return true;
	else
		return false;
}

const std::string& Material :: getTransparencyMapFilename () const
{
	assert(isTransparencyMap());

	return m_transparency_filename;
}

bool Material :: isTransparencyMapLoaded () const
{
	assert(isTransparencyMap());

	if(mp_transparency_map != NULL)
		return true;
	else
		return false;
}

const Texture* Material :: getTransparencyMap ()
{
	assert(isTransparencyMap());

	if(!isTransparencyMapLoaded())
		mp_transparency_map = &(TextureManager::get(m_transparency_filename));

	return mp_transparency_map;
}

const Texture* Material :: getTransparencyMap () const
{
	assert(isTransparencyMap());

	return mp_transparency_map;
}

bool Material :: isTransparencyMapChannelSet () const
{
	assert(isTransparencyMap());

	if(m_transparency_channel != CHANNEL_UNSPECIFIED)
		return true;
	else
		return false;
}

char Material :: getTransparencyMapChannel () const
{
	assert(isTransparencyMap());

	return m_transparency_channel;
}

bool Material :: isDecalMap () const
{
	if(m_decal_filename.compare("") != 0)
		return true;
	else
		return false;
}

bool Material :: isOpticalDensityDefault () const
{
	if(m_optical_density == DEFAULT_OPTICAL_DENSITY)
		return true;
	else
		return false;
}

double Material :: getOpticalDensity () const
{
	return m_optical_density;
}

bool Material :: isTransmissionFilterDefault () const
{
	if(m_transmission_filter.m_red   != DEFAULT_TRANSMISSION_FILTER) return false;
	if(m_transmission_filter.m_green != DEFAULT_TRANSMISSION_FILTER) return false;
	if(m_transmission_filter.m_blue  != DEFAULT_TRANSMISSION_FILTER) return false;
	return true;
}

double Material :: getTransmissionFilterRed () const
{
	return m_transmission_filter.m_red;
}

double Material :: getTransmissionFilterGreen () const
{
	return m_transmission_filter.m_green;
}

double Material :: getTransmissionFilterBlue () const
{
	return m_transmission_filter.m_blue;
}

const std::string& Material :: getDecalMapFilename () const
{
	assert(isDecalMap());

	return m_decal_filename;
}

bool Material :: isDecalMapLoaded () const
{
	assert(isDecalMap());

	if(mp_decal_map != NULL)
		return true;
	else
		return false;
}

const Texture* Material :: getDecalMap ()
{
	assert(isDecalMap());

	if(!isDecalMapLoaded())
		mp_decal_map = &(TextureManager::get(m_decal_filename));

	return mp_decal_map;
}

const Texture* Material :: getDecalMap () const
{
	assert(isDecalMap());

	return mp_decal_map;
}

bool Material :: isDecalMapChannelSet () const
{
	assert(isDecalMap());

	if(m_decal_channel != CHANNEL_UNSPECIFIED)
		return true;
	else
		return false;
}

char Material :: getDecalMapChannel () const
{
	assert(isDecalMap());

	return m_decal_channel;
}

bool Material :: isDisplacementMap () const
{
	if(m_displacement_filename.compare("") != 0)
		return true;
	else
		return false;
}

const std::string& Material :: getDisplacementMapFilename () const
{
	assert(isDisplacementMap());

	return m_displacement_filename;
}

bool Material :: isDisplacementMapLoaded () const
{
	assert(isDisplacementMap());

	if(mp_displacement_map != NULL)
		return true;
	else
		return false;
}

const Texture* Material :: getDisplacementMap ()
{
	assert(isDisplacementMap());

	if(!isDisplacementMapLoaded())
		mp_displacement_map = &(TextureManager::get(m_displacement_filename));

	return mp_displacement_map;
}

const Texture* Material :: getDisplacementMap () const
{
	assert(isDisplacementMap());

	return mp_displacement_map;
}

bool Material :: isDisplacementMapChannelSet () const
{
	assert(isDisplacementMap());

	if(m_displacement_channel != CHANNEL_UNSPECIFIED)
		return true;
	else
		return false;
}

char Material :: getDisplacementMapChannel () const
{
	assert(isDisplacementMap());

	return m_displacement_channel;
}

bool Material :: isBumpMap () const
{
	if(m_bump_filename.compare("") != 0)
		return true;
	else
		return false;
}

const std::string& Material :: getBumpMapFilename () const
{
	assert(isBumpMap());

	return m_bump_filename;
}

bool Material :: isBumpMapLoaded () const
{
	assert(isBumpMap());

	if(mp_bump_map != NULL)
		return true;
	else
		return false;
}

const Texture* Material :: getBumpMap ()
{
	assert(isBumpMap());

	if(!isBumpMapLoaded())
		mp_bump_map = &(TextureManager::get(m_bump_filename));

	return mp_bump_map;
}

const Texture* Material :: getBumpMap () const
{
	assert(isBumpMap());

	return mp_bump_map;
}

bool Material :: isBumpMapChannelSet () const
{
	assert(isBumpMap());

	if(m_bump_channel != CHANNEL_UNSPECIFIED)
		return true;
	else
		return false;
}

char Material :: getBumpMapChannel () const
{
	assert(isBumpMap());

	return m_bump_channel;
}

bool Material :: isBumpMapMultiplierDefault () const
{
	assert(isBumpMap());

	if(m_bump_multiplier == DEFAULT_BUMP_MULTIPLIER)
		return true;
	else
		return false;
}

double Material :: getBumpMapMultiplier () const
{
	assert(isBumpMap());

	return m_bump_multiplier;
}

void Material :: print () const
{
	cout << "    " << m_name << ":" << endl;

	cout << "        Illumination Mode: " << m_illumination_mode << endl;

	// ambient
	cout << "        Ambient Colour: ";
	cout << "\t" << m_ambient_colour.m_red;
	cout << "\t" << m_ambient_colour.m_green;
	cout << "\t" << m_ambient_colour.m_blue;
	if(isAmbientDefault())
		cout << " (default)";
	cout << endl;
	if(isAmbientMap())
		cout << "        Ambinet Map: " << m_ambient_filename << endl;

	// diffuse
	cout << "        Diffuse Colour: ";
	cout << "\t" << m_diffuse_colour.m_red;
	cout << "\t" << m_diffuse_colour.m_green;
	cout << "\t" << m_diffuse_colour.m_blue;
	if(isDiffuseDefault())
		cout << " (default)";
	cout << endl;
	if(isDiffuseMap())
		cout << "        Ambinet Map: " << m_diffuse_filename << endl;

	// specular
	cout << "        Specular Colour: ";
	cout << "\t" << m_specular_colour.m_red;
	cout << "\t" << m_specular_colour.m_green;
	cout << "\t" << m_specular_colour.m_blue;
	if(isSpecularDefault())
		cout << " (default)";
	cout << endl;
	if(isSpecularMap())
		cout << "        Ambinet Map: " << m_specular_filename << endl;

	// specular exponent
	cout << "        Specular Exponent: " << m_specular_exponent;
	if(isSpecularExponentDefault())
		cout << " (default)";
	cout << endl;
	if(isSpecularExponentMap())
		cout << "        Specular Exponent Map: " << m_specular_exponent_filename << " (" << m_specular_exponent_channel << ")" << endl;

	// transparency
	if(!isTransparencyDefault())
		cout << "        Transparency: " << m_transparency << endl;
	if(isTransparencyMap())
		cout << "        Transparency Map: " << m_transparency_filename << " (" << m_transparency_channel << ")" << endl;

	// transmission filter
	cout << "        Transmission Filter: ";
	cout << "\t" << m_specular_colour.m_red;
	cout << "\t" << m_specular_colour.m_green;
	cout << "\t" << m_specular_colour.m_blue;
	if(isTransmissionFilterDefault())
		cout << " (default)";
	cout << endl;

	// decal map
	if(isDecalMap())
		cout << "        Decal Map: " << m_decal_filename << " (" << m_decal_channel << ")" << endl;

	// displacement map
	if(isDisplacementMap())
		cout << "        Displacement Map: " << m_displacement_filename << " (" << m_displacement_channel << ")" << endl;

	// bump map
	if(isBumpMap())
		cout << "        Bump Map: " << m_bump_filename << " (" << m_bump_channel << ")" << " * " << m_bump_multiplier << endl;
}



void Material :: activate () const
{
	assert(!isMaterialActive());

	GLfloat a_ambient[4];
	GLfloat a_diffuse[4];
	GLfloat a_specular[4];
	unsigned int effective_illumination_mode = m_illumination_mode;

	if(m_texture_type_display == TEXTURE_TYPE_UNSPECIFIED)
	{
		//
		//  I am using const_cast<> to add remove
		//    const-ness from the "this" pointer so I
		//    can calculate which texture image to use.
		//    This may involve loading the image.  The
		//    three alternatives are:
		//	<1> Require a special prepare() function
		//	    to be called before this Material
		//	    is used
		//	<2> Display this Material without
		//	    textures until prepare() is called
		//	<3> Recalculate the texture information
		//	    each time this Material is activated
		//	<4> Make display non-const
		//
		//  The objections to these methods are:
		//	<1> is very inconvenient
		//	<2> makes model inexplicably display
		//	    without textures
		//	<3> is slow
		//	<4> would mean that any function that
		//	    used Materials for display (such as
		//	    ObjModel) would have to have
		//	    non-const display functions (bad
		//	    practice for output functions)
		//

		Material* p_non_const_this = const_cast<Material*>(this);

		p_non_const_this->loadDisplayTextures();
	}

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |  GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	a_ambient[0] = m_ambient_colour.m_red;
	a_ambient[1] = m_ambient_colour.m_green;
	a_ambient[2] = m_ambient_colour.m_blue;
	a_ambient[3] = m_transparency;

	a_diffuse[0] = m_diffuse_colour.m_red;
	a_diffuse[1] = m_diffuse_colour.m_green;
	a_diffuse[2] = m_diffuse_colour.m_blue;
	a_diffuse[3] = m_transparency;

	a_specular[0] = m_specular_colour.m_red;
	a_specular[1] = m_specular_colour.m_green;
	a_specular[2] = m_specular_colour.m_blue;
	a_specular[3] = m_transparency;

	if(glIsEnabled(GL_LIGHTING) == GL_FALSE)
		effective_illumination_mode = ILLUMINATION_CONSTANT;

	//
	//  Shading Modes
	//    http://paulbourke.net/dataformats/mtl/
	//
	//  0	Color on and Ambient off
	//  1	Color on and Ambient on
	//  2	Highlight on
	//  3	Reflection on and Ray trace on
	//  4	Transparency: Glass on
	// 	Reflection: Ray trace on
	//  5	Reflection: Fresnel on and Ray trace on
	//  6	Transparency: Refraction on
	//	Reflection: Fresnel off and Ray trace on
	//  7	Transparency: Refraction on
	//	Reflection: Fresnel on and Ray trace on
	//  8	Reflection on and Ray trace off
	//  9	Transparency: Glass on
	// 	Reflection: Ray trace off
	//  10	Casts shadows onto invisible surfaces
	//

	switch(effective_illumination_mode)
	{
	case ILLUMINATION_CONSTANT:
		//  We don't wate to use the lighting-style
		//    material information, so it doesn't matter
		//    what is in the arrays.  We just want the
		//    sum of the ambient and half the diffuse
		//    colours.
		glDisable(GL_LIGHTING);
		a_ambient[0] += a_diffuse[0] / 2.0;
		a_ambient[1] += a_diffuse[1] / 2.0;
		a_ambient[2] += a_diffuse[2] / 2.0;
		glColor4fv(a_ambient);
		break;
	case ILLUMINATION_PHONG_NO_SPECULAR:
		// remove specular
		a_specular[0] = 0.0;
		a_specular[1] = 0.0;
		a_specular[2] = 0.0;
		break;
	case ILLUMINATION_PHONG:
	case ILLUMINATION_RAY_REFRACTION:
		// everything is already right
		break;
	case ILLUMINATION_RAY_REFLECTIVE:
	case ILLUMINATION_REFLECTIVE:
		//
		//  True reflection requires ray tracing, so it
		//    is not supported. It is also possible for
		//    a model to reflect an enviroment map, but
		//    that is slow and requires a specially-made
		//    image of the enviroment.  Or 6 images, if
		//    you want to do it properly.
		//
		break;
	case ILLUMINATION_RAY_FRESNEL:
	case ILLUMINATION_RAY_FRESNEL_REFRACTION:
		//
		//  Fresnel effects govern how much light is
		//    refracted and how much is reflected based
		//    on the angle of the incident light ray.
		//    This requires ray tracing, so it is not
		//    supported.
		//
		break;
	case ILLUMINATION_RAY_GLASS:
	case ILLUMINATION_GLASS:
		assert(isSeperateSpecular());
		a_specular[0] = 0.0;
		a_specular[1] = 0.0;
		a_specular[2] = 0.0;
		// highlights on glass must be drawn seperately
		break;
	case ILLUMINATION_RAY_INVISIBLE_SHADOWS:
		// just make it invisible, we don't do shadows
		a_ambient[3]  = 0.0;
		a_diffuse[3]  = 0.0;
		a_specular[3] = 0.0;
		break;
	}

	if(effective_illumination_mode != ILLUMINATION_CONSTANT)
	{
		glMaterialfv(GL_FRONT, GL_AMBIENT,   a_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE,   a_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR,  a_specular);
		glMaterialfv(GL_FRONT, GL_EMISSION,  BLACK);
		glMaterialf (GL_FRONT, GL_SHININESS, m_specular_exponent);
	}

	switch(m_texture_type_display)
	{
	case TEXTURE_TYPE_AMBIENT:
		assert(mp_ambient_map != NULL);
		mp_ambient_map->activate();
		break;
	case TEXTURE_TYPE_DIFFUSE:
		assert(mp_diffuse_map != NULL);
		mp_diffuse_map->activate();
		break;
	case TEXTURE_TYPE_SPECULAR:
		assert(mp_specular_map != NULL);
		mp_specular_map->activate();
		break;
	case TEXTURE_TYPE_NONE:
	case TEXTURE_TYPE_UNSPECIFIED:
	default:
		glDisable(GL_TEXTURE_2D);
		break;
	};

	g_is_material_active = true;
	assert(isMaterialActive());
}

bool Material :: isSeperateSpecular () const
{
	switch(m_illumination_mode)
	{
	case ILLUMINATION_RAY_GLASS:
	case ILLUMINATION_GLASS:
		return true;
	default:
		return false;
	}
}

void Material :: activateSeperateSpecular () const
{
	assert(isSeperateSpecular());
	assert(!isMaterialActive());

	GLfloat a_specular[4];

	glPushAttrib(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT | GL_TEXTURE_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_EQUAL);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0);

	// no specular highlights
	if(glIsEnabled(GL_LIGHTING) == GL_FALSE)
		glBlendFunc(GL_ZERO, GL_ONE);

	a_specular[0] = m_specular_colour.m_red;
	a_specular[1] = m_specular_colour.m_green;
	a_specular[2] = m_specular_colour.m_blue;
	a_specular[3] = 1.0;

	glMaterialfv(GL_FRONT, GL_AMBIENT,   BLACK);
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   BLACK);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  a_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION,  BLACK);
	glMaterialf (GL_FRONT, GL_SHININESS, m_specular_exponent);

	g_is_material_active = true;
	assert(isMaterialActive());
}

void Material :: save (ostream& r_out) const
{
	bool in_block;

	r_out << "newmtl " << m_name << endl;
	r_out << endl;

	r_out << "illum " << m_illumination_mode << endl;
	r_out << endl;

	r_out << "# basic colour information" << endl;
	if(!isAmbientDefault())
		r_out << "Ka\t" << m_ambient_colour.m_red << "\t" << m_ambient_colour.m_green << "\t" << m_ambient_colour.m_blue << endl;
	if(!isDiffuseDefault())
		r_out << "Kd\t" << m_diffuse_colour.m_red << "\t" << m_diffuse_colour.m_green << "\t" << m_diffuse_colour.m_blue << endl;
	if(!isSpecularDefault() || !isSpecularExponentDefault())
	{
		r_out << "Ks\t" << m_specular_colour.m_red << "\t" << m_specular_colour.m_green << "\t" << m_specular_colour.m_blue << endl;
		r_out << "Ns\t" << m_specular_exponent << endl;
	}
	if(!isTransmissionFilterDefault())
		r_out << "Tf " << m_transmission_filter.m_red << "\t" << m_transmission_filter.m_green << "\t" << m_transmission_filter.m_blue << endl;
	r_out << endl;

	if(!isTransparencyDefault())
	{
		r_out << "# both of these are transparency, the format is inconsistant" << endl;
		r_out << "Tr " << m_transparency << endl;
		r_out << "d  " << m_transparency << endl;
		r_out << endl;
	}

	in_block = false;
	if(isAmbientMap())
	{
		r_out << "map_Ka " << m_ambient_filename << endl;
		in_block = true;
	}
	if(isDiffuseMap())
	{
		r_out << "map_Kd " << m_diffuse_filename << endl;
		in_block = true;
	}
	if(isSpecularMap())
	{
		r_out << "map_Ks " << m_specular_filename << endl;
		in_block = true;
	}
	if(isSpecularExponentMap())
	{
		r_out << "map_Ns " << m_specular_exponent_filename;
		if(isSpecularExponentMapChannelSet())
			r_out << " -imfchan " << m_specular_exponent_channel;
		r_out << endl;
		in_block = true;
	}
	if(in_block)
		r_out << endl;

	if(isTransparencyMap())
	{
		r_out << "map_Tr " << m_transparency_filename;
		if(isTransparencyMapChannelSet())
			r_out << " -imfchan " << m_transparency_channel;
		r_out << endl;
		r_out << "map_d  " << m_transparency_filename;
		if(isTransparencyMapChannelSet())
			r_out << " -imfchan " << m_transparency_channel;
		r_out << endl;
		r_out << endl;
	}

	in_block = false;
	if(isDecalMap())
	{
		r_out << "decal " << m_decal_filename;
		if(isDecalMapChannelSet())
			r_out << " -imfchan " << m_decal_channel;
		r_out << endl;
		in_block = true;
	}
	if(isDisplacementMap())
	{
		r_out << "disp " << m_displacement_filename;
		if(isDisplacementMapChannelSet())
			r_out << " -imfchan " << m_displacement_channel;
		r_out << endl;
		in_block = true;
	}
	if(isBumpMap())
	{
		r_out << "bump " << m_bump_filename;
		if(isBumpMapChannelSet())
			r_out << " -imfchan " << m_bump_channel;
		if(!isBumpMapMultiplierDefault())
			r_out << " -bm " << m_bump_multiplier;
		r_out << endl;
		in_block = true;
	}
	else if(!isBumpMapMultiplierDefault())
	{
		r_out << "# nonstandard bump multiplier without bump map (probably)" << endl;
		r_out << "Km " << m_bump_multiplier << endl;
		in_block = true;
	}
	if(in_block)
		r_out << endl;
}



void Material :: setName (const std::string& name)
{
	assert(name.compare("") != 0);

	m_name = toLowercase(name);

	assert(invariant());
}

void Material :: setIlluminationMode (unsigned int mode)
{
	assert(isValidIlluminationMode(mode));

	m_illumination_mode = mode;

	assert(invariant());
}

void Material :: setAmbientRed (double red)
{
	m_ambient_colour.m_red = red;

	assert(invariant());
}
void Material :: setAmbientGreen (double green)
{
	m_ambient_colour.m_green = green;

	assert(invariant());
}
void Material :: setAmbientBlue (double blue)
{
	m_ambient_colour.m_blue = blue;

	assert(invariant());
}

void Material :: setAmbientColour (double red, double green, double blue)
{
	m_ambient_colour.m_red = red;
	m_ambient_colour.m_green = green;
	m_ambient_colour.m_blue = blue;

	assert(invariant());
}

void Material :: setAmbientMap (const string& filename)
{
	assert(filename.compare("") != 0);

	if(mp_ambient_map != NULL)
		mp_ambient_map = NULL;

	assert(mp_ambient_map == NULL);

	m_ambient_filename = filename;
	// The map is actually loaded when it is needed

	assert(invariant());
}

void Material :: setAmbientMapNone ()
{
	m_ambient_filename = "";
	mp_ambient_map = NULL;

	assert(mp_ambient_map == NULL);
	assert(invariant());
}

void Material :: setDiffuseRed (double red)
{
	m_diffuse_colour.m_red = red;

	assert(invariant());
}
void Material :: setDiffuseGreen (double green)
{
	m_diffuse_colour.m_green = green;

	assert(invariant());
}
void Material :: setDiffuseBlue (double blue)
{
	m_diffuse_colour.m_blue = blue;

	assert(invariant());
}

void Material :: setDiffuseColour (double red, double green, double blue)
{
	m_diffuse_colour.m_red = red;
	m_diffuse_colour.m_green = green;
	m_diffuse_colour.m_blue = blue;

	assert(invariant());
}

void Material :: setDiffuseMap (const string& filename)
{
	assert(filename.compare("") != 0);

	if(mp_diffuse_map != NULL)
		mp_diffuse_map = NULL;

	assert(mp_diffuse_map == NULL);

	m_diffuse_filename = filename;
	// The map is actually loaded when it is needed

	assert(invariant());
}

void Material :: setDiffuseMapNone ()
{
	m_diffuse_filename = "";
	mp_diffuse_map = NULL;

	assert(mp_diffuse_map == NULL);
	assert(invariant());
}

void Material :: setSpecularRed (double red)
{
	m_specular_colour.m_red = red;

	assert(invariant());
}
void Material :: setSpecularGreen (double green)
{
	m_specular_colour.m_green = green;

	assert(invariant());
}
void Material :: setSpecularBlue (double blue)
{
	m_specular_colour.m_blue = blue;

	assert(invariant());
}

void Material :: setSpecularColour (double red, double green, double blue)
{
	m_specular_colour.m_red = red;
	m_specular_colour.m_green = green;
	m_specular_colour.m_blue = blue;

	assert(invariant());
}

void Material :: setSpecularMap (const string& filename)
{
	assert(filename.compare("") != 0);

	if(mp_specular_map != NULL)
		mp_specular_map = NULL;

	assert(mp_specular_map == NULL);

	m_specular_filename = filename;
	// The map is actually loaded when it is needed

	assert(invariant());
}

void Material :: setSpecularMapNone ()
{
	m_specular_filename = "";
	mp_specular_map = NULL;

	assert(mp_specular_map == NULL);
	assert(invariant());
}

void Material :: setSpecularExponent (double exponent)
{
	m_specular_exponent = exponent;

	assert(invariant());
}

void Material :: setSpecularExponentMap (const string& filename, char channel)
{
	assert(filename.compare("") != 0);
	assert(isValidChannel(channel));

	if(mp_specular_exponent_map != NULL)
		mp_specular_exponent_map = NULL;

	assert(mp_specular_exponent_map == NULL);

	m_specular_exponent_filename = filename;
	// The map is actually loaded when it is needed
	m_specular_exponent_channel = channel;

	assert(invariant());
}

void Material :: setSpecularExponentMapNone ()
{
	m_specular_exponent_filename = "";
	mp_specular_exponent_map = NULL;
	m_specular_exponent_channel = CHANNEL_UNSPECIFIED;

	assert(invariant());
}

void Material :: setTransparency (double transparency)
{
	m_transparency = transparency;

	assert(invariant());
}

void Material :: setTransparencyMap (const std::string& filename, char channel)
{
	assert(filename.compare("") != 0);
	assert(isValidChannel(channel));

	if(mp_transparency_map != NULL)
		mp_transparency_map = NULL;

	assert(mp_transparency_map == NULL);

	m_transparency_filename = filename;
	// The map is actually loaded when it is needed
	m_transparency_channel = channel;

	assert(invariant());
}

void Material :: setTransparencyMapNone ()
{
	m_transparency_filename = "";
	mp_transparency_map = NULL;
	m_transparency_channel = CHANNEL_UNSPECIFIED;

	assert(invariant());
}

void Material :: setOpticalDensity (double density)
{
	m_optical_density = density;

	assert(invariant());
}

void Material :: setTransmissionFilterRed (double red)
{
	m_transmission_filter.m_red = red;

	assert(invariant());
}

void Material :: setTransmissionFilterGreen (double green)
{
	m_transmission_filter.m_green = green;

	assert(invariant());
}

void Material :: setTransmissionFilterBlue (double blue)
{
	m_transmission_filter.m_blue = blue;

	assert(invariant());
}

void Material :: setTransmissionFilter (double red, double green, double blue)
{
	m_transmission_filter.m_red = red;
	m_transmission_filter.m_green = green;
	m_transmission_filter.m_blue = blue;

	assert(invariant());
}

void Material :: setDecalMap (const std::string& filename, char channel)
{
	assert(filename.compare("") != 0);
	assert(isValidChannel(channel));

	if(mp_decal_map != NULL)
		mp_decal_map = NULL;

	assert(mp_decal_map == NULL);

	m_decal_filename = filename;
	// The map is actually loaded when it is needed
	m_decal_channel = channel;

	assert(invariant());
}

void Material :: setDecalMapNone ()
{
	m_decal_filename = "";
	mp_decal_map = NULL;
	m_decal_channel = CHANNEL_UNSPECIFIED;

	assert(invariant());
}

void Material :: setDisplacementMap (const std::string& filename, char channel)
{
	assert(filename.compare("") != 0);
	assert(isValidChannel(channel));

	if(mp_displacement_map != NULL)
		mp_displacement_map = NULL;

	assert(mp_displacement_map == NULL);

	m_displacement_filename = filename;
	// The map is actually loaded when it is needed
	m_displacement_channel = channel;

	assert(invariant());
}

void Material :: setDisplacementMapNone ()
{
	m_displacement_filename = "";
	mp_displacement_map = NULL;
	m_displacement_channel = CHANNEL_UNSPECIFIED;

	assert(invariant());
}

void Material :: setBumpMap (const std::string& filename, char channel, double multiplier)
{
	assert(filename.compare("") != 0);
	assert(isValidChannel(channel));

	if(mp_bump_map != NULL)
		mp_bump_map = NULL;

	assert(mp_bump_map == NULL);

	m_bump_filename = filename;
	// The map is actually loaded when it is needed
	m_bump_channel = channel;
	m_bump_multiplier = multiplier;

	assert(invariant());
}

void Material :: setBumpMapNone ()
{
	m_bump_filename = "";
	mp_bump_map = NULL;
	m_bump_channel = CHANNEL_UNSPECIFIED;
	m_bump_multiplier = DEFAULT_BUMP_MULTIPLIER;

	assert(invariant());
}

void Material :: setBumpMapMultiplier (double multiplier)
{
	m_bump_multiplier = multiplier;

	assert(invariant());
}



void Material :: makeDefault ()
{
	m_name = DEFAULT_NAME;
	m_illumination_mode = Material::ILLUMINATION_PHONG;
	m_texture_type_display = TEXTURE_TYPE_UNSPECIFIED;

	m_ambient_colour.m_red   = DEFAULT_AMBIENT;
	m_ambient_colour.m_green = DEFAULT_AMBIENT;
	m_ambient_colour.m_blue  = DEFAULT_AMBIENT;
	m_ambient_filename = "";
	mp_ambient_map = NULL;

	m_diffuse_colour.m_red   = DEFAULT_DIFFUSE;
	m_diffuse_colour.m_green = DEFAULT_DIFFUSE;
	m_diffuse_colour.m_blue  = DEFAULT_DIFFUSE;
	m_diffuse_filename = "";
	mp_diffuse_map = NULL;

	m_specular_colour.m_red   = DEFAULT_SPECULAR;
	m_specular_colour.m_green = DEFAULT_SPECULAR;
	m_specular_colour.m_blue  = DEFAULT_SPECULAR;
	m_specular_filename = "";
	mp_specular_map = NULL;

	m_specular_exponent = DEFAULT_SPECULAR_EXPONENT;
	m_specular_exponent_filename = "";
	mp_specular_exponent_map = NULL;
	m_specular_exponent_channel = CHANNEL_UNSPECIFIED;

	m_transparency = DEFAULT_TRANSPARENCY;
	m_transparency_filename = "";
	mp_transparency_map = NULL;
	m_transparency_channel = CHANNEL_UNSPECIFIED;

	m_transmission_filter.m_red   = DEFAULT_TRANSMISSION_FILTER;
	m_transmission_filter.m_green = DEFAULT_TRANSMISSION_FILTER;
	m_transmission_filter.m_blue  = DEFAULT_TRANSMISSION_FILTER;

	m_decal_filename = "";
	mp_decal_map = NULL;
	m_decal_channel = CHANNEL_UNSPECIFIED;

	m_displacement_filename = "";
	mp_displacement_map = NULL;
	m_displacement_channel = CHANNEL_UNSPECIFIED;

	m_bump_filename = "";
	mp_bump_map = NULL;
	m_bump_channel = CHANNEL_UNSPECIFIED;
	m_bump_multiplier = DEFAULT_BUMP_MULTIPLIER;

	assert(invariant());
}

void Material :: loadDisplayTextures ()
{
	//  If the texture type is already know, then the
	//    appropriate texture is already loaded.
	if(m_texture_type_display != TEXTURE_TYPE_UNSPECIFIED)
	{
		assert(invariant());
		return;
	}

	// attempt to use ambient texture
	if(mp_ambient_map == NULL && m_ambient_filename.compare("") != 0)
		mp_ambient_map = &(TextureManager::get(m_ambient_filename));
	if(mp_ambient_map != NULL && !TextureManager::isDummyTexture(*mp_ambient_map))
	{
		m_texture_type_display = TEXTURE_TYPE_AMBIENT;
		assert(invariant());
		return;
	}

	// attempt to use diffuse texture
	if(mp_diffuse_map == NULL && m_diffuse_filename.compare("") != 0)
		mp_diffuse_map = &(TextureManager::get(m_diffuse_filename));
	if(mp_diffuse_map != NULL && !TextureManager::isDummyTexture(*mp_diffuse_map))
	{
		m_texture_type_display = TEXTURE_TYPE_DIFFUSE;
		assert(invariant());
		return;
	}

	// attempt to use specular texture
	if(mp_specular_map == NULL && m_specular_filename.compare("") != 0)
		mp_specular_map = &(TextureManager::get(m_specular_filename));
	if(mp_specular_map != NULL && !TextureManager::isDummyTexture(*mp_specular_map))
	{
		m_texture_type_display = TEXTURE_TYPE_SPECULAR;
		assert(invariant());
		return;
	}

	// don't use a texture for display
	m_texture_type_display = TEXTURE_TYPE_NONE;
	assert(invariant());
}

void Material :: loadAllTextures ()
{
	if(mp_ambient_map == NULL && m_ambient_filename.compare("") == 0)
		mp_ambient_map = &(TextureManager::get(m_ambient_filename));

	if(mp_diffuse_map == NULL && m_diffuse_filename.compare("") == 0)
		mp_diffuse_map = &(TextureManager::get(m_diffuse_filename));

	if(mp_specular_map == NULL && m_specular_filename.compare("") == 0)
		mp_specular_map = &(TextureManager::get(m_specular_filename));

	if(mp_specular_exponent_map == NULL && m_specular_exponent_filename.compare("") == 0)
		mp_specular_exponent_map = &(TextureManager::get(m_specular_exponent_filename));

	if(mp_transparency_map == NULL && m_transparency_filename.compare("") == 0)
		mp_transparency_map = &(TextureManager::get(m_transparency_filename));

	if(mp_decal_map == NULL && m_decal_filename.compare("") == 0)
		mp_decal_map = &(TextureManager::get(m_decal_filename));

	if(mp_displacement_map == NULL && m_displacement_filename.compare("") == 0)
		mp_displacement_map = &(TextureManager::get(m_displacement_filename));

	if(mp_bump_map == NULL && m_bump_filename.compare("") == 0)
		mp_bump_map = &(TextureManager::get(m_bump_filename));
}



void Material :: destroy ()
{
	mp_ambient_map = NULL;
	mp_diffuse_map = NULL;
	mp_specular_map = NULL;
	mp_specular_exponent_map = NULL;
	mp_transparency_map = NULL;
	mp_decal_map = NULL;
	mp_displacement_map = NULL;
	mp_bump_map = NULL;

	assert(mp_ambient_map == NULL);
	assert(mp_diffuse_map == NULL);
	assert(mp_specular_map == NULL);
	assert(mp_specular_exponent_map == NULL);
	assert(mp_transparency_map == NULL);
	assert(mp_decal_map == NULL);
	assert(mp_displacement_map == NULL);
	assert(mp_bump_map == NULL);
}

void Material :: copy (const Material& original)
{
	assert(mp_ambient_map == NULL);
	assert(mp_diffuse_map == NULL);
	assert(mp_specular_map == NULL);
	assert(mp_specular_exponent_map == NULL);
	assert(mp_transparency_map == NULL);
	assert(mp_decal_map == NULL);
	assert(mp_displacement_map == NULL);
	assert(mp_bump_map == NULL);

	m_name = original.m_name;
	m_illumination_mode = original.m_illumination_mode;
	m_texture_type_display = original.m_texture_type_display;

	// m_ambient_colour is copied elsewhere
	m_ambient_filename = original.m_ambient_filename;
	mp_ambient_map = original.mp_ambient_map;

	// m_diffuse_colour is copied elsewhere
	m_diffuse_filename = original.m_diffuse_filename;
	mp_diffuse_map = original.mp_diffuse_map;

	// m_specular_colour is copied elsewhere
	m_specular_filename = original.m_specular_filename;
	mp_specular_map = original.mp_specular_map;

	m_specular_exponent = original.m_specular_exponent;
	m_specular_exponent_filename = original.m_specular_exponent_filename;
	mp_specular_exponent_map = original.mp_specular_exponent_map;
	m_specular_exponent_channel = original.m_specular_exponent_channel;

	m_transparency = original.m_transparency;
	m_transparency_filename = original.m_transparency_filename;
	mp_transparency_map = original.mp_transparency_map;
	m_transparency_channel = original.m_transparency_channel;

	// m_transmission_filter is copied elsewhere

	m_decal_filename = original.m_decal_filename;
	mp_decal_map = original.mp_decal_map;
	m_decal_channel = original.m_decal_channel;

	m_displacement_filename = original.m_displacement_filename;
	mp_displacement_map = original.mp_displacement_map;
	m_displacement_channel = original.m_displacement_channel;

	m_bump_filename = original.m_bump_filename;
	mp_bump_map = original.mp_bump_map;
	m_bump_channel = original.m_bump_channel;
	m_bump_multiplier = original.m_bump_multiplier;

	assert(invariant());
}

bool Material :: invariant () const
{
	if(m_name.compare("") == 0) return false;
	if(m_illumination_mode >= ILLUMINATION_TYPE_COUNT) return false;
	if(!isValidTextureType(m_texture_type_display)) return false;
	if(!isValidChannel(m_specular_exponent_channel)) return false;
	if(!isValidChannel(m_transparency_channel)) return false;
	if(!isValidChannel(m_decal_channel)) return false;
	if(!isValidChannel(m_displacement_channel)) return false;
	if(!isValidChannel(m_bump_channel)) return false;
	if(m_texture_type_display == TEXTURE_TYPE_AMBIENT  && mp_ambient_map  == NULL) return false;
	if(m_texture_type_display == TEXTURE_TYPE_DIFFUSE  && mp_diffuse_map  == NULL) return false;
	if(m_texture_type_display == TEXTURE_TYPE_SPECULAR && mp_specular_map == NULL) return false;
	return true;
}

/*	assert(m_name.compare("") != 0);
	assert(m_illumination_mode < ILLUMINATION_TYPE_COUNT);
	assert(isValidTextureType(m_texture_type_display));
	assert(isValidChannel(m_specular_exponent_channel));
	assert(isValidChannel(m_transparency_channel));
	assert(isValidChannel(m_decal_channel));
	assert(isValidChannel(m_displacement_channel));
	assert(isValidChannel(m_bump_channel));
	assert(m_texture_type_display != TEXTURE_TYPE_AMBIENT  || mp_ambient_map  != NULL);
	assert(m_texture_type_display != TEXTURE_TYPE_DIFFUSE  || mp_diffuse_map  != NULL);
	assert(m_texture_type_display != TEXTURE_TYPE_SPECULAR || mp_specular_map != NULL);
*/



