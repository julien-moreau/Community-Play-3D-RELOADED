
#ifndef __H_C_CP3D_GEOMETRY_CREATOR_INCLUDED__
#define __H_C_CP3D_GEOMETRY_CREATOR_INCLUDED__

#include <irrlicht.h>
#include <ICP3DGeometryCreator.h>

namespace cp3d {

	namespace rendering {
		class ICP3DRenderingEngine;
	}

namespace engine {

/*
Class that creates geometries
This class works like a factory and make you able to create
geometries quickly.
*/
class CCP3DGeometryCreator : public ICP3DGeometryCreator {
public:

	CCP3DGeometryCreator(irr::scene::ISceneManager *smgr);
	~CCP3DGeometryCreator();

	irr::scene::IMesh *createRingMesh(irr::f32 radius, irr::f32 width, irr::u32 sections, irr::video::SColor color);
	irr::scene::IMesh *createTorusMesh(irr::f32 diameter, irr::f32 thickness, irr::u32 tessellation, irr::video::SColor color);
	irr::scene::IMesh *createMassMesh(irr::f32 size, irr::f32 length, irr::video::SColor color);

private:
	/// Irrlicht
	irr::scene::ISceneManager *Smgr;

};

} /// End namespace engine
} /// End namespace cp3d

#endif
