
#ifndef __C_MESH_SCENE_NODE_INSTANCES_H_INCLUDED__
#define __C_MESH_SCENE_NODE_INSTANCES_H_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace engine {

	class CMeshSceneNodeInstances : public irr::scene::IMeshSceneNode {
public:

	//! constructor
	CMeshSceneNodeInstances(irr::scene::IMesh* mesh, ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		const irr::core::vector3df& position = irr::core::vector3df(0,0,0),
		const irr::core::vector3df& rotation = irr::core::vector3df(0, 0, 0),
		const irr::core::vector3df& scale = irr::core::vector3df(1.0f, 1.0f, 1.0f));

	//! destructor
	virtual ~CMeshSceneNodeInstances();

	//! frame
	virtual void OnRegisterSceneNode();

	//! renders the node.
	virtual void render();

	//! returns the axis aligned bounding box of this node
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

	//! returns the material based on the zero based index i. To get the amount
	//! of materials used by this scene node, use getMaterialCount().
	//! This function is needed for inserting the node into the scene hirachy on a
	//! optimal position for minimizing renderstate changes, but can also be used
	//! to directly modify the material of a scene node.
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);
		
	//! returns amount of materials used by this scene node.
	virtual irr::u32 getMaterialCount() const;

	//! Writes attributes of the scene node.
	virtual void serializeAttributes(irr::io::IAttributes* out, irr::io::SAttributeReadWriteOptions* options = 0) const;

	//! Reads attributes of the scene node.
	virtual void deserializeAttributes(irr::io::IAttributes* in, irr::io::SAttributeReadWriteOptions* options = 0);

	//! Returns type of the scene node
	virtual irr::scene::ESCENE_NODE_TYPE getType() const { return irr::scene::ESNT_MESH; }

	//! Sets a new mesh
	virtual void setMesh(irr::scene::IMesh* mesh);

	//! Returns the current mesh
	virtual irr::scene::IMesh* getMesh(void) { return Mesh; }

	//! Creates shadow volume scene node as child of this node
	//! and returns a pointer to it.
	virtual irr::scene::IShadowVolumeSceneNode* addShadowVolumeSceneNode(const irr::scene::IMesh* shadowMesh,
		irr::s32 id, bool zfailmethod = true, irr::f32 infinity = 10000.0f);

	//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
	/* In this way it is possible to change the materials a mesh causing all mesh scene nodes 
	referencing this mesh to change too. */
	virtual void setReadOnlyMaterials(bool readonly);

	//! Returns if the scene node should not copy the materials of the mesh but use them in a read only style
	virtual bool isReadOnlyMaterials() const;

	//! Creates a clone of this scene node and its children.
	virtual ISceneNode* clone(ISceneNode* newParent = 0, irr::scene::ISceneManager* newManager = 0);

	//! Removes a child from this scene node.
	//! Implemented here, to be able to remove the shadow properly, if there is one,
	//! or to remove attached childs.
	virtual bool removeChild(ISceneNode* child);

protected:

	void copyMaterials();

	irr::core::array<irr::video::SMaterial> Materials;
	irr::core::aabbox3d<irr::f32> Box;
	irr::video::SMaterial ReadOnlyMaterial;

	irr::scene::IMesh* Mesh;

	irr::s32 PassCount;
	bool ReadOnlyMaterials;
};

} // end namespace scene
} // end namespace irr

#endif

