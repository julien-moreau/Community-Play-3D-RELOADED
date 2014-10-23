#ifndef __H_C_CLOUD_SCENE_NODE_INCLUDED__
#define __H_C_CLOUD_SCENE_NODE_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace engine {

#define CLOUDSUBDIV 16

class CCloudSceneNode : public irr::scene::ISceneNode {
public:

	/// Constructor
	CCloudSceneNode(irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id = -1);

protected:
	/// Vertices and indices
	irr::video::S3DVertex InnerVertices[CLOUDSUBDIV+1];
	irr::u16 InnerIndices[CLOUDSUBDIV+2];
	irr::video::S3DVertex OuterVertices[2*CLOUDSUBDIV];
	irr::u16 OuterIndices[2*CLOUDSUBDIV+2];

	/// Scale of the cloud
	irr::f32 TextureScale;

	/// Inner and outer radius
	irr::f32 InnerRadius;
	irr::f32 OuterRadius;

	/// Height of the cloud (center, inner and outer)
	irr::f32 CenterHeight;
	irr::f32 InnerHeight;
	irr::f32 OuterHeight;

	/// color of the cloud
	irr::video::SColor CenterColor;
	irr::video::SColor InnerColor;
	irr::video::SColor OuterColor;

	/// Last update of the node
	irr::u32 LastUpdate;

	/// Cloud translation
	irr::core::vector2d<irr::f32> Translation;
	/// Current translation (next translation step = current + elapsed time * Translation)
	irr::core::vector2d<irr::f32> CurrentTranslation;
	/// Bounding box
	irr::core::aabbox3d<irr::f32> BBox;
	/// Material of the node
	irr::video::SMaterial Material;

	/// Creates the vertices and indices used by the node
	void createCloudLayer();
		
public:

	/// Typical OnRegisterSceneNode function
	virtual void OnRegisterSceneNode();
	/// Renders the node
	virtual void render();
	/// Returns the bounding box
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
	/// Returns the node type
	virtual irr::scene::ESCENE_NODE_TYPE getType() const;
	/// Returns the material count
	virtual irr::u32 getMaterialCount() const;
	/// Returns the material
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);
	/// Clones the node
	virtual ISceneNode* clone(irr::scene::ISceneNode* newParent = 0, irr::scene::ISceneManager* newManager = 0); 
	/// Animates the node
	virtual void OnAnimate(irr::u32 timeMs);
		
	/// Writes attributes of the scene node.
	virtual void serializeAttributes(irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options = 0) const;

	/// Reads attributes of the scene node.
	virtual void deserializeAttributes(irr::io::IAttributes* in, irr::io::SAttributeReadWriteOptions* options = 0);

	/// Returns the translation of the node
	virtual irr::core::vector2d<irr::f32>& getTranslation() { return Translation; }
	/// Sets the translation of the node
	virtual void setTranslation(const irr::core::vector2d<irr::f32>& translation);

	/// Returns the scale of the cloud texture
	virtual irr::f32 getTextureScale() { return TextureScale; }
	/// Sets the scale of the cloud texture
	virtual void setTextureScale(irr::f32 textureScale); 
		
	/// Returns the inner radius
	virtual irr::f32 getInnerRadius() { return InnerRadius; }
	/// Returns the outer radius
	virtual irr::f32 getOuterRadius() { return OuterRadius; }
		
	/// Returns the center height
	virtual irr::f32 getCenterHeight() { return CenterHeight; }
	/// Returns the inner height
	virtual irr::f32 getInnerHeight() { return InnerHeight; }
	/// Returns the outer height
	virtual irr::f32 getOuterHeight() { return OuterHeight; }

	/// Returns the center color
	virtual irr::video::SColor& getCenterColor() { return CenterColor; }
	/// Returns the inner color
	virtual irr::video::SColor& getInnerColor() { return InnerColor; }
	/// Returns the outer color
	virtual irr::video::SColor& getOuterColor() { return OuterColor; }
		
	/// Sets the cloud radius
	virtual void setCloudRadius(irr::f32 innerRadius, irr::f32 outerRadius);
		
	/// Sets the cloud height
	virtual void setCloudHeight(irr::f32 centerHeight, irr::f32 innerHeight, irr::f32 outerHeight);

	/// Sets the cloud colors
	virtual void setCloudColor(const irr::video::SColor& centerColor = irr::video::SColor(220,220,220,220), 
							   const irr::video::SColor& innerColor = irr::video::SColor(180,180,180,180),
							   const irr::video::SColor& outerColor = irr::video::SColor(0,0,0,0));
};

} // end namespace engine
} // end namespace cp3d

#endif
