#ifndef _H_ICP3D_GEOMETRY_CREATOR_INCLUDED__
#define _H_ICP3D_GEOMETRY_CREATOR_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace engine {

class ICP3DGeometryCreator {
public:

	//! Creates a ring mesh
	//! \param radius: the ring's radius
	//! \param width: the ring's width
	//! \param sections: number of sections for the ring
	virtual irr::scene::IMesh *createRingMesh(irr::f32 radius, irr::f32 width, irr::u32 sections, irr::video::SColor color) = 0;

	virtual irr::scene::IMesh *createTorusMesh(irr::f32 diameter, irr::f32 thickness, irr::u32 tessellation, irr::video::SColor color) = 0;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
