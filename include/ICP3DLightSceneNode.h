#ifndef _H_ICP3D_LIGHT_SCENE_NODE_INCLUDED__
#define _H_ICP3D_LIGHT_SCENE_NODE_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

struct ICP3DLightSceneNode {
public:

	//! Constructor
	//! \param node: the light scene node
	ICP3DLightSceneNode(irr::scene::ILightSceneNode *node) : Node(node), ComputeNormalMapping(true),
		LightStrength(0.6f)
	{

	}

	//! *operator (indirection)
	//! Example :
	/*
	cp3d::rendering::ICP3LightSceneNode *light = engine->createLightSceneNode();
	irr::scene::ILightSceneNode *irrLight = *light;
	*/
	operator irr::scene::ILightSceneNode* () const {
		return Node;
	}

	//! Returns if the light computes normal mapping
	const bool isComputingNormalMapping() const {
		return ComputeNormalMapping;
	}

	//! Sets if the light computes normal mapping
	//! \param compute: true if used by normal mapping material
	void setComputeNormalMapping(const bool compute) {
		ComputeNormalMapping = compute;
	}

	//! Returns the light's strength
	const irr::f32 getLightStrength() const {
		return LightStrength;
	}

	//! Sets the light's strength
	//! \param strength: the new strength of the light
	void setLightStrength(const irr::f32 strength) {
		LightStrength = strength;
	}

private:
	/// Light scene node (irrlicht)
	irr::scene::ILightSceneNode *Node;

	/// Normal Mapping
	bool ComputeNormalMapping;
	irr::f32 LightStrength;
};

} /// End rendering namespace
} /// End cp3d namespace

#endif
