//
//  Material.h
//
//  A module to encapsulate a simplified form of a MTL Material.
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

#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

class Texture;



//
//  Material
//
//  Class Invariant:
//	<1> m_name.compare("") != 0
//	<2> m_illumination_type < ILLUMINATION_TYPE_COUNT
//	<3> isValidTextureType(m_texture_type_display)
//	<4> isValidChannel(m_specular_exponent_channel)
//	<5> isValidChannel(m_transparency_channel)
//	<6> isValidChannel(m_decal_channel)
//	<7> isValidChannel(m_displacement_channel)
//	<8> isValidChannel(m_bump_channel)
//	<9> m_texture_type_display != TEXTURE_TYPE_AMBIENT ||
//	    mp_ambient_map != NULL
//	<10>m_texture_type_display != TEXTURE_TYPE_DIFFUSE ||
//	    mp_diffuse_map != NULL
//	<11>m_texture_type_display != TEXTURE_TYPE_SPECULAR ||
//	    mp_specular_map != NULL
//

class Material
{
public:
//
//  CHANNEL_UNSPECIFIED
//
//  A constant used to indicate that the no channel is currently
//    specified to be used for monochrome data.
//

	static const char CHANNEL_UNSPECIFIED;

//
//  CHANNEL_RED
//  CHANNEL_GREEN
//  CHANNEL_BLUE
//  CHANNEL_ALPHA
//
//  A constant used to indicate that the red/green/blue/alpha
//    colour channel should be used for monochrome data.
//
//  Note: The official MTL specification does not support the
//	  alpha channel.
//

	static const char CHANNEL_RED;
	static const char CHANNEL_GREEN;
	static const char CHANNEL_BLUE;
	static const char CHANNEL_ALPHA;

//
//  CHANNEL_MATTE
//
//  A constant used to indicate that the matte channel should be
//    used for monochrome data.
//

	static const char CHANNEL_MATTE;

//
//  CHANNEL_LUMINANCE
//
//  A constant used to indicate that the luminance channel
//    should be used for monochrome data.
//

	static const char CHANNEL_LUMINANCE;

//
//  CHANNEL_Z_DEPTH
//
//  A constant used to indicate that the z-depth channel should
//    be used for monochrome data.
//

	static const char CHANNEL_Z_DEPTH;

//
//  ILLUMINATION_CONSTANT
//
//  The material should be a constant colour, without lighting
//    or shading.  The diffuse colour is used.
//
//  This corresponds to MTL illumination mode 0.
//

	static const unsigned int ILLUMINATION_CONSTANT;

//
//  ILLUMINATION_PHONG_NO_SPECULAR
//
//  The material use Phong shading without specular highlights.
//
//  This corresponds to MTL illumination mode 1.
//

	static const unsigned int
				 ILLUMINATION_PHONG_NO_SPECULAR;

//
//  ILLUMINATION_PHONG
//
//  The material use Phong shading, including specular.
//
//  This corresponds to MTL illumination mode 2.
//

	static const unsigned int ILLUMINATION_PHONG;

//
//  ILLUMINATION_RAY_REFLECTIVE
//
//  The material use Phong shading, including specular.  The
//    model also reflects the enviroment.
//
//  Requires ray tracing.
//
//  This corresponds to MTL illumination mode 3.
//

	static const unsigned int ILLUMINATION_RAY_REFLECTIVE;

//
//  ILLUMINATION_RAY_GLASS
//
//  The material use Phong shading, including specular.  The
//    specular highlights are applied seperatly, and do not
//    become transparent when the rest of the model does.  The
//    model also reflects the enviroment.
//
//  Requires ray tracing.
//
//  This corresponds to MTL illumination mode 4.
//

	static const unsigned int ILLUMINATION_RAY_GLASS;

//
//  ILLUMINATION_RAY_FRESNEL
//
//  The material use Phong shading, including specular.  The
//    model also reflects the enviroment and includes Fresnel
//    effects.
//
//  Requires ray tracing.
//
//  This corresponds to MTL illumination mode 5.
//

	static const unsigned int ILLUMINATION_RAY_FRESNEL;

//
//  ILLUMINATION_RAY_REFRACTION
//
//  The material use Phong shading, including specular.  The
//    model also reflects the enviroment.  Light passing through
//    this model is refracted and colourized by its passage.
//
//  Requires ray tracing.
//
//  This corresponds to MTL illumination mode 6.
//

	static const unsigned int ILLUMINATION_RAY_REFRACTION;

//
//  ILLUMINATION_RAY_FRESNEL_REFRACTION
//
//  The material use Phong shading, including specular.  The
//    model also reflects the enviroment and includes Fresnel
//    effects.  Light passing through this model is refracted
//    and colourized by its passage.
//
//  Requires ray tracing.
//
//  This corresponds to MTL illumination mode 7.
//

	static const unsigned int
			    ILLUMINATION_RAY_FRESNEL_REFRACTION;

//
//  ILLUMINATION_REFLECTIVE
//
//  The material use Phong shading, including specular.  The
//    model also reflects an enviroment map.
//
//  This corresponds to MTL illumination mode 8.
//

	static const unsigned int ILLUMINATION_REFLECTIVE;

//
//  ILLUMINATION_GLASS
//
//  The material use Phong shading, including specular.  The
//    specular highlights are applied seperatly, and do not
//    become transparent when the rest of the model does.  The
//    model also reflects an enviroment map.
//
//  This corresponds to MTL illumination mode 9.
//

