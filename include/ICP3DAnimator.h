#ifndef _H_ICP3D_ANIMATOR_INCLUDED__
#define _H_ICP3D_ANIMATOR_INCLUDED__

#include <irrlicht.h>
#include <functional>

namespace cp3d {
namespace rendering {
	template<class T>
	class ICP3DAnimator : public irr::scene::ISceneNodeAnimator {
	public:
		//! Constructor
		ICP3DAnimator()
		{

		}

		//! animates a scene node
		virtual void animateNode(irr::scene::ISceneNode* node, irr::u32 timeMs) {
			LastTime = timeMs;
		}

		//! Writes attributes of the scene node animator.
		virtual void serializeAttributes(irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options = 0) const {
			/// Empty
		}

		//! Reads attributes of the scene node animator.
		virtual void deserializeAttributes(irr::io::IAttributes* in, irr::io::SAttributeReadWriteOptions* options = 0) {
			/// Empty
		}

		//! Returns type of the scene node animator
		virtual irr::scene::ESCENE_NODE_ANIMATOR_TYPE getType() const { return irr::scene::ESNAT_UNKNOWN; }

		//! Creates a clone of this animator.
		/** Please note that you will have to drop
		(IReferenceCounted::drop()) the returned pointer after calling
		this. */
		virtual ISceneNodeAnimator* createClone(irr::scene::ISceneNode* node, irr::scene::ISceneManager* newManager = 0) {
			return 0;
		}

	protected:
		/// The pointer to the data
		T Data;

		/// Animator datas
		irr::u32 LastTime;
	};

}
}

#endif
