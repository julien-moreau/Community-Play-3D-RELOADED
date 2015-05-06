
#include "stdafx.h"
#include "CCP3DExporter.h"

#include "../../CP3DEngine.h"

using namespace irr;
using namespace core;
using namespace io;
using namespace video;

namespace cp3d {
namespace engine {

CCP3DExporter::CCP3DExporter(CCP3DEngine *engine) : Engine(engine) {

}

CCP3DExporter::~CCP3DExporter() {

}

bool CCP3DExporter::addAttributes(stringc name, IAttributes *attributes) {
	for (u32 i = 0; i < Attributes.size(); i++) {
		if (Attributes[i].Attributes == attributes || Attributes[i].Name == name) {
			return false;
		}
	}

	Attributes.push_back({ name, attributes });
	return true;
}

bool CCP3DExporter::removeAttributes(irr::io::IAttributes *attributes) {
	for (u32 i = 0; i < Attributes.size(); i++) {
		if (Attributes[i].Attributes == attributes) {
			Attributes.erase(i);
			return true;
		}
	}

	return false;
}

bool CCP3DExporter::removeAttributes(stringc name) {
	for (u32 i = 0; i < Attributes.size(); i++) {
		if (Attributes[i].Name == name) {
			Attributes.erase(i);
			return true;
		}
	}

	return false;
}

stringw CCP3DExporter::getValue(IAttributes *attributes, u32 indice) {
	E_ATTRIBUTE_TYPE type = attributes->getAttributeType(indice);

	switch (type) {
		case EAT_INT:
			return stringw(attributes->getAttributeAsInt(indice));
			break;
		case EAT_FLOAT:
			return stringw(attributes->getAttributeAsFloat(indice));
			break;
		case EAT_STRING:
			return attributes->getAttributeAsStringW(indice);
			break;
		case EAT_BOOL: {
			bool value = attributes->getAttributeAsBool(indice);
			return (value == false) ? "false" : "true";
		}
			break;
		case EAT_ENUM:
			break;
		case EAT_COLOR: {
			SColor color = attributes->getAttributeAsColor(indice);
			return stringw(color.getRed()) + "," + stringw(color.getGreen()) + "," + stringw(color.getBlue());
		}
			break;
		case EAT_COLORF: {
			SColorf colorf = attributes->getAttributeAsColorf(indice);
			return stringw(colorf.r) + ", " + stringw(colorf.g) + ", " + stringw(colorf.b);
		}
			break;
		case EAT_VECTOR3D: {
			vector3df v3d = attributes->getAttributeAsVector3d(indice);
			return stringw(v3d.X) + ", " + stringw(v3d.Y) + ", " + stringw(v3d.Z);
		}
			break;
		case EAT_POSITION2D: {
			position2di p2d = attributes->getAttributeAsPosition2d(indice);
			return stringw(p2d.X) + ", " + stringw(p2d.Y);
		}
			break;
		case EAT_VECTOR2D: {
			vector2df v2d = attributes->getAttributeAsVector2d(indice);
			return stringw(v2d.X) + ", " + stringw(v2d.Y);
		}
			break;
		case EAT_RECT: {
			rect<s32> r = attributes->getAttributeAsRect(indice);
			return stringw(r.UpperLeftCorner.X) + ", " + stringw(r.UpperLeftCorner.Y) + ", " + stringw(r.LowerRightCorner.X) + ", " + stringw(r.LowerRightCorner.Y);
		}
			break;
		case EAT_MATRIX: {
			matrix4 mat = attributes->getAttributeAsMatrix(indice);
			stringw matW = "";
			for (u32 i = 0; i < 16; i++) {
				matW += stringw(mat.pointer()[i]) + ", ";
			}
			return matW;
		}
			break;
		case irr::io::EAT_QUATERNION:
			break;
		case irr::io::EAT_BBOX:
			break;
		case irr::io::EAT_PLANE:
			break;
		case irr::io::EAT_TRIANGLE3D:
			break;
		case irr::io::EAT_LINE2D:
			break;
		case irr::io::EAT_LINE3D:
			break;
		case irr::io::EAT_STRINGWARRAY:
			break;

		case EAT_INTARRAY:
		case EAT_FLOATARRAY: {
			array<stringw> array = attributes->getAttributeAsArray(indice);
			stringw arrayW = "";
			for (u32 i = 0; i < array.size(); i++) {
				arrayW += array[i] + ", ";
			}
		}
			break;
		case irr::io::EAT_BINARY:
			break;
		case EAT_TEXTURE:
			return "texture";
			break;
		case irr::io::EAT_USER_POINTER:
			break;
		case EAT_DIMENSION2D: {
			dimension2du dim = attributes->getAttributeAsDimension2d(indice);
			return stringw(dim.Width) + ", " + stringw(dim.Height);
		}
			break;
		case EAT_COUNT:
			break;
		case EAT_UNKNOWN:
			break;
		default: break;
	}

	return L"";
}

bool CCP3DExporter::exportProject(stringc filename) {
	IrrlichtDevice *device = Engine->getRenderingEngine()->getDevice();
	IXMLWriter *writer = device->getFileSystem()->createXMLWriter(filename);

	/// Write header
	writer->writeXMLHeader();

	/// Write external attributes
	for (u32 i = 0; i < Attributes.size(); i++) {
		IAttributes *attr = Attributes[i].Attributes;
		stringw name = Attributes[i].Name;

		writer->writeElement(name.c_str());
		writer->writeLineBreak();

		for (u32 j = 0; j < attr->getAttributeCount(); j++) {
			stringw attrName = attr->getAttributeName(i);

			writer->writeElement(attrName.c_str(), true, L"type", attr->getAttributeTypeString(j), L"value", getValue(attr, j).c_str());
			writer->writeLineBreak();
		}

		writer->writeClosingTag(name.c_str());
		writer->writeLineBreak();
	}

	/// Close XML writer
	writer->drop();

	return true;
}

} /// End namespace engine
} /// End namespace cp3d