	static const unsigned int ILLUMINATION_GLASS;

//
//  ILLUMINATION_RAY_INVISIBLE_SHADOWS
//
//  The material is used to cast shadows onto an invisible
//    surface.  I do not know if this corresponds to the
//    invisible surface or the shadow-caster.
//
//  Requires ray tracing.
//
//  This corresponds to MTL illumination mode 10.
//

	static const unsigned int
			     ILLUMINATION_RAY_INVISIBLE_SHADOWS;

//
//  Class Function: isValidChannel
//
//  Purpose: To determine if the specified channel can be used
//	     for monochrome data.
//  Parameter(s):
//	<1> channel: Which channel
//  Precondition(s):  N/A
//  Returns: Whether channel channel can be used for monochrome
//	     data.
//  Side Effect: N/A
//

	static bool isValidChannel (char channel);

//
//  Class Function: isValidIlluminationMode
//
//  Purpose: To determine if the specified illumination mode is
//	     valid.
//  Parameter(s):
//	<1> mode: Which illumination mode
//  Precondition(s):  N/A
//  Returns: Whether mode is a valid illumination mode.
//  Side Effect: N/A
//

	static bool isValidIlluminationMode (unsigned int mode);

//
//  Class Function: isMaterialActive
//
//  Purpose: To determine if there is currently a Material that
//	     has been activated but not deactivated.
//  Parameter(s): N/A
//  Precondition(s):  N/A
//  Returns: Whether there is a currently active Material.
//  Side Effect: N/A
//

	static bool isMaterialActive ();

//
//  Class Function: deactivate
//
//  Purpose: To deactivate the currently active Material, if
//	     any.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The current active material, if any, is
//		 deactivated.
//

	static void deactivate ();

public:
//
//  Default Constructor
//
//  Purpose: To create a new Material with default values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Material is created with an ambient of
//		 (0.2, 0.2, 0.2), a diffuse of (0.8, 0.8, 0.8),
//		 and a specular of (1.0, 1.0, 1.0) and a
//		 specular exponent of 0.0.
//

	Material ();

//
//  Constructor
//
//  Purpose: To create a new Material with the specified name.
//  Parameter(s):
//	<1> name: The name
//  Precondition(s):
//	<1> name.compare("") != 0
//  Returns: N/A
//  Side Effect: A new Material with name name is created with
//		 an ambient of (0.2, 0.2, 0.2), a diffuse of
//		 (0.8, 0.8, 0.8), and a specular of
//		 (1.0, 1.0, 1.0) and a specular exponent of 0.0.
//

	Material (const std::string& name);

//
//  Copy Constructor
//
//  Purpose: To create a new Material as a copy of another.
//  Parameter(s):
//	<1> original: The Material to copy
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: A new Material is created as an exact copy of
//		 original.
//

	Material (const Material& original);

//
//  Assignment Operator
//
//  Purpose: To modify this Material to be copy of another.
//  Parameter(s):
//	<1> original: The Material to copy
//  Precondition(s): N/A
//  Returns: A reference to this Material.
//  Side Effect: This Material is changed into an exact copy of
//		 original.  The existing contents of this
//		 Material are lost.
//

	Material& operator= (const Material& original);

//
//  Destructor
//
//  Purpose: To safely destroy this Material without memory
//	     leaks.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//

	~Material ();

//
//  getName
//
//  Purpose: To determine the name of this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The name of this Material.
//  Side Effect: N/A
//

	const std::string& getName () const;

//
//  getIlluminationMode
//
//  Purpose: To determine the illumination mode for this
//	     Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The illumination mode for this Material.
//  Side Effect: N/A
//

	unsigned int getIlluminationMode () const;

//
//  isAmbientDefault
//
//  Purpose: To determine if this Material has the default
//	     ambient colour.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has the default ambient
//	     colour.
//  Side Effect: N/A
//

	bool isAmbientDefault () const;

//
//  getAmbientRed
//  getAmbientGreen
//  getAmbientBlue
//
//  Purpose: To determine the red/green/blue component of the
//	     ambient colour for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The red/green/blue component of the ambient colour
//	     for this Material.
//  Side Effect: N/A
//

	double getAmbientRed () const;
	double getAmbientGreen () const;
	double getAmbientBlue () const;

//
//  isAmbientMap
//
//  Purpose: To determine if there is an ambient map specified
//	     for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has an ambient map specified.
//  Side Effect: N/A
//

	bool isAmbientMap () const;

//
//  getAmbientMapFilename
//
//  Purpose: To determine the name of the file containing the
//	     ambient map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isAmbientMap()
//  Returns: The filename for the ambient map.
//  Side Effect: N/A
//

	const std::string& getAmbientMapFilename () const;

//
//  isAmbientMapLoaded
//
//  Purpose: To determine if texture for the ambient map for
//	     this Material is loaded.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isAmbientMap()
//  Returns: Whether the texture name for the ambient map has
//	     been loaded into memory.
//  Side Effect: N/A
//

	bool isAmbientMapLoaded () const;

//
//  getAmbientMap
//
//  Purpose: To retrieve the Texture for the ambient map for
//	     this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isAmbientMap()
//  Returns: A pointer to the Texture for the ambient map.  If
//	     the texture cannot be loaded, NULL is returned.
//  Side Effect: If the ambient texture map has not been loaded
//		 into memory, it is loaded.
//

