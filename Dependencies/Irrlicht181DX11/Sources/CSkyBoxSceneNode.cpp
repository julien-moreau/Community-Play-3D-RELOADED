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

	setAutomaticCulling(scene::EAC_OFF);
	Box.MaxEdge.set(0,0,0);
	Box.MinEdge.set(0,0,0);

    for(u32 i = 0; i < 6; ++i)
        VertexBuffer[i] = new CVertexBuffer<video::S3DVertex>(mgr->getVideoDriver()->getVertexDescriptor(0));

	IndexBuffer = new CIndexBuffer(video::EIT_16BIT);

	// create indices

	IndexBuffer->addIndex(0);
	IndexBuffer->addIndex(1);
	IndexBuffer->addIndex(2);
	IndexBuffer->addIndex(3);

	// create material

	video::SMaterial mat;
	mat.Lighting = false;
	mat.ZBuffer = video::ECFN_DISABLED;
	mat.ZWriteEnable = false;
	mat.AntiAliasing=0;
	mat.TextureLayer[0].TextureWrapU = video::ETC_CLAMP_TO_EDGE;
	mat.TextureLayer[0].TextureWrapV = video::ETC_CLAMP_TO_EDGE;

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

	Material[0] = mat;
	Material[0].setTexture(0, front);

	video::S3DVertex Vertex = video::S3DVertex(-1,-1,-1, 0,0,1, video::SColor(255,255,255,255), t, t);
	VertexBuffer[0]->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1,-1,-1, 0,0,1, video::SColor(255,255,255,255), o, t);
	VertexBuffer[0]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1,-1, 0,0,1, video::SColor(255,255,255,255), t, o);
	VertexBuffer[0]->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1,-1, 0,0,1, video::SColor(255,255,255,255), o, o);
	VertexBuffer[0]->addVertex(&Vertex);

	// create left side

	Material[1] = mat;
	Material[1].setTexture(0, left);

	Vertex = video::S3DVertex( 1,-1,-1, -1,0,0, video::SColor(255,255,255,255), t, t);
	VertexBuffer[1]->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1,-1, 1, -1,0,0, video::SColor(255,255,255,255), o, t);
	VertexBuffer[1]->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1,-1, -1,0,0, video::SColor(255,255,255,255), t, o);
	VertexBuffer[1]->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1, 1, -1,0,0, video::SColor(255,255,255,255), o, o);
	VertexBuffer[1]->addVertex(&Vertex);

	// create back side

	Material[2] = mat;
	Material[2].setTexture(0, back);

	Vertex = video::S3DVertex( 1,-1, 1, 0,0,-1, video::SColor(255,255,255,255), t, t);
	VertexBuffer[2]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1,-1, 1, 0,0,-1, video::SColor(255,255,255,255), o, t);
	VertexBuffer[2]->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1, 1, 0,0,-1, video::SColor(255,255,255,255), t, o);
	VertexBuffer[2]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1, 1, 0,0,-1, video::SColor(255,255,255,255), o, o);
    VertexBuffer[2]->addVertex(&Vertex);

	// create right side

	Material[3] = mat;
	Material[3].setTexture(0, right);

	Vertex = video::S3DVertex(-1,-1, 1, 1,0,0, video::SColor(255,255,255,255), t, t);
	VertexBuffer[3]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1,-1,-1, 1,0,0, video::SColor(255,255,255,255), o, t);
	VertexBuffer[3]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1, 1, 1,0,0, video::SColor(255,255,255,255), t, o);
	VertexBuffer[3]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1,-1, 1,0,0, video::SColor(255,255,255,255), o, o);
    VertexBuffer[3]->addVertex(&Vertex);

	// create top side

	Material[4] = mat;
	Material[4].setTexture(0, top);

	Vertex = video::S3DVertex( 1, 1,-1, 0,-1,0, video::SColor(255,255,255,255), t, t);
	VertexBuffer[4]->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1, 1, 1, 0,-1,0, video::SColor(255,255,255,255), o, t);
	VertexBuffer[4]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1,-1, 0,-1,0, video::SColor(255,255,255,255), t, o);
	VertexBuffer[4]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1, 1, 1, 0,-1,0, video::SColor(255,255,255,255), o, o);
    VertexBuffer[4]->addVertex(&Vertex);

	// create bottom side

	Material[5] = mat;
	Material[5].setTexture(0, bottom);

	Vertex = video::S3DVertex( 1,-1, 1, 0,1,0, video::SColor(255,255,255,255), o, o);
	VertexBuffer[5]->addVertex(&Vertex);

	Vertex = video::S3DVertex( 1,-1,-1, 0,1,0, video::SColor(255,255,255,255), t, o);
	VertexBuffer[5]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1,-1, 1, 0,1,0, video::SColor(255,255,255,255), o, t);
	VertexBuffer[5]->addVertex(&Vertex);

	Vertex = video::S3DVertex(-1,-1,-1, 0,1,0, video::SColor(255,255,255,255), t, t);
	VertexBuffer[5]->addVertex(&Vertex);
}


//! destructor
CSkyBoxSceneNode::~CSkyBoxSceneNode()
{
    for(u32 i = 0; i < 6; ++i)
        VertexBuffer[i]->drop();

    IndexBuffer->drop();
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
			driver->setMaterial(Material[i]);
			driver->drawVertexPrimitiveList(false, VertexBuffer[i], false, IndexBuffer, 2, EPT_TRIANGLE_STRIP);
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

		video::ITexture* tex = Material[idx].getTexture(0);

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
	return Box;
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
	return Material[i];
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

	for (u32 i=0; i<6; ++i)
		nb->Material[i] = Material[i];

	if ( newParent )
		nb->drop();
	return nb;
}


} // end namespace scene
} // end namespace irr

