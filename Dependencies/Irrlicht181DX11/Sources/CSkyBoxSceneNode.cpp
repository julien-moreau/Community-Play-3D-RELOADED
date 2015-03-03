// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CSkyBoxSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "ICameraSceneNode.h"
#include "S3DVertex.h"
#include "os.h"

namespace irr
{
namespace scene
{

//! constructor
CSkyBoxSceneNode::CSkyBoxSceneNode(video::ITexture* top, video::ITexture* bottom, video::ITexture* left,
			video::ITexture* right, video::ITexture* front, video::ITexture* back, ISceneNode* parent, ISceneManager* mgr, s32 id)
: ISceneNode(parent, mgr, id)
{
	#ifdef _DEBUG
	setDebugName("CSkyBoxSceneNode");
	#endif

	for (u32 i = 0; i < 6; ++i)
		MeshBuffer[i] = new CMeshBuffer<video::S3DVertex>(mgr->getVideoDriver()->getVertexDescriptor(0), video::EIT_16BIT);

	setAutomaticCulling(scene::EAC_OFF);

	video::SMaterial mat;
	mat.Lighting = false;
	mat.ZBuffer = video::ECFN_DISABLED;
	mat.ZWriteEnable = false;
	mat.AntiAliasing = 0;
	mat.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
	mat.TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;

	for (u32 i = 0; i < 6; ++i)
	{
		MeshBuffer[i]->getBoundingBox().MaxEdge.set(0, 0, 0);
		MeshBuffer[i]->getBoundingBox().MinEdge.set(0, 0, 0);

		IIndexBuffer* IndexBuffer = MeshBuffer[i]->getIndexBuffer();

		IndexBuffer->addIndex(3);
		IndexBuffer->addIndex(2);
		IndexBuffer->addIndex(0);
		IndexBuffer->addIndex(1);
		IndexBuffer->addIndex(3);
		IndexBuffer->addIndex(0);

		MeshBuffer[i]->getMaterial() = mat;
	}

	/* Hey, I am no artist, but look at that
	   cool ASCII art I made! ;)

       -111         111
          /6--------/5        y
         /  |      / |        ^  z
        /   |   11-1 |        | /
  -11-1 3---------2  |        |/
        |   7- - -| -4 1-11    *---->x
        | -1-11   |  /       3-------|2
        |/        | /         |    //|
        0---------1/          |  //  |
     -1-1-1     1-1-1         |//    |
	                     0--------1
	*/

	video::ITexture* tex = front;
	if (!tex) tex = left;
	if (!tex) tex = back;
	if (!tex) tex = right;
	if (!tex) tex = top;
	if (!tex) tex = bottom;

	const f32 onepixel = tex?(1.0f / (tex->getSize().Width * 1.5f)) : 0.0f;
	const f32 t = 1.0f - onepixel;
	const f32 o = 0.0f + onepixel;

	// create front side

	MeshBuffer[0]->getMaterial().setTexture(0, front);

	IVertexBuffer* VertexBuffer = MeshBuffer[0]->getVertexBuffer(0);

	video::S3DVertex Vertex = video::S3DVertex(-1,-1,-1, 0,0,1, video::SColor(255,255,255,255), t, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1,-1,-1, 0,0,1, video::SColor(255,255,255,255), o, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1,-1, 0,0,1, video::SColor(255,255,255,255), t, o);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1,-1, 0,0,1, video::SColor(255,255,255,255), o, o);
	VertexBuffer->addVertex(&Vertex);

	// create left side

	MeshBuffer[1]->getMaterial().setTexture(0, left);

	VertexBuffer = MeshBuffer[1]->getVertexBuffer(0);

	Vertex = video::S3DVertex( 1,-1,-1, -1,0,0, video::SColor(255,255,255,255), t, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1,-1, 1, -1,0,0, video::SColor(255,255,255,255), o, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1,-1, -1,0,0, video::SColor(255,255,255,255), t, o);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1, 1, -1,0,0, video::SColor(255,255,255,255), o, o);
	VertexBuffer->addVertex(&Vertex);

	// create back side

	MeshBuffer[2]->getMaterial().setTexture(0, back);

	VertexBuffer = MeshBuffer[2]->getVertexBuffer(0);

	Vertex = video::S3DVertex( 1,-1, 1, 0,0,-1, video::SColor(255,255,255,255), t, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1,-1, 1, 0,0,-1, video::SColor(255,255,255,255), o, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1, 1, 0,0,-1, video::SColor(255,255,255,255), t, o);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1, 1, 0,0,-1, video::SColor(255,255,255,255), o, o);
    VertexBuffer->addVertex(&Vertex);

	// create right side

	MeshBuffer[3]->getMaterial().setTexture(0, right);

	VertexBuffer = MeshBuffer[3]->getVertexBuffer(0);

	Vertex = video::S3DVertex(-1,-1, 1, 1,0,0, video::SColor(255,255,255,255), t, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1,-1,-1, 1,0,0, video::SColor(255,255,255,255), o, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1, 1, 1,0,0, video::SColor(255,255,255,255), t, o);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1,-1, 1,0,0, video::SColor(255,255,255,255), o, o);
    VertexBuffer->addVertex(&Vertex);

	// create top side

	MeshBuffer[4]->getMaterial().setTexture(0, top);

	VertexBuffer = MeshBuffer[4]->getVertexBuffer(0);

	Vertex = video::S3DVertex( 1, 1,-1, 0,-1,0, video::SColor(255,255,255,255), t, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1, 1, 0,-1,0, video::SColor(255,255,255,255), o, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1,-1, 0,-1,0, video::SColor(255,255,255,255), t, o);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1, 1, 0,-1,0, video::SColor(255,255,255,255), o, o);
    VertexBuffer->addVertex(&Vertex);

	// create bottom side

	MeshBuffer[5]->getMaterial().setTexture(0, bottom);

	VertexBuffer = MeshBuffer[5]->getVertexBuffer(0);

	Vertex = video::S3DVertex( 1,-1, 1, 0,1,0, video::SColor(255,255,255,255), o, o);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1,-1,-1, 0,1,0, video::SColor(255,255,255,255), t, o);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1,-1, 1, 0,1,0, video::SColor(255,255,255,255), o, t);
	VertexBuffer->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1,-1,-1, 0,1,0, video::SColor(255,255,255,255), t, t);
	VertexBuffer->addVertex(&Vertex);
}


