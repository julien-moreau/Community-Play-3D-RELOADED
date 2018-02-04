#ifndef __C_TOKENIZER_H_INCLUDED__
#define __C_TOKENIZER_H_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace engine {

enum E_TOKEN_TYPE
{
	ETT_UNKNOWN = 0,

	ETT_IDENTIFIER,
	ETT_NUMBER,
	ETT_STRING,

	ETT_ACCESSOR_OPEN,
	ETT_ACCESSOR_CLOSE,

	ETT_BRACKET_OPEN,
	ETT_BRACKET_CLOSE,

	ETT_ASSIGN
};

class CTokenizer
{
public:
	/// ctor and dtor
	CTokenizer(const irr::core::stringc& filePath);
	~CTokenizer();

	//! Returns the next token type
	E_TOKEN_TYPE getNextToken();

	//! Returns if the tokenizer matches an identifier
	bool matchIdentifier(irr::core::stringc *identifier);

	//! Returns if the tokenizer is at the end of file's content
	bool isEnd() { return Pos >= Content.size(); }

	//! Reads the next char and returns its value
	char& read() { return Content[Pos++]; }

	//! Goes to the next char without returning its value
	void forward() { Pos++; }

private:
	/// Methods
	irr::core::stringc readFile(const irr::core::stringc& filePath);

	/// Datas
	irr::u32 Pos;

	irr::core::stringc Content;
	irr::core::stringc CurrentIdentifier;
	irr::core::stringc CurrentString;
	irr::core::stringc CurrentNumber;

	E_TOKEN_TYPE CurrentToken;
};

}
}

#endif
