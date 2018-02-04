#ifndef __C_JSON_PARSER_H_INCLUDED__
#define __C_JSON_PARSER_H_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace engine {

class CTokenizer;

class CJSONParser
{
public:
	CJSONParser(const irr::core::stringc& filePath);
	~CJSONParser();

private:
	/// Datas
	CTokenizer* Tokenizer;
	irr::core::stringc FilePath;
};

}
}

#endif
