#ifndef __S_JSON_OBJECT_H_INCLUDED__
#define __S_JSON_OBJECT_H_INCLUDED__

#include <irrlicht.h>
#include <string>

namespace cp3d {
namespace engine {

//! JSON Object type enum
enum E_JSON_OBJECT_TYPE
{
	EJOT_STRING = 0,
	EJOT_NUMBER,
	EJOT_BOOL,
	EJOT_NULL,
	EJOT_OBJECT,
	EJOT_ARRAY
};

//! JSON Object structure
struct SJSONObject
{
public:
	//! ctor
	//! \param value the value of the JSON object
	//! \param type the value type of the JSON object
	SJSONObject(void* value, E_JSON_OBJECT_TYPE type) : Value(value), Type(type)
	{ }

	//! dtor
	~SJSONObject()
	{
		delete Value;
	}

	//! Object's value
	void* Value;

	//! Object's type
	E_JSON_OBJECT_TYPE Type;
};

//! JSON object key type
typedef irr::core::map<irr::core::stringc, SJSONObject*> JSONObjectKeysType;

//! JSON Object tree structure
struct SJSONObjectTree
{
public:
	//! ctor
	SJSONObjectTree()
	{ }

	//! dtor
	~SJSONObjectTree()
	{
		JSONObjectKeysType::Iterator it = Keys.getIterator();
		for (; !it.atEnd(); it++)
		{
			delete it.getNode()->getValue()->Value;
		}

		Keys.clear();
	}

	//! JSON object tree keys (key -> value)
	JSONObjectKeysType Keys;

	//! Returns the given number idenfified by its key in the JSON file
	irr::f64 getNumber(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n ? std::stod((*(irr::core::stringc*) n->getValue()->Value).c_str()) : 0.0;
	}

	//! Returns the given string idenfified by its key in the JSON file
	irr::core::stringc getString(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n ? *(irr::core::stringc*) n->getValue()->Value : "";
	}

	//! Returns the given object idenfified by its key in the JSON file
	SJSONObjectTree* getObject(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n ? (SJSONObjectTree*) n->getValue()->Value : 0;
	}
};

} /// End namespace engine
} /// End namespace engine

#endif