	const Texture* getAmbientMap ();

//
//  getAmbientMap
//
//  Purpose: To retrieve the Texture for the ambient map for
//	     this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isAmbientMap()
//  Returns: A pointer to the Texture for the ambient map.  If
//	     the texture is not loaded, NULL is returned.
//  Side Effect: N/A.
//

	const Texture* getAmbientMap () const;

//
//  isDiffuseDefault
//
//  Purpose: To determine if this Material has the default
//	     diffuse colour.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has the default diffuse
//	     colour.
//  Side Effect: N/A
//

	bool isDiffuseDefault () const;

//
//  getDiffuseRed
//  getDiffuseGreen
//  getDiffuseBlue
//
//  Purpose: To determine the red/green/blue component of the
//	     diffuse colour for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The red/green/blue component of the diffuse colour
//	     for this Material.
//  Side Effect: N/A
//

	double getDiffuseRed () const;
	double getDiffuseGreen () const;
	double getDiffuseBlue () const;

//
//  isDiffuseMap
//
//  Purpose: To determine if there is an diffuse map specified
//	     for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has an diffuse map specified.
//  Side Effect: N/A
//

	bool isDiffuseMap () const;

//
//  getDiffuseMapFilename
//
//  Purpose: To determine the name of the file containing the
//	     diffuse map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDiffuseMap()
//  Returns: The filename for the diffuse map.
//  Side Effect: N/A
//

	const std::string& getDiffuseMapFilename () const;

//
//  isDiffuseMapLoaded
//
//  Purpose: To determine if texture for the diffuse map for
//	     this Material is loaded.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDiffuseMap()
//  Returns: Whether the texture name for the diffuse map has
//	     been loaded into memory.
//  Side Effect: N/A
//

	bool isDiffuseMapLoaded () const;

//
//  getDiffuseMap
//
//  Purpose: To retrieve the Texture for the diffuse map for
//	     this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDiffuseMap()
//  Returns: A pointer to the Texture for the diffuse map.  If
//	     the texture cannot be loaded, NULL is returned.
//  Side Effect: If the diffuse texture map has not been loaded
//		 into memory, it is loaded.
//

	const Texture* getDiffuseMap ();

//
//  getDiffuseMap
//
//  Purpose: To retrieve the Texture for the diffuse map for
//	     this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDiffuseMap()
//  Returns: A pointer to the Texture for the diffuse map.  If
//	     the texture is not loaded, NULL is returned.
//  Side Effect: N/A.
//

	const Texture* getDiffuseMap () const;

//
//  isSpecularDefault
//
//  Purpose: To determine if this Material has the default
//	     specular colour.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has the default specular
//	     colour.
//  Side Effect: N/A
//

	bool isSpecularDefault () const;

//
//  getSpecularRed
//  getSpecularGreen
//  getSpecularBlue
//
//  Purpose: To determine the red/green/blue component of the
//	     specular colour for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The red/green/blue component of the specular colour
//	     for this Material.
//  Side Effect: N/A
//

	double getSpecularRed () const;
	double getSpecularGreen () const;
	double getSpecularBlue () const;

//
//  isSpecularMap
//
//  Purpose: To determine if there is an specular map specified
//	     for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has an specular map
//	     specified.
//  Side Effect: N/A
//

	bool isSpecularMap () const;

//
//  getSpecularMapFilename
//
//  Purpose: To determine the name of the file containing the
//	     specular map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularMap()
//  Returns: The filename for the specular map.
//  Side Effect: N/A
//

	const std::string& getSpecularMapFilename () const;

//
//  isSpecularMapLoaded
//
//  Purpose: To determine if texture for the specular map for
//	     this Material is loaded.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularMap()
//  Returns: Whether the texture name for the specular map has
//	     been loaded into memory.
//  Side Effect: N/A
//

	bool isSpecularMapLoaded () const;

//
//  getSpecularMap
//
//  Purpose: To retrieve the Texture for the specular map for
//	     this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularMap()
//  Returns: A pointer to the Texture for the specular map.  If
//	     the texture cannot be loaded, NULL is returned.
//  Side Effect: If the specular texture map has not been loaded
//		 into memory, it is loaded.
//

	const Texture* getSpecularMap ();

//
//  getSpecularMap
//
//  Purpose: To retrieve the Texture for the specular map for
//	     this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularMap()
//  Returns: A pointer to the Texture for the specular map.  If
//	     the texture is not loaded, NULL is returned.
//  Side Effect: N/A.
//

	const Texture* getSpecularMap () const;

//
//  isSpecularExponentDefault
//
//  Purpose: To determine if this Material has the default
//	     specular exponent.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has the default specular
//	     exponent.
//  Side Effect: N/A
//

	bool isSpecularExponentDefault () const;

//
//  getSpecularExponent
//
//  Purpose: To determine the specular exponent for this
//	     Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The specular exponent for this Material.
//  Side Effect: N/A
//

	double getSpecularExponent () const;

//
//  isSpecularExponentMap
//
//  Purpose: To determine if there is an specular exponent map
//	     specified for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has an specular exponent map
//	     specified.
//  Side Effect: N/A
//

	bool isSpecularExponentMap () const;

//
//  getSpecularExponentMapFilename
//
//  Purpose: To determine the name of the file containing the
//	     specular exponent map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularExponentMap()
//  Returns: The filename for the specular exponent map.
//  Side Effect: N/A
//

