#ifndef _H_ICP3D_EXPORTER_INCLUDED__
#define _H_ICP3D_EXPORTER_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace engine {

	class ICP3DExporter {
	public:

		//! Exports the project into a file
		//! \param smgr: the scene to export
		//! \return: if the export succeded or not
		virtual bool exportProject(irr::core::stringc filename) = 0;

		//! Imports the given file
		//! \param filename: the file to import
		virtual bool importProject(irr::core::stringc filename) = 0;

		//! Adds a IAttributes object to export
		//! \param attributes: attributes to add and to export
		//! \return: if the attributes were added (true) or already exist (false)
		virtual bool addAttributes(irr::core::stringc name, irr::io::IAttributes *attributes) = 0;

		//! Removes the given attributes from the export
		//! \param attributes: attributes to remove
		//! \return: if the attributes object was removed
		virtual bool removeAttributes(irr::io::IAttributes *attributes) = 0;

		// !Removes the given attributes's name from the export
		//! \param name: the name of the attributes to remove
		//! \return: if the attributes object was removed
		virtual bool removeAttributes(irr::core::stringc name) = 0;

	};

} /// End namespace rendering
} /// End namespace cp3d

#endif
