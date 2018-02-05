#include "Stdafx.h"

#include "CJSONParser.h"
#include "CTokenizer.h"

namespace cp3d {
namespace engine {

using namespace irr;
using namespace core;
using namespace io;

/// Ctor
CJSONParser::CJSONParser(const stringc& fileContent)
{
	Tokenizer = new CTokenizer(fileContent);
}

CJSONParser::CJSONParser(IReadFile* fileToRead)
{
	stringc str;
	c8 c;

	while (fileToRead->read(&c, 1) != 0)
		str += c;

	Tokenizer = new CTokenizer(str);
}

/// Dtor
CJSONParser::~CJSONParser()
{
	delete Tokenizer;
}

//! Parses the JSON file
SJSONObjectTree* CJSONParser::parse(SJSONObjectTree* tree)
{
	// Init
	stringc string;

	if (!tree)
		tree = new SJSONObjectTree();

	bool isKey = true;

	// Parse
	while (!Tokenizer->isEnd())
	{
		// New object
		if (Tokenizer->matchObjectDefinition())
		{
			return parse(tree);
		}
		// End object
		else if (Tokenizer->match(ETT_BRACKET_CLOSE))
		{
			return tree;
		}
		// String (key)
		else if (Tokenizer->matchString(&string))
		{
			if (isKey)
			{
				if (!Tokenizer->matchAssignation())
					return 0;

				tree->Keys[string] = parseValue();

				isKey = false;
			}
		}
		// Comma, next key
		else if (Tokenizer->matchComma())
		{
			isKey = true;
		}
	}

	return tree;
}

//! Parse current value
SJSONObject* CJSONParser::parseValue()
{
	if (Tokenizer->isEnd())
		return 0;

	stringc string;
	stringc identifier;
	stringc number;

	if (Tokenizer->matchString(&string))
		return new SJSONObject(new stringc(string), EJOT_STRING);
	else if (Tokenizer->matchNumber(&number))
		return new SJSONObject(new stringc(number), EJOT_NUMBER);
	else if (Tokenizer->matchIdentifier(&identifier))
		return new SJSONObject(new stringc(identifier), identifier == "false" || identifier == "true" ? EJOT_BOOL : EJOT_NULL);
	else if (Tokenizer->matchArrayDefinition())
		return new SJSONObject(parseArray(), EJOT_ARRAY);

	return 0;
}

//! Parse current array
array<SJSONObject*>* CJSONParser::parseArray()
{
	array<SJSONObject*> *values = new array<SJSONObject*>();

	while (!Tokenizer->isEnd())
	{
		if (Tokenizer->matchComma())
			continue;

		if (Tokenizer->match(ETT_ACCESSOR_CLOSE))
			break;

		values->push_back(parseValue());
	}

	return values;
}

} /// End namespace engine
} /// End namespace cp3d
