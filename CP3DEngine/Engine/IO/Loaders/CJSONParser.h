#ifndef __C_JSON_PARSER_H_INCLUDED__
#define __C_JSON_PARSER_H_INCLUDED__

#include <irrlicht.h>

#include "SJSONObject.h"

namespace cp3d {
namespace engine {

class CTokenizer;

//! JSON parser class
class CJSONParser
{
public:
	/// ctor and dtor
	CJSONParser(irr::io::IReadFile* fileToRead, irr::ILogger* logger);

	~CJSONParser();

	//! Parses the JSON file
	SJSONObjectTree* parse();

private:
	/// Datas
	irr::ILogger* Logger;
	CTokenizer* Tokenizer;

	/// Methods
	SJSONObject* parseValue();
	irr::core::array<SJSONObject*>* parseArray();
};

} /// End namespace engine
} /// End namespace engine

#endif
