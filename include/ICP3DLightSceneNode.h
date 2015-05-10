#ifndef _H_ICP3D_LIGHT_SCENE_NODE_INCLUDED__
#define _H_ICP3D_LIGHT_SCENE_NODE_INCLUDED__

#include <irrlicht.h>
#include <SShadowLight.h>

#define DEFAULT_LIGHT_STRENGTH 0.6f

namespace cp3d {
namespace rendering {

class CCP3DRenderingEngine;

/*
Structure that handle all lights datas
*/
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
	inline const bool isComputingNormalMapping() const {
		return ComputeNormalMapping;
	}

	//! Sets if the light computes normal mapping
	//! \param compute: true if used by normal mapping material
	inline void setComputeNormalMapping(const bool compute) {
		ComputeNormalMapping = compute;
	}

	//! Returns the light's strength
	inline const irr::f32 getLightStrength() const {
		return LightStrength;
	}

	//! Sets the light's strength
	//! \param strength: the new strength of the light
	inline void setLightStrength(const irr::f32 strength) {
		LightStrength = strength;
	}

	//! Returns the shadow light
	inline SShadowLight *getShadowLight() {
		return ShadowLight;
	}

	//! Returns the shadow light index in the ICP3DHandler
	inline const irr::s32 getShadowLightIndex() const {
		return ShadowLightIndex;
	}

	//! Returns if the light computes shadows
	//! If true, then getShadowLightIndex should return an index
	//! != -1
	inline const bool isComputingShadows() const {
		return ShadowLightIndex != -1;
	}

	//! Returns the light data
	inline irr::video::SLight &getLightData() {
		return Node->getLightData();
	}

	//! Sets the light's color
	//! \param color: the new color of the light
	inline void setLightColor(const irr::video::SColorf &color) {
		Node->getLightData().DiffuseColor = color;

		if (ShadowLight)
			ShadowLight->setLightColor(color);
	}

	//! Sets the light's position
	inline void setPosition(const irr::core::vector3df& position) {
		Node->setPosition(position);
		if (ShadowLight)
			ShadowLight->setPosition(position);
	}

	//! Sets the new parent of the light
	inline void setParent(irr::scene::ISceneNode *parent) {
		parent->addChild(Node);
	}

	//! Sets the light's name
	inline void setName(const irr::core::stringc &name) {
		Node->setName(name.c_str());
	}

	//! Returns the light's name
	inline irr::core::stringc getName() {
		return Node->getName();
	}

	//! Returns the scene node type of the light (ESNT_LIGHT)
	inline const irr::scene::ESCENE_NODE_TYPE getType() const {
		return irr::scene::ESNT_LIGHT;
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
	{ }

	//! Private destructor
	~ICP3DLightSceneNode() {
		Node->remove();
	}
};

} /// End rendering namespace
} /// End cp3d namespace

#endif