	const std::string& getSpecularExponentMapFilename ()
							  const;

//
//  isSpecularExponentMapLoaded
//
//  Purpose: To determine if texture for the specular exponent
//	     map for this Material is loaded.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularExponentMap()
//  Returns: Whether the texture name for the specular exponent
//	     map has been loaded into memory.
//  Side Effect: N/A
//

	bool isSpecularExponentMapLoaded () const;

//
//  getSpecularExponentMap
//
//  Purpose: To retrieve the Texture for the specular exponent
//	     map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularExponentMap()
//  Returns: A pointer to the Texture for the specular exponent
//	     map.  If the texture cannot be loaded, NULL is
//	     returned.
//  Side Effect: If the specular exponent texture map has not
//		 been loaded into memory, it is loaded.
//

	const Texture* getSpecularExponentMap ();

//
//  getSpecularExponentMap
//
//  Purpose: To retrieve the Texture for the specular exponent
//	     map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularExponentMap()
//  Returns: A pointer to the Texture for the specular exponent
//	     map.  If the texture is not loaded, NULL is
//	     returned.
//  Side Effect: N/A.
//

	const Texture* getSpecularExponentMap () const;

//
//  isSpecularExponentMapChannelSet
//
//  Purpose: To determine if the channel of the specular
//	     exponent map this Material uses has been set.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularExponentMap()
//  Returns: Whether the channel of the specular exponent map to
//	     use has been set.
//  Side Effect: N/A
//

	bool isSpecularExponentMapChannelSet () const;

//
//  getSpecularExponentMapChannel
//
//  Purpose: To determine the channel of the specular exponent
//	     map to use.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSpecularExponentMap()
//  Returns: The channel of specular exponent map to use.
//  Side Effect: N/A
//

	char getSpecularExponentMapChannel () const;

//
//  isTransparencyDefault
//
//  Purpose: To determine if this Material has the default
//	     transparency.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has the default transparency.
//  Side Effect: N/A
//

	bool isTransparencyDefault () const;

//
//  getTransparency
//
//  Purpose: To determine the transparency of this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The transparency of this Material.
//  Side Effect: N/A
//

	double getTransparency () const;

//
//  isTransparencyMap
//
//  Purpose: To determine if there is a transparency map
//	     specified for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has a transparency map
//	     specified.
//  Side Effect: N/A
//

	bool isTransparencyMap () const;

//
//  getTransparencyMapFilename
//
//  Purpose: To determine the name of the file containing the
//	     transparency map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isTransparencyMap()
//  Returns: The filename for the transparency map.
//  Side Effect: N/A
//

	const std::string& getTransparencyMapFilename () const;

//
//  isTransparencyMapLoaded
//
//  Purpose: To determine if texture for the transparency map
//	     for this Material is loaded.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isTransparencyMap()
//  Returns: Whether the texture name for the transparency map
//	     has been loaded into memory.
//  Side Effect: N/A
//

	bool isTransparencyMapLoaded () const;

//
//  getTransparencyMap
//
//  Purpose: To retrieve the Texture for the transparency map
//	     for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isTransparencyMap()
//  Returns: A pointer to the Texture for the transparency map.
//	     If the texture cannot be loaded, NULL is returned.
//  Side Effect: If the transparency texture map has not been
//		 loaded into memory, it is loaded.
//

	const Texture* getTransparencyMap ();

//
//  getTransparencyMap
//
//  Purpose: To retrieve the Texture for the transparency map
//	     for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isTransparencyMap()
//  Returns: A pointer to the Texture for the transparency map.
//	     If the texture is not loaded, NULL is returned.
//  Side Effect: N/A.
//

	const Texture* getTransparencyMap () const;

//
//  isTransparencyMapChannelSet
//
//  Purpose: To determine if the channel of the transparency map
//	     this Material uses has been set.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isTransparencyMap()
//  Returns: Whether the channel of the transparency map to use
//	     has been set.
//  Side Effect: N/A
//

	bool isTransparencyMapChannelSet () const;

//
//  getTransparencyMapChannel
//
//  Purpose: To determine the channel of the transparency map to
//	     use.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isTransparencyMap()
//  Returns: The channel of transparency map to use.
//  Side Effect: N/A
//

	char getTransparencyMapChannel () const;

//
//  isOpticalDensityDefault
//
//  Purpose: To determine if this Material has the default
//	     optical density.  The optical density is better
//	     known as the index of refraction.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has the default optical
//	     density.
//  Side Effect: N/A
//

	bool isOpticalDensityDefault () const;

//
//  getOpticalDensity
//
//  Purpose: To determine the optical density for this Material.
//	     The optical density is better known as the index of
//	     refraction.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The optical density for this Material.
//  Side Effect: N/A
//

	double getOpticalDensity () const;

//
//  isTransmissionFilterDefault
//
//  Purpose: To determine if this Material has the default
//	     transmission filter.  The transmission filter
//	     determines what fraction of each colour of light
//	     passes through this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has the default transmission
//	     filter.
//  Side Effect: N/A
//

	bool isTransmissionFilterDefault () const;

//
//  getTransmissionFilterRed
//  getTransmissionFilterGreen
//  getTransmissionFilterBlue
//
//  Purpose: To determine the red/green/blue component of the
//	     transmission filter for this Material.  The
//	     transmission filter determines what fraction of
//	     each colour of light passes through this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: The red/green/blue component of the transmission
//	     filter for this Material.
//  Side Effect: N/A
//

