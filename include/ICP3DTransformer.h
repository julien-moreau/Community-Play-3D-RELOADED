#ifndef _H_ICP3D_TRANSFORMER_INCLUDED__
#define _H_ICP3D_TRANSFORMER_INCLUDED__

#include <irrlicht.h>

namespace cp3d {

enum E_TRANSFORMER_TYPE {
	ETT_POSITION = 0,
	ETT_ROTATION,
	ETT_SCALE,
	ETT_FREE,

	ETT_NONE
};

//! Handles the transformer's class (position, rotation, scale, free)
class ICP3DTransformer {
public:

	//! Sets the new transformer type
	//! \param type: the new type (position, rotation, scale, free)
	virtual void setTransformerType(E_TRANSFORMER_TYPE type) = 0;

};

} /// End cp3d namespace

#endif
