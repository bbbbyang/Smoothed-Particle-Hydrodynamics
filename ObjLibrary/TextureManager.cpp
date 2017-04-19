//
//  TextureManager.cpp
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
#include "Texture.h"
#include "TextureBmp.h"
#include "TextureManager.h"

#ifdef OBJ_LIBRARY_LOAD_PNG_TEXTURES
	#include "GetGlut.h"
	#include "../glpng.h"
#endif

using namespace std;
using namespace TextureManager;
namespace
{
	struct TextureData
	{
		string m_name;
		Texture m_texture;
	};

	// some sort of hash table would be nice
	//  -> convert to map<> when C++11 becomes more
	//     widely supported?
	vector<TextureData*> g_textures;

	Texture g_white;

	//
	//  getDummyTetxure
	//
	//  Purpose: To retreive a reference to the dummy
	//	     texture.
	//  Parameter(s): N/A
	//  Precondition(s): N/A
	//  Returns: A reference to the dummy texture.
	//  Side Effect: If the dummy texture has not been
	//		 generated, it is generated.
	//

	const Texture& getDummyTexture ()
	{
		if(!g_white.isSet())
		{
			TextureBmp white;
			g_white.set(white.addToOpenGL());
		}

		return g_white;
	}
}



unsigned int TextureManager :: getCount ()
{
	return g_textures.size();
}

const Texture& TextureManager :: get (unsigned int index)
{
	assert(index < getCount());

	return g_textures[index]->m_texture;
}

const Texture& TextureManager :: get (const char* a_name)
{
	assert(a_name != NULL);

	return get(string(a_name), cerr);
}

const Texture& TextureManager :: get (const char* a_name, const string& logfile)
{
	assert(a_name != NULL);
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	const Texture& texture = get(string(a_name), logstream);
	logstream.close();

	return texture;
}

const Texture& TextureManager :: get (const char* a_name, ostream& r_logstream)
{
	assert(a_name != NULL);

	return get(string(a_name), r_logstream);
}

const Texture& TextureManager :: get (const string& name)
{
	return get(name, cerr);
}

const Texture& TextureManager :: get (const string& name, const string& logfile)
{
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	const Texture& texture = get(name, logstream);
	logstream.close();

	return texture;
}

const Texture& TextureManager :: get (const string& name, ostream& r_logstream)
{
	string lower = toLowercase(name);
	unsigned int texture_count = g_textures.size();

	for(unsigned int i = 0; i < texture_count; i++)
		if(g_textures[i]->m_name.compare(lower) == 0)
			return g_textures[i]->m_texture;

	if(endsWith(lower, ".bmp"))
	{
		TextureBmp texture_bmp(name.c_str(), r_logstream);
		if(texture_bmp.isBad())
		{
			// TextureBmp prints loading error
			return getDummyTexture();
		}
		else
		{
			// flip texture to match Maya - remove? <|>
			texture_bmp.mirrorY();
			return add(texture_bmp.addToOpenGL(), lower);
		}
	}
	else if(endsWith(lower, ".png"))
	{
#ifdef OBJ_LIBRARY_LOAD_PNG_TEXTURES
		unsigned int texture_name = pngBind(lower.c_str(), PNG_BUILDMIPMAPS, PNG_SOLID, NULL, GL_REPEAT, GL_NEAREST_MIPMAP_NEAREST, GL_NEAREST);
		if(texture_name == 0)
		{
			r_logstream << "Error: Loading failed: " << lower << endl;
			return getDummyTexture();
		}
		else
			return add(texture_name, lower);
#else
		r_logstream << "Error: Loading .png textures is disabled: " << lower << endl;
		return getDummyTexture();
#endif
	}
	else
	{
		r_logstream << "Error: Invalid image file extension: " << lower << endl;
		return getDummyTexture();
	}
}

void TextureManager :: activate (unsigned int index)
{
	assert(index < getCount());

	g_textures[index]->m_texture.activate();
}

void TextureManager :: activate (const char* a_name)
{
	assert(a_name != NULL);

	get(string(a_name), cerr).activate();
}

void TextureManager :: activate (const char* a_name, const std::string& logfile)
{
	assert(a_name != NULL);
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	get(string(a_name), logstream).activate();
	logstream.close();
}

void TextureManager :: activate (const char* a_name, std::ostream& r_logstream)
{
	assert(a_name != NULL);

	get(string(a_name), r_logstream).activate();
}
		
void TextureManager :: activate (const std::string& name)
{
	get(name, cerr).activate();
}

void TextureManager :: activate (const std::string& name, const std::string& logfile)
{
	assert(logfile.compare("") != 0);

	ofstream logstream(logfile.c_str());
	get(name, logstream).activate();
	logstream.close();
}

void TextureManager :: activate (const std::string& name, std::ostream& r_logstream)
{
	get(name).activate(), r_logstream;
}

bool TextureManager :: isLoaded (const char* a_name)
{
	assert(a_name != NULL);

	return isLoaded(string(a_name));
}

bool TextureManager :: isLoaded (const string& name)
{
	string lower = toLowercase(name);

	for(unsigned int i = 0; i < g_textures.size(); i++)
		if(g_textures[i]->m_name.compare(lower) == 0)
			return true;
	return false;
}

bool TextureManager :: isDummyTexture (const Texture& texture)
{
	if(texture == getDummyTexture())
		return true;
	else
		return false;
}



const Texture& TextureManager :: add (unsigned int opengl_name, const char* a_name)
{
	assert(opengl_name != 0);
	assert(a_name != NULL);
	assert(!isLoaded(a_name));

	return add(Texture(opengl_name), string(a_name));
}

const Texture& TextureManager :: add (unsigned int opengl_name, const string& name)
{
	assert(opengl_name != 0);
	assert(!isLoaded(name));

	return add(Texture(opengl_name), name);
}

const Texture& TextureManager :: add (const Texture& texture, const char* a_name)
{
	assert(texture.isSet());
	assert(a_name != NULL);
	assert(!isLoaded(a_name));

	return add(texture, string(a_name));
}

const Texture& TextureManager :: add (const Texture& texture, const string& name)
{
	assert(texture.isSet());
	assert(!isLoaded(name));

	string lower = toLowercase(name);
	unsigned int texture_count = g_textures.size();

	g_textures.push_back(new TextureData);
	g_textures[texture_count]->m_name = lower;
	g_textures[texture_count]->m_texture = texture;

	return g_textures[texture_count]->m_texture;
}

void unloadAll ()
{
	for(unsigned int i = 0; i < g_textures.size(); i++)
		delete g_textures[i];	// destructor frees video memory
	g_textures.clear();
}
