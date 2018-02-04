#include "Stdafx.h"

#include <fstream>
#include <string>
#include <iostream>
#include <cctype>

#include "CTokenizer.h"

namespace cp3d {
namespace engine {

using namespace irr;
using namespace core;

//! Ctor
//! \param filePath the path to the file to read
CTokenizer::CTokenizer(const stringc& filePath) : Pos(0)
{
	Content = readFile(filePath);
	CurrentToken = getNextToken();
}

//! Dtor
CTokenizer::~CTokenizer()
{ }

//! Returns if the tokenizer matches an identifier
bool CTokenizer::matchIdentifier(irr::core::stringc *identifier)
{
	if (CurrentToken == ETT_IDENTIFIER)
	{
		*identifier = CurrentIdentifier;
		getNextToken();

		return true;
	}

	return false;
}

//! Returns the next token type
E_TOKEN_TYPE CTokenizer::getNextToken()
{
	if (isEnd())
		return ETT_UNKNOWN;

	// Bypass new lines and spaces
	char c = read();
	while (c == ' ' || c == '\n' || c == '\r')
		c = read();

	// Switch first token's char
	switch (c)
	{
		case '[': return ETT_ACCESSOR_OPEN;
		case ']': return ETT_ACCESSOR_CLOSE;
		case '{': return ETT_BRACKET_OPEN;
		case '}': return ETT_BRACKET_CLOSE;
		case ':': return ETT_ASSIGN;

		default:
		{
			// Identifier
			if (c == '_' || std::isalpha(c))
			{
				CurrentIdentifier += c;
				
				while ((c = read()) == '_' || std::isalpha(c) || std::isdigit(c))
					CurrentIdentifier += c;

				return ETT_IDENTIFIER;
			}

			// String
			if (c == '"')
			{
				CurrentString += c;
				while ((c = read()) && c != '"')
					CurrentString += c;

				return ETT_STRING;
			}

			// Number
			if (std::isdigit(c))
			{
				CurrentNumber += c;

				while ((c = read()) && (std::isdigit(c) || c == '.'))
					CurrentNumber += c;

				return ETT_NUMBER;
			}
		}
	}

	return ETT_UNKNOWN;
}

//! Reads the given file and returns its string
stringc CTokenizer::readFile(const stringc& filePath)
{
	std::ifstream file(filePath.c_str(), std::ios::in);
	std::string content;
	
	if (file.is_open()) {
		for (std::string line; std::getline(file, line);)
			content += line + "\n";

		file.close();
	}

	return content.c_str();
}

} /// End namespace engine
} /// End namespace cp3d
