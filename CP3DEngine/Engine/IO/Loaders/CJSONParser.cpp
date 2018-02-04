#include "Stdafx.h"

#include "CJSONParser.h"
#include "CTokenizer.h"

namespace cp3d {
namespace engine {

using namespace irr;
using namespace core;

/// Ctor
CJSONParser::CJSONParser(const stringc& filePath) : FilePath(filePath)
{
	Tokenizer = new CTokenizer(filePath);
}

/// Dtor
CJSONParser::~CJSONParser()
{
	delete Tokenizer;
}

} /// End namespace engine
} /// End namespace cp3d