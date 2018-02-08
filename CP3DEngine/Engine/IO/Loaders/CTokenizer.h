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

	ETT_ASSIGN,
	ETT_COMMA,

	ETT_END_OF_FILE
};

class CTokenizer
{
public:
	/// ctor and dtor
	CTokenizer(irr::io::IReadFile *file);
	~CTokenizer();

	//! Returns the next token type
	E_TOKEN_TYPE getNextToken();

	//! Matches the current token
	bool match(const E_TOKEN_TYPE& token);

	//! Returns if the tokenizer matches an identifier
	bool matchIdentifier(irr::core::stringc *identifier);

	//! Matches a string like a key
	bool matchString(irr::core::stringc *string);

	//! Matches a number
	bool matchNumber(irr::core::stringc* number);

	//! Matches an object definition "{"
	bool matchObjectDefinition();

	//! Matches an array definition
	bool matchArrayDefinition();

	//! Matches an assignation ":"
	bool matchAssignation();

	//! Matches a comma
	bool matchComma();

	//! Returns if the tokenizer is at the end of file's content
	bool isEnd();

	//! Reads the next char and returns its value
	char read();

	//! Goes to the next char without returning its value
	void forward();

	//! Peeks the current char
	char peek();

private:
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
