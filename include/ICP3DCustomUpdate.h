#ifndef _H_ICP3D_CUSTOM_UPDATE_INCLUDED__
#define _H_ICP3D_CUSTOM_UPDATE_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace engine {

class ICP3DUpdate {
public:

	//! Called by the custom updater.
	//! This function is called each frame before rendering to update
	//! some specific things you'll handle in your programs
	virtual void OnPreUpdate() { }

	//! Called by the custom updater.
	//! This function is called each frame after rendering to update
	//! some specific things you'll handle in your programs
	virtual void OnPostUpdate() { }

};

class ICP3DCustomUpdater : public ICP3DUpdate {
public:

	//! Adds a custom update
	//! \param update: the instance of the custom update
	/* Example :
	
	class MyClass : public cp3d::engine::ICP3DUpdate {
	public:

		MyClass() { }
		~MyClass() { }

		void OnPreUpdate() {
			/// Update what you want here like :
			if (Device->getTimer()->getTime() % 2 == 0)
				std::cout << Device->getTimer()->getTime() << std::endl;
			/// etc.
		}

	};

	*/
	virtual bool addCustomUpdate(ICP3DUpdate *update) = 0;

	//! Removes a custom update
	//! \param update: the update to remove
	virtual bool removeCustomUpdate(ICP3DUpdate *update) = 0;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
