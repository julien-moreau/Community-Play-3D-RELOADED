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

//! JSON types
struct SJSONObjectTree;
struct SJSONObject;

typedef irr::core::stringc JSONString;
typedef irr::f64 JSONNumber;
typedef irr::core::map<irr::core::stringc, SJSONObject*> JSONObjectKeysType;
typedef irr::core::array<SJSONObject*> JSONArray;
typedef SJSONObjectTree JSONObject;

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

	//! Returns if the value is null
	bool isNull()
	{
		return Type == EJOT_NULL;
	}

	//! Returns value as number
	JSONNumber toNumber()
	{
		return std::stod(((irr::core::stringc*) Value)->c_str());
	}

	//! Returns value as string
	JSONString toString()
	{
		return *(irr::core::stringc*) Value;
	}

	//! Object's value
	void* Value;

	//! Object's type
	E_JSON_OBJECT_TYPE Type;
};

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
			SJSONObject* object = it.getNode()->getValue();
			
			switch (object->Type)
			{
				// Array
				case EJOT_ARRAY:
				{
					irr::core::array<SJSONObject*>& arr = *(irr::core::array<SJSONObject*>*) object->Value;
					const irr::u32 size = arr.size();

					for (irr::u32 i = 0; i < size; i++)
						delete arr[i];

				}
				break;

				// Nothing to do
				default: break;
			}

			delete object;
		}

		Keys.clear();
	}

	//! JSON object tree keys (key -> value)
	JSONObjectKeysType Keys;

	//! Returns the JSON object if exists. If not, returns 0
	bool has(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n != 0;
	}

	//! Returns if the given key is null
	bool isNull(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n ? n->getValue()->Type == EJOT_NULL : true;
	}

	//! Returns the given number idenfified by its key in the JSON file
	JSONNumber getNumber(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n ? std::stod((*(irr::core::stringc*) n->getValue()->Value).c_str()) : 0.0;
	}

	//! Returns the given string idenfified by its key in the JSON file
	JSONString getString(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n ? *(irr::core::stringc*) n->getValue()->Value : "";
	}

	//! Returns the given bool idenfified by its key in the JSON file
	bool getBool(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n ? *(irr::core::stringc*) n->getValue()->Value == "true" : false;
	}

	//! Returns the given array identified by its key in the JSON file
	JSONArray getArray(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n ? *(irr::core::array<SJSONObject*>*) n->getValue()->Value : 0;
	}

	//! Returns the given object idenfified by its key in the JSON file
	JSONObject* getObject(const irr::core::stringc& key)
	{
		JSONObjectKeysType::Node* n = Keys.find(key);
		return n ? (SJSONObjectTree*) n->getValue()->Value : 0;
	}
};

} /// End namespace engine
} /// End namespace engine

#endif
