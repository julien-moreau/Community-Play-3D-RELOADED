
#ifndef __H_C_CP3D_EXPORTER_INCLUDED__
#define __H_C_CP3D_EXPORTER_INCLUDED__

#include <ICP3DExporter.h>

namespace cp3d {
namespace engine {

class CCP3DEngine;

class CCP3DExporter : public ICP3DExporter {
public:

	/// ctor & dtor
	CCP3DExporter(CCP3DEngine *engine);
	~CCP3DExporter();

	/// ICP3DExporter
	bool exportProject(irr::core::stringc filename);
	bool importProject(irr::core::stringc filename);

	bool addAttributes(irr::core::stringc name, irr::io::IAttributes *attributes);
	bool removeAttributes(irr::io::IAttributes *attributes);
	bool removeAttributes(irr::core::stringc name);

private:

	/// Structs
	struct SAttribute {
		irr::core::stringc Name;
		irr::io::IAttributes *Attributes;
	};
	irr::core::array<SAttribute> Attributes;

	/// Methods
	irr::core::stringw getValue(irr::io::IAttributes *attributes, irr::u32 indice);
	void setAttribute(irr::io::IAttributes *attributes, irr::core::stringw type);

	void writeAttributes(SAttribute attributes);
	void exportScene();

	void importNode();
	void importScene();

	/// Datas
	CCP3DEngine *Engine;
	irr::IrrlichtDevice *Device;
	irr::io::IXMLWriter *Writer;
	irr::io::IXMLReader *Reader;
};

} /// End namespace engine
} /// End namespace cp3d

#endif
