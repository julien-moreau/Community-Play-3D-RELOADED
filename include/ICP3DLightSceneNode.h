#ifndef _H_ICP3D_LIGHT_SCENE_NODE_INCLUDED__
#define _H_ICP3D_LIGHT_SCENE_NODE_INCLUDED__

#include <irrlicht.h>
#include <SShadowLight.h>

#define DEFAULT_LIGHT_STRENGTH 0.6f

namespace cp3d {
namespace rendering {

class CCP3DRenderingEngine;

struct ICP3DLightSceneNode {
	friend class CCP3DRenderingEngine;
public:

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

	//! Returns the shadow light
	SShadowLight *getShadowLight() {
		return ShadowLight;
	}

	//! Returns the shadow light index in the ICP3DHandler
	const irr::s32 getShadowLightIndex() const {
		return ShadowLightIndex;
	}

	//! Returns if the light computes shadows
	//! If true, then getShadowLightIndex should return an index
	//! != -1
	const bool isComputingShadows() const {
		return ShadowLightIndex != -1;
	}

	//! Returns the light data
	irr::video::SLight &getLightData() {
		return Node->getLightData();
	}

	//! Sets the light's position
	void setPosition(const irr::core::vector3df& position) {
		Node->setPosition(position);
		if (ShadowLight)
			ShadowLight->setPosition(position);
	}

	//! Sets the new parent of the light
	void setParent(irr::scene::ISceneNode *parent) {
		parent->addChild(Node);
	}

	//! Sets the light's name
	void setName(const irr::core::stringc &name) {
		Node->setName(name.c_str());
	}

	//! Returns the light's name
	irr::core::stringc getName() {
		return Node->getName();
	}

private:
	/// Light scene node (irrlicht)
	irr::scene::ILightSceneNode *Node;

	/// Normal Mapping
	bool ComputeNormalMapping;
	irr::f32 LightStrength;

	/// Shadows
	SShadowLight *ShadowLight;
	irr::s32 ShadowLightIndex;

	//! Private constructor
	//! \param node: the light scene node
	//! \param computeNormalMapping: true if the light computes normal mapping material
	//! \param computeShadows: true if the light computes shadows
	ICP3DLightSceneNode(irr::scene::ILightSceneNode *node, const bool computeNormalMapping, const irr::s32 shadowLightIndex)
		: Node(node), ComputeNormalMapping(computeNormalMapping), LightStrength(DEFAULT_LIGHT_STRENGTH), ShadowLightIndex(shadowLightIndex)
	{

	}
};

} /// End rendering namespace
} /// End cp3d namespace

#endif
