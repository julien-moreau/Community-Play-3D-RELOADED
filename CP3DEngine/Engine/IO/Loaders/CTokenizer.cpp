#include "Stdafx.h"

#include <cctype>

#include "CTokenizer.h"

namespace cp3d {
namespace engine {

using namespace irr;
using namespace core;
using namespace io;

//! Ctor
//! \param filePath the path to the file to read
CTokenizer::CTokenizer(IReadFile* file)
{
	File = file;
	CurrentToken = getNextToken();
}

//! Dtor
CTokenizer::~CTokenizer()
{ }

//! Peeks the current character
char CTokenizer::peek()
{
	char c = read();
	File->seek(File->getPos() - 1);
	
	return c;
}

//! Reads the character at current position
char CTokenizer::read()
{
	c8 c;
	File->read(&c, sizeof(c8));

	return c;
}

void CTokenizer::forward()
{
	File->seek(File->getPos() + 1);
}

//! Returns if the tokenizer is at end
bool CTokenizer::isEnd()
{
	return File->getPos() >= File->getSize();
}

//! Matches the current token
bool CTokenizer::match(const E_TOKEN_TYPE& token)
{
	if (CurrentToken == token)
	{
		CurrentToken = getNextToken();

		return true;
	}

	return false;
}

//! Returns if the tokenizer matches an identifier
bool CTokenizer::matchIdentifier(stringc *identifier)
{
	if (CurrentToken == ETT_IDENTIFIER)
	{
		*identifier = CurrentIdentifier;
		CurrentToken = getNextToken();

		return true;
	}

	return false;
}

//! Matches a string like a key
bool CTokenizer::matchString(stringc *string)
{
	if (CurrentToken == ETT_STRING)
	{
		*string = CurrentString;
		CurrentToken = getNextToken();

		return true;
	}

	return false;
}

//! Matches a number
bool CTokenizer::matchNumber(stringc* number)
{
	if (CurrentToken == ETT_NUMBER)
	{
		*number = CurrentNumber;
		CurrentToken = getNextToken();

		return true;
	}

	return false;
}

//! Matches an object definition "{"
bool CTokenizer::matchObjectDefinition()
{
	if (CurrentToken == ETT_BRACKET_OPEN)
	{
		CurrentToken = getNextToken();

		return true;
	}

	return false;
}

//! Matches an array definition
bool CTokenizer::matchArrayDefinition()
{
	if (CurrentToken == ETT_ACCESSOR_OPEN)
	{
		CurrentToken = getNextToken();

		return true;
	}

	return false;
}

//! Matches an assignation ":"
bool CTokenizer::matchAssignation()
{
	if (CurrentToken == ETT_ASSIGN)
	{
		CurrentToken = getNextToken();

		return true;
	}

	return false;
}

//! Matches a comma
bool CTokenizer::matchComma()
{
	if (CurrentToken == ETT_COMMA)
	{
		CurrentToken = getNextToken();
		return true;
	}

	return false;
}

//! Returns the next token type
E_TOKEN_TYPE CTokenizer::getNextToken()
{
	// Check if not at end
	if (isEnd())
		return ETT_END_OF_FILE;

	// Bypass new lines and spaces
	char c = read();
	while (c == ' ' || c == '\n' || c == '\r')
		c = read();

	// Check if not at end again
	if (isEnd())
		return ETT_END_OF_FILE;

	// Switch first token's char
	switch (c)
	{
		case '[': return ETT_ACCESSOR_OPEN;
		case ']': return ETT_ACCESSOR_CLOSE;
		case '{': return ETT_BRACKET_OPEN;
		case '}': return ETT_BRACKET_CLOSE;
		case ':': return ETT_ASSIGN;
		case ',': return ETT_COMMA;

		default:
		{
			// Identifier
			if (c == '_' || std::isalpha(c))
			{
				CurrentIdentifier = stringc(c);
				
				while ((c = peek()) == '_' || std::isalpha(c) || std::isdigit(c))
				{
					CurrentIdentifier += c;
					forward();
				}

				return ETT_IDENTIFIER;
			}

			// String
			if (c == '"')
			{
				CurrentString = "";

				while ((c = read()) && c != '"')
					CurrentString += c;

				return ETT_STRING;
			}

			// Number
			if (std::isdigit(c) || c == '-')
			{
				CurrentNumber = stringc(c);

				while ((c = peek()) == '.' || std::isdigit(c) || c == 'e' || c == 'E' || c == '+')
				{
					CurrentNumber += c;
					forward();
				}

				return ETT_NUMBER;
			}
		}
	}

	return ETT_UNKNOWN;
}

} /// End namespace engine
} /// End namespace cp3d