	double getTransmissionFilterRed () const;
	double getTransmissionFilterGreen () const;
	double getTransmissionFilterBlue () const;

//
//  isDecalMap
//
//  Purpose: To determine if there is a decal map specified for
//	     this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has a decal map specified.
//  Side Effect: N/A
//

	bool isDecalMap () const;

//
//  getDecalMapFilename
//
//  Purpose: To determine the name of the file containing the
//	     decal map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDecalMap()
//  Returns: The filename for the decal map.
//  Side Effect: N/A
//

	const std::string& getDecalMapFilename () const;

//
//  isDecalMapLoaded
//
//  Purpose: To determine if texture for the decal map for this
//	     Material is loaded.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDecalMap()
//  Returns: Whether the texture name for the decal map has been
//	     loaded into memory.
//  Side Effect: N/A
//

	bool isDecalMapLoaded () const;

//
//  getDecalMap
//
//  Purpose: To retrieve the Texture for the decal map for this
//	     Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDecalMap()
//  Returns: A pointer to the Texture for the decal map.  If the
//	     texture cannot be loaded, NULL is returned.
//  Side Effect: If the decal texture map has not been loaded
//		 into memory, it is loaded.
//

	const Texture* getDecalMap ();

//
//  getDecalMap
//
//  Purpose: To retrieve the Texture for the decal map for this
//	     Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDecalMap()
//  Returns: A pointer to the Texture for the decal map.  If the
//	     texture is not loaded, NULL is returned.
//  Side Effect: N/A.
//

	const Texture* getDecalMap () const;

//
//  isDecalMapChannelSet
//
//  Purpose: To determine if the channel of the decal map this
//	     Material uses has been set.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDecalMap()
//  Returns: Whether the channel of the decal map to use has
//	     been set.
//  Side Effect: N/A
//

	bool isDecalMapChannelSet () const;

//
//  getDecalMapChannel
//
//  Purpose: To determine the channel of the decal map to use.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDecalMap()
//  Returns: The channel of decal map to use.
//  Side Effect: N/A
//

	char getDecalMapChannel () const;

//
//  isDisplacementMap
//
//  Purpose: To determine if there is a displacement map
//	     specified for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has a displacement map
//	     specified.
//  Side Effect: N/A
//

	bool isDisplacementMap () const;

//
//  getDisplacementMapFilename
//
//  Purpose: To determine the name of the file containing the
//	     displacement map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDisplacementMap()
//  Returns: The filename for the displacement map.
//  Side Effect: N/A
//

	const std::string& getDisplacementMapFilename () const;

//
//  isDisplacementMapLoaded
//
//  Purpose: To determine if texture for the displacement map
//	     for this Material is loaded.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDisplacementMap()
//  Returns: Whether the texture name for the displacement map
//	     has been loaded into memory.
//  Side Effect: N/A
//

	bool isDisplacementMapLoaded () const;

//
//  getDisplacementMap
//
//  Purpose: To retrieve the Texture for the displacement map
//	     for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDisplacementMap()
//  Returns: A pointer to the Texture for the displacement map.
//	     If the texture cannot be loaded, NULL is returned.
//  Side Effect: If the displacement texture map has not been
//		 loaded into memory, it is loaded.
//

	const Texture* getDisplacementMap ();

//
//  getDisplacementMap
//
//  Purpose: To retrieve the Texture for the displacement map
//	     for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDisplacementMap()
//  Returns: A pointer to the Texture for the displacement map.
//	     If the texture is not loaded, NULL is returned.
//  Side Effect: N/A.
//

	const Texture* getDisplacementMap () const;

//
//  isDisplacementMapChannelSet
//
//  Purpose: To determine if the channel of the displacement map
//	     this Material uses has been set.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDisplacementMap()
//  Returns: Whether the channel of the displacement map to use
//	     has been set.
//  Side Effect: N/A
//

	bool isDisplacementMapChannelSet () const;

//
//  getDisplacementMapChannel
//
//  Purpose: To determine the channel of the displacement map to
//	     use.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isDisplacementMap()
//  Returns: The channel of displacement map to use.
//  Side Effect: N/A
//

	char getDisplacementMapChannel () const;

//
//  isBumpMap
//
//  Purpose: To determine if there is a bump map specified for
//	     this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material has a bump map specified.
//  Side Effect: N/A
//

	bool isBumpMap () const;

//
//  getBumpMapFilename
//
//  Purpose: To determine the name of the file containing the
//	     bump map for this Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isBumpMap()
//  Returns: The filename for the bump map.
//  Side Effect: N/A
//

	const std::string& getBumpMapFilename () const;

//
//  isBumpMapLoaded
//
//  Purpose: To determine if texture for the bump map for
//	     this Material is loaded.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isBumpMap()
//  Returns: Whether the texture name for the bump map has
//	     been loaded into memory.
//  Side Effect: N/A
//

	bool isBumpMapLoaded () const;

//
//  getBumpMap
//
//  Purpose: To retrieve the Texture for the bump map for this
//	     Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isBumpMap()
//  Returns: A pointer to the Texture for the bump map.  If the
//	     texture cannot be loaded, NULL is returned.
//  Side Effect: If the bump texture map has not been loaded
//		 into memory, it is loaded.
//

	const Texture* getBumpMap ();

//
//  getBumpMap
//
//  Purpose: To retrieve the Texture for the bump map for this
//	     Material.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isBumpMap()
//  Returns: A pointer to the Texture for the bump map.  If the
//	     texture is not loaded, NULL is returned.
//  Side Effect: N/A.
//

