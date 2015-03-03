// Copyright (C) 2015 Christian Holz
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#ifndef __C_INSTANCE_SCENE_NODE_H_INCLUDED__
#define __C_INSTANCE_SCENE_NODE_H_INCLUDED__

#include "IInstancedMeshSceneNode.h"

namespace irr
{
	namespace scene
	{
		class IMesh;
		class IMeshBuffer;

		class CInstancedMeshSceneNode : public IInstancedMeshSceneNode
		{
		public:

			//! constructor
			CInstancedMeshSceneNode(IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, s32 id,
				const core::vector3df& position = core::vector3df(0, 0, 0),
				const core::vector3df& rotation = core::vector3df(0, 0, 0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f));

			//! destructor
			virtual ~CInstancedMeshSceneNode();

			//! frame
			virtual void OnRegisterSceneNode();

			//! renders the node.
			virtual void render();

			virtual void OnAnimate(u32 timeMs);

			//! returns the axis aligned bounding box of this node
			virtual const core::aabbox3d<f32>& getBoundingBox() const;

			//! returns the material based on the zero based index i. To get the amount
			//! of materials used by this scene node, use getMaterialCount().
			//! This function is needed for inserting the node into the scene hirachy on a
			//! optimal position for minimizing renderstate changes, but can also be used
			//! to directly modify the material of a scene node.
			virtual video::SMaterial& getMaterial(u32 i);

			//! returns amount of materials used by this scene node.
			virtual u32 getMaterialCount() const;

			//! Returns type of the scene node
			virtual ESCENE_NODE_TYPE getType() const { return ESNT_MESH; }

			//! Sets a new mesh
			virtual void setMesh(IMesh* mesh);

			virtual ISceneNode* addInstance(ISceneNode* node);

			virtual ISceneNode* addInstance(const core::vector3df& position,
				const core::vector3df& rotation = core::vector3df(0, 0, 0),
				const core::vector3df& scale = core::vector3df(1.0f, 1.0f, 1.0f),
				s32 id = -1);

			virtual bool removeInstance(u32 index);

			virtual bool removeInstance(ISceneNode* instance);

			virtual bool removeInstanceById(s32 id);

			virtual ISceneNode* getInstance(u32 index) const;

			virtual ISceneNode* getInstanceById(s32 id) const;

			virtual u32 getInstanceCount() const;

			virtual ISceneNode* clone(ISceneNode* newParent, ISceneManager* newManager);

			//! Get the currently defined mesh for display.
			/** \return Pointer to mesh which is displayed by this node. */
			virtual IMesh* getMesh(void);

			//! Creates shadow volume scene node as child of this node.
			/** The shadow can be rendered using the ZPass or the zfail
			method. ZPass is a little bit faster because the shadow volume
			creation is easier, but with this method there occur ugly
			looking artifacs when the camera is inside the shadow volume.
			These error do not occur with the ZFail method.
			\param shadowMesh: Optional custom mesh for shadow volume.
			\param id: Id of the shadow scene node. This id can be used to
			identify the node later.
			\param zfailmethod: If set to true, the shadow will use the
			zfail method, if not, zpass is used.
			\param infinity: Value used by the shadow volume algorithm to
			scale the shadow volume (for zfail shadow volume we support only
			finite shadows, so camera zfar must be larger than shadow back cap,
			which is depend on infinity parameter).
			\return Pointer to the created shadow scene node. This pointer
			should not be dropped. See IReferenceCounted::drop() for more
			information. */
			virtual IShadowVolumeSceneNode* addShadowVolumeSceneNode(const IMesh* shadowMesh = 0,
				s32 id = -1, bool zfailmethod = true, f32 infinity = 1000.0f);

			//! Sets if the scene node should not copy the materials of the mesh but use them in a read only style.
			/** In this way it is possible to change the materials of a mesh
			causing all mesh scene nodes referencing this mesh to change, too.
			\param readonly Flag if the materials shall be read-only. */
			virtual void setReadOnlyMaterials(bool readonly);

			//! Check if the scene node should not copy the materials of the mesh but use them in a read only style
			/** This flag can be set by setReadOnlyMaterials().
			\return Whether the materials are read-only. */
			virtual bool isReadOnlyMaterials() const;

			virtual void updateInstances();

			virtual bool isStatic() const;

			virtual void setStatic(bool staticInstances = true);

		protected:
			core::aabbox3d<f32> box;

			//IMeshBuffer* renderBuffer;
			IMesh* baseMesh;

			bool staticInstances;

			core::array<ISceneNode*> instanceNodeArray;

			bool readOnlyMaterial;
			video::SMaterial material;

			struct StarInst
			{
				irr::core::matrix4 transformation;

				bool operator==(const StarInst &other) const
				{
					if ((this->transformation == other.transformation))
						return true;

					return false;
				}
			};
		};

	} // end namespace scene
} // end namespace irr

#endif

