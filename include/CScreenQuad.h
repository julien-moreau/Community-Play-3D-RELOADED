#ifndef __H_CP3D_SCREEN_QUAD_INCLUDED__
#define __H_CP3D_SCREEN_QUAD_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class CScreenQuad {

public:

	CScreenQuad() {
		Material.Wireframe = false;
		Material.Lighting = false;
		Material.ZWriteEnable = false;

		Vertices[0] = irr::video::S3DVertex(-1.0f, -1.0f, 0.0f, 0, 0, 1, irr::video::SColor(0x0), 0.0f, 1.0f);
		Vertices[1] = irr::video::S3DVertex(-1.0f, 1.0f, 0.0f, 0, 0, 1, irr::video::SColor(0x0), 0.0f, 0.0f);
		Vertices[2] = irr::video::S3DVertex(1.0f, 1.0f, 0.0f, 0, 0, 1, irr::video::SColor(0x0), 1.0f, 0.0f);
		Vertices[3] = irr::video::S3DVertex(1.0f, -1.0f, 0.0f, 0, 0, 1, irr::video::SColor(0x0), 1.0f, 1.0f);
	}

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	~CScreenQuad() {
		Buffer->drop();
	}

	virtual void initializeD3D11(irr::video::IVideoDriver *driver) {
		irr::video::IVertexDescriptor *vertexDescriptor = driver->getVertexDescriptor(0);
		Buffer = new irr::scene::CMeshBuffer<irr::video::S3DVertex>(vertexDescriptor, irr::video::EIT_16BIT);

		for (irr::u32 i = 0; i < 4; i++)
			Buffer->getVertexBuffer(0)->addVertex(&Vertices[i]);

		const irr::u16 indices[6] = { 0, 1, 2, 0, 2, 3 };
		for (irr::u32 i = 0; i < 6; i++)
			Buffer->getIndexBuffer()->addIndex(indices[i]);
	}
	#endif

	virtual void render(irr::video::IVideoDriver* driver) {

		driver->setMaterial(Material);
		driver->setTransform(irr::video::ETS_WORLD, irr::core::matrix4());

		#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
		driver->drawMeshBuffer(Buffer);
		#else
		const irr::u16 indices[6] = { 0, 1, 2, 0, 2, 3 };
		driver->drawIndexedTriangleList(&Vertices[0], 4, &indices[0], 2);
		#endif
	}

	virtual irr::video::SMaterial& getMaterial() {
		return Material;
	}

	irr::video::ITexture* rt[2];

private:

	irr::video::S3DVertex Vertices[4];
	irr::video::SMaterial Material;

	#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
	irr::scene::CMeshBuffer<irr::video::S3DVertex> *Buffer;
	#endif

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