	const Texture* getBumpMap () const;

//
//  isBumpMapChannelSet
//
//  Purpose: To determine if the channel of the bump map this
//	     Material uses has been set.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isBumpMap()
//  Returns: hether the channel of the bump map to use has been
//	     set.
//  Side Effect: N/A
//

	bool isBumpMapChannelSet () const;

//
//  getBumpMapChannel
//
//  Purpose: To determine the channel of the bump map to use.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isBumpMap()
//  Returns: The channel of bump map to use.
//  Side Effect: N/A
//

	char getBumpMapChannel () const;

//
//  isBumpMapMultiplierDefault
//
//  Purpose: To determine if this Material uses the default
//	     multplier for the bump map.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isBumpMap()
//  Returns: Whether the the bump map uses the default
//	     multiplier.
//  Side Effect: N/A
//

	bool isBumpMapMultiplierDefault () const;

//
//  getBumpMapMultiplier
//
//  Purpose: To determine the multiplier for the bump map.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isBumpMap()
//  Returns: The multiplier for the bump map.
//  Side Effect: N/A
//

	double getBumpMapMultiplier () const;

//
//  print
//
//  Purpose: To print theis Material to standard output.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material printed to standard output.
//

	void print () const;

//
//  activate
//
//  Purpose: To set the OpenGL drawing state to correspond to
//	     this Material.  After the drawing has been
//	     completed, deactivate() must be called before
//	     another material can be activated.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> !isMaterialActive()
//  Returns: N/A
//  Side Effect: The current OpenGL state is set to draw
//		 according to this Material.  If the appropriate
//		 textures have not been loaded, they are loaded.
//

	void activate () const;

//
//  isSeperateSpecular
//
//  Purpose: To determine if specular lighting should be
//	     calculated seperately for this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: Whether this Material should be displayed with
//	     seperate specular highlights.
//  Side Effect: N/A
//

	bool isSeperateSpecular () const;

//
//  activateSeperateSpecular
//
//  Purpose: To set the OpenGL drawing state to correspond to
//	     the specular component of this Material.  This
//	     function should only be used to draw the specular
//	     hightlights on a model after the model itself was
//	     drawn.  If the model has not yet been drawn, the
//	     specular highlights will not be displayed.
//	     deactivate() must be called before another material
//	     can be activated.
//  Parameter(s): N/A
//  Precondition(s):
//	<1> isSeperateSpecular()
//	<2> !isMaterialActive()
//  Returns: N/A
//  Side Effect: The current OpenGL state is set to draw the
//		 specular component according to this Material.
//

	void activateSeperateSpecular () const;

//
//  save
//
//  Purpose: To write the contents of this Material to the
//	     specified output stream.
//  Parameter(s):
//	<1> r_out: A reference to the output stream
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This ObjModel written to r_out in MTL format.
//

	void save (std::ostream& r_out) const;

//
//  setName
//
//  Purpose: To change the name of this Material.
//  Parameter(s):
//	<1> name: The new name
//  Precondition(s):
//	<1> name.compare("") != 0
//  Returns: N/A
//  Side Effect: This Material is set to have name name.
//

	void setName (const std::string& name);

//
//  setIlluminationMode
//
//  Purpose: To change the illumination mode for this
//	     Material.
//  Parameter(s):
//	<1> mode: The new illumination mode
//  Precondition(s):
//	<1> isValidIlluminationMode(mode)
//  Returns: N/A
//  Side Effect: This Material is set to have illumination mode
//		 mode.  mode may be either a named constant or a
//		 MTL illumination number.
//

	void setIlluminationMode (unsigned int mode);

//
//  setAmbientRed
//  setAmbientGreen
//  setAmbientBlue
//
//  Purpose: To change the red/green/blue component of the
//	     ambient colour for this Material.
//  Parameter(s):
//	<1> red
//	<1> green
//	<1> blue: The new red/green/blue componment of the
//		  ambient colour
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The red/green/blue ambient colour component of
//		 this Material is set to red/green/blue.
//

	void setAmbientRed   (double red);
	void setAmbientGreen (double green);
	void setAmbientBlue  (double blue);

//
//  setAmbientColour
//
//  Purpose: To change the ambient colour for this Material.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The new ambient colour
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The ambient colour of this Material is set to
//		 (red, green, blue).
//

	void setAmbientColour (double red, double green,
			       double blue);

//
//  setAmbientMap
//
//  Purpose: To mark this Material as using the specified file
//	     as an ambient map.
//  Parameter(s):
//	<1> filename: The name of the file containing the
//		      ambient map
//  Precondition(s):
//	<1> filename.compare("") != 0
//  Returns: N/A
//  Side Effect: This Material is set to use file filename as an
//		 ambient map.
//

	void setAmbientMap (const std::string& filename);

//
//  setAmbientMapNone
//
//  Purpose: To mark this Material as not having an ambient map.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material is set to not have an ambient
//		 map.
//

	void setAmbientMapNone ();

//
//  setDiffuseRed
//  setDiffuseGreen
//  setDiffuseBlue
//
//  Purpose: To change the red/green/blue component of the
//	     diffuse colour for this Material.
//  Parameter(s):
//	<1> red
//	<1> green
//	<1> blue: The new red/green/blue componment of the
//		  diffuse colour
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The red/green/blue diffuse colour component of
//		 this Material is set to red/green/blue.
//