//! destructor
CSkyBoxSceneNode::~CSkyBoxSceneNode()
{
    for (u32 i = 0; i < 6; ++i)
		MeshBuffer[i]->drop();
}


//! renders the node.
void CSkyBoxSceneNode::render()
{
	video::IVideoDriver* driver = SceneManager->getVideoDriver();
	scene::ICameraSceneNode* camera = SceneManager->getActiveCamera();

	if (!camera || !driver)
		return;

	if ( !camera->isOrthogonal() )
	{
		// draw perspective skybox

		core::matrix4 translate(AbsoluteTransformation);
		translate.setTranslation(camera->getAbsolutePosition());

		// Draw the sky box between the near and far clip plane
		const f32 viewDistance = (camera->getNearValue() + camera->getFarValue()) * 0.5f;
		core::matrix4 scale;
		scale.setScale(core::vector3df(viewDistance, viewDistance, viewDistance));

		driver->setTransform(video::ETS_WORLD, translate * scale);

		for (s32 i=0; i<6; ++i)
		{
			driver->setMaterial(MeshBuffer[i]->getMaterial());
			driver->drawMeshBuffer(MeshBuffer[i]);
		}
	}
	else
	{
		// draw orthogonal skybox,
		// simply choose one texture and draw it as 2d picture.
		// there could be better ways to do this, but currently I think this is ok.

		core::vector3df lookVect = camera->getTarget() - camera->getAbsolutePosition();
		lookVect.normalize();
		core::vector3df absVect( core::abs_(lookVect.X),
					 core::abs_(lookVect.Y),
					 core::abs_(lookVect.Z));

		int idx = 0;

		if ( absVect.X >= absVect.Y && absVect.X >= absVect.Z )
		{
			// x direction
			idx = lookVect.X > 0 ? 0 : 2;
		}
		else
		if ( absVect.Y >= absVect.X && absVect.Y >= absVect.Z )
		{
			// y direction
			idx = lookVect.Y > 0 ? 4 : 5;
		}
		else
		if ( absVect.Z >= absVect.X && absVect.Z >= absVect.Y )
		{
			// z direction
			idx = lookVect.Z > 0 ? 1 : 3;
		}

		video::ITexture* tex = MeshBuffer[idx]->getMaterial().getTexture(0);

		if ( tex )
		{
			core::rect<s32> rctDest(core::position2d<s32>(-1,0),
									core::dimension2di(driver->getCurrentRenderTargetSize()));
			core::rect<s32> rctSrc(core::position2d<s32>(0,0),
									core::dimension2di(tex->getSize()));

			driver->draw2DImage(tex, rctDest, rctSrc);
		}
	}
}



//! returns the axis aligned bounding box of this node
const core::aabbox3d<f32>& CSkyBoxSceneNode::getBoundingBox() const
{
	return MeshBuffer[0]->getBoundingBox();
}


void CSkyBoxSceneNode::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this, ESNRP_SKY_BOX);

	ISceneNode::OnRegisterSceneNode();
}


//! returns the material based on the zero based index i. To get the amount
//! of materials used by this scene node, use getMaterialCount().
//! This function is needed for inserting the node into the scene hirachy on a
//! optimal position for minimizing renderstate changes, but can also be used
//! to directly modify the material of a scene node.
video::SMaterial& CSkyBoxSceneNode::getMaterial(u32 i)
{
	return MeshBuffer[i]->getMaterial();
}


//! returns amount of materials used by this scene node.
u32 CSkyBoxSceneNode::getMaterialCount() const
{
	return 6;
}


//! Creates a clone of this scene node and its children.
ISceneNode* CSkyBoxSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
{
	if (!newParent) newParent = Parent;
	if (!newManager) newManager = SceneManager;

	CSkyBoxSceneNode* nb = new CSkyBoxSceneNode(0,0,0,0,0,0, newParent,
		newManager, ID);

	nb->cloneMembers(this, newManager);

	for (u32 i = 0; i < 6; ++i)
	{
		nb->MeshBuffer[i]->getMaterial() = MeshBuffer[i]->getMaterial();
		nb->MeshBuffer[i]->getBoundingBox() = MeshBuffer[i]->getBoundingBox();
	}

	if ( newParent )
		nb->drop();
	return nb;
}


} // end namespace scene
} // end namespace irr