	void setDiffuseRed   (double red);
	void setDiffuseGreen (double green);
	void setDiffuseBlue  (double blue);

//
//  setDiffuseColour
//
//  Purpose: To change the diffuse colour for this Material.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The new diffuse colour
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The diffuse colour of this Material is set to
//		 (red, green, blue).
//

	void setDiffuseColour (double red, double green,
			       double blue);

//
//  setDiffuseMap
//
//  Purpose: To mark this Material as using the specified file
//	     as an diffuse map.
//  Parameter(s):
//	<1> filename: The name of the file containing the
//		      diffuse map
//  Precondition(s):
//	<1> filename.compare("") != 0
//  Returns: N/A
//  Side Effect: This Material is set to use file filename as an
//		 diffuse map.
//

	void setDiffuseMap (const std::string& filename);

//
//  setDiffuseMapNone
//
//  Purpose: To mark this Material as not having an diffuse map.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material is set to not have an diffuse
//		 map.
//

	void setDiffuseMapNone ();

//
//  setSpecularRed
//  setSpecularGreen
//  setSpecularBlue
//
//  Purpose: To change the red/green/blue component of the
//	     specular colour for this Material.
//  Parameter(s):
//	<1> red
//	<1> green
//	<1> blue: The new red/green/blue componment of the
//		  specular colour
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The red/green/blue specular colour component of
//		 this Material is set to red/green/blue.
//

	void setSpecularRed   (double red);
	void setSpecularGreen (double green);
	void setSpecularBlue  (double blue);

//
//  setSpecularColour
//
//  Purpose: To change the specular colour for this Material.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The new specular colour
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The specular colour of this Material is set to
//		 (red, green, blue).
//

	void setSpecularColour (double red, double green,
				double blue);

//
//  setSpecularMap
//
//  Purpose: To mark this Material as using the specified file
//	     as an specular map.
//  Parameter(s):
//	<1> filename: The name of the file containing the
//		      specular map
//  Precondition(s):
//	<1> filename.compare("") != 0
//  Returns: N/A
//  Side Effect: This Material is set to use file filename as an
//		 specular map.
//

	void setSpecularMap (const std::string& filename);

//
//  setSpecularMapNone
//
//  Purpose: To mark this Material as not having an specular map.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material is set to not have an specular
//		 map.
//

	void setSpecularMapNone ();

//
//  setSpecularExponent
//
//  Purpose: To change the specular exponent for this Material.
//  Parameter(s):
//	<1> exponent: The new specular exponent
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The specular exponent of this Material is set
//		 to exponent.
//

	void setSpecularExponent (double exponent);

//
//  setSpecularExponentMap
//
//  Purpose: To mark this Material as using the specified file
//	     as an specular exponent map.
//  Parameter(s):
//	<1> filename: The name of the file containing the
//		      specular exponent map
//	<2> channel: The channel of the specular exponent map to
//		     use
//  Precondition(s):
//	<1> filename.compare("") != 0
//	<2> isValidChannel(channel)
//  Returns: N/A
//  Side Effect: This Material is set to use channel channel of
//		 file filename as an specular exponent map.
//

	void setSpecularExponentMap(const std::string& filename,
				    char channel);

//
//  setSpecularExponentMapNone
//
//  Purpose: To mark this Material as not having an specular
//	     exponent map.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material is set to not have an specular
//		 exponent map.
//

	void setSpecularExponentMapNone ();

//
//  setTransparency
//
//  Purpose: To change the transparency for this Material.
//  Parameter(s):
//	<1> transparency: The new transparency
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The transparency of this Material is set to
//		 transparency.
//

	void setTransparency (double transparency);

//
//  setTransparencyMap
//
//  Purpose: To mark this Material as using the specified file
//	     as a transparency map.
//  Parameter(s):
//	<1> filename: The name of the file containing the
//		      transparency map
//	<2> channel: The channel of the transparency map to use
//  Precondition(s):
//	<1> filename.compare("") != 0
//	<2> isValidChannel(channel)
//  Returns: N/A
//  Side Effect: This Material is set to use channel channel of
//		 file filename as a transparency map.
//

	void setTransparencyMap (const std::string& filename,
				 char channel);

//
//  setTransparencyMapNone
//
//  Purpose: To mark this Material as not having a transparency
//	     map.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material is set to not have a transparency
//		 map.
//

	void setTransparencyMapNone ();

//
//  setOpticalDensity
//
//  Purpose: To change the optical density for this Material.
//	     The optical density is better known as the index of
//	     refraction.
//  Parameter(s):
//	<1> density: The new optical density
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The optical density of this Material is set to
//		 density.
//

	void setOpticalDensity (double density);

//
//  setTransmissionFilterRed
//  setTransmissionFilterGreen
//  setTransmissionFilterBlue
//
//  Purpose: To change the red/green/blue component of the
//	     transmission filter for this Material.  The
//	     transmission filter determines what fraction of
//	     each colour of light passes through this Material.
//  Parameter(s):
//	<1> red
//	<1> green
//	<1> blue: The new red/green/blue componment of the
//		  transmission filter
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The red/green/blue transmission filter
//		 component of this Material is set to
//		 red/green/blue.
//

	void setTransmissionFilterRed   (double red);
	void setTransmissionFilterGreen (double green);
	void setTransmissionFilterBlue  (double blue);

//
//  setTransmissionFilter
//
//  Purpose: To change the transmission filter for this
//	     Material.
//  Parameter(s):
//	<1> red
//	<2> green
//	<3> blue: The new transmission filter
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The transmission filter of this Material is set
//		 to (red, green, blue).
//

	void setTransmissionFilter (double red, double green,
				    double blue);

//
//  setDecalMap
//
//  Purpose: To mark this Material as using the specified file
//	     as a decal map.
//  Parameter(s):
//	<1> filename: The name of the file containing the decal
//		      map
//	<2> channel: The channel of the decal map to use
//  Precondition(s):
//	<1> filename.compare("") != 0
//	<2> isValidChannel(channel)
//  Returns: N/A
//  Side Effect: This Material is set to use channel channel of
//		 file filename as a decal map.
//

	void setDecalMap (const std::string& filename,
			  char channel);

//
//  setDecalMapNone
//
//  Purpose: To mark this Material as not having a decal map.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material is set to not have a decal map.
//

	void setDecalMapNone ();

//
//  setDisplacementMap
//
//  Purpose: To mark this Material as using the specified file
//	     as a displacement map.
//  Parameter(s):
//	<1> filename: The name of the file containing the
//		      displacement map
//	<2> channel: The channel of the displacement map to use
//  Precondition(s):
//	<1> filename.compare("") != 0
//	<2> isValidChannel(channel)
//  Returns: N/A
//  Side Effect: This Material is set to use channel channel of
//		 file filename as an displacement map.
//

	void setDisplacementMap (const std::string& filename,
				 char channel);

//
//  setDisplacementMapNone
//
//  Purpose: To mark this Material as not having a displacement
//	     map.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material is set to not have a displacement
//		 map.
//

	void setDisplacementMapNone ();

//
//  setBumpMap
//
//  Purpose: To mark this Material as using the specified file
//	     as a bump map.
//  Parameter(s):
//	<1> filename: The name of the file containing the
//		      bump map
//	<2> channel: The channel of the bump map to use
//	<3> multiplier: The bump height multiplier
//  Precondition(s):
//	<1> filename.compare("") != 0
//	<2> isValidChannel(channel)
//  Returns: N/A
//  Side Effect: This Material is set to use channel channel of
//		 file filename as a bump map with bump height
//		 multiplier mmultiplier.
//

	void setBumpMap (const std::string& filename,
			 char channel, double multiplier);

//
//  setBumpMapNone
//
//  Purpose: To mark this Material as not having a bump map.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material is set to not have an bump map.
//

	void setBumpMapNone ();

//
//  setBumpMapMultiplier
//
//  Purpose: To determine the multiplier for the bump map for
//	     this Material.
//  Parameter(s):
//	<1> multiplier: The new bump map multiplier
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: The bump map multiplier is set to mult.
//

	void setBumpMapMultiplier (double multiplier);

//
//  makeDefault
//
//  Purpose: To set all fields in this Material to their default
//	     values.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: This Material is set to be a default material.
//		 Any existing values are lost.
//

	void makeDefault ();

//
//  loadDisplayTextures
//
//  Purpose: To load all textures that are actually used when
//	     displaying this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All textures that would be needed to display
//		 this Material are loaded.
//

	void loadDisplayTextures ();

//
//  loadAllTextures
//
//  Purpose: To load all textures referenced by this Material,
//	     regardless of whether they are actually used.  To
//	     conserve video memory, consider using
//	     loadDisplayTextures() instead.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All textures referenced by this Material are
//		 loaded.
//

	void loadAllTextures ();

private:
//
//  destroy
//
//  Purpose: To free all dynamically allocated memory associated
//	     with this Material.
//  Parameter(s): N/A
//  Precondition(s): N/A
//  Returns: N/A
//  Side Effect: All dynamically allocated memory is freed.
//		 mp_ambient_map, mp_diffuse_map, and
//		 mp_specular_map are set to NULL.
//

	void destroy ();

//
//  copy
//
//  Purpose: To copy the values from another Material to this
//	     Material.
//  Parameter(s):
//	<1> original: The Material to copy
//  Precondition(s):
//	<1> mp_ambient_map == NULL
//	<2> mp_diffuse_map == NULL
//	<3> mp_specular_map == NULL
//  Returns: N/A
//  Side Effect: The values from original are copied to this
//		 Material.  Any existing values are lost.
//

	void copy (const Material& original);

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
	//
	//  Colour
	//
	//  A record to represent a colour as RGB components.
	//

	struct Colour
	{
		double m_red;
		double m_green;
		double m_blue;
	};

private:
	std::string m_name;
	unsigned int m_illumination_mode;
	char m_texture_type_display;

	Colour m_ambient_colour;
	std::string m_ambient_filename;
	const Texture* mp_ambient_map;

	Colour m_diffuse_colour;
	std::string m_diffuse_filename;
	const Texture* mp_diffuse_map;

	Colour m_specular_colour;
	std::string m_specular_filename;
	const Texture* mp_specular_map;

	double m_specular_exponent;
	std::string m_specular_exponent_filename;
	const Texture* mp_specular_exponent_map;
	char m_specular_exponent_channel;

	double m_transparency;
	std::string m_transparency_filename;
	const Texture* mp_transparency_map;
	char m_transparency_channel;

	double m_optical_density;
	Colour m_transmission_filter;

	std::string m_decal_filename;
	const Texture* mp_decal_map;
	char m_decal_channel;

	std::string m_displacement_filename;
	const Texture* mp_displacement_map;
	char m_displacement_channel;

	std::string m_bump_filename;
	const Texture* mp_bump_map;
	char m_bump_channel;
	double m_bump_multiplier;
};

#endif
