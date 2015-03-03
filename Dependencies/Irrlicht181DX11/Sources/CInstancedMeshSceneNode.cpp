// Copyright (C) 2015 Christian Holz
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "CInstancedMeshSceneNode.h"
#include "IVideoDriver.h"
#include "ISceneManager.h"
#include "IVertexDescriptor.h"
#include "os.h"

namespace irr
{
	namespace scene
	{

		//! constructor
		CInstancedMeshSceneNode::CInstancedMeshSceneNode(IMesh* mesh, ISceneNode* parent, ISceneManager* mgr, s32 id,
			const core::vector3df& position, const core::vector3df& rotation, const core::vector3df& scale)
			: IInstancedMeshSceneNode(parent, mgr, id, position, rotation, scale),
			baseMesh(NULL), readOnlyMaterial(false), staticInstances(false)
		{
#ifdef _DEBUG
			setDebugName("CInstancedMeshSceneNode");
#endif
			setMesh(mesh);
		}

		//! destructor
		CInstancedMeshSceneNode::~CInstancedMeshSceneNode()
		{
			instanceNodeArray.clear();

			if (baseMesh)
				baseMesh->drop();
		}

		//! frame
		void CInstancedMeshSceneNode::OnRegisterSceneNode()
		{
			if (IsVisible)
			{
				SceneManager->registerNodeForRendering(this);

				ISceneNode::OnRegisterSceneNode();
			}
		}

		/*Creates an empty scenenode and return the pointer to the user
		this empty scene node is used for irrlicht and the user to manage the instance
		Each instance can be manipulated individualy through thier own empty scenenode*/
		ISceneNode* CInstancedMeshSceneNode::addInstance(scene::ISceneNode* node)
		{
			return addInstance(node->getPosition(), node->getRotation(), node->getScale(), node->getID());
		}

		/*Creates an empty scenenode and return the pointer to the user
		this empty scene node is used for irrlicht and the user to manage the instance
		Each instance can be manipulated individualy through thier own empty scenenode*/
		ISceneNode* CInstancedMeshSceneNode::addInstance(const core::vector3df& position, const core::vector3df& rotation,
			const core::vector3df& scale, s32 id)
		{
			ISceneNode* empty = SceneManager->addEmptySceneNode(this, id);

			empty->setPosition(position);
			empty->setScale(scale);
			empty->setRotation(rotation);

			empty->updateAbsolutePosition();

			instanceNodeArray.push_back(empty);
			return empty;
		}

		bool CInstancedMeshSceneNode::removeInstance(u32 index)
		{
			if (index > instanceNodeArray.size())
				return false;

			removeChild(instanceNodeArray[index]);

			instanceNodeArray[index]->drop();
			instanceNodeArray[index] = instanceNodeArray[instanceNodeArray.size() - 1];
			instanceNodeArray.erase(instanceNodeArray.size() - 1);

			return true;
		}

		bool CInstancedMeshSceneNode::removeInstance(ISceneNode* instance)
		{
			const u32 size = instanceNodeArray.size();

			for (u32 i = 0; i < size; ++i)
			{
				if (instanceNodeArray[i] == instance)
				{
					removeChild(instanceNodeArray[i]);

					instanceNodeArray[i]->drop();
					instanceNodeArray[i] = instanceNodeArray[instanceNodeArray.size() - 1];
					instanceNodeArray.erase(instanceNodeArray.size() - 1);

					return true;
				}
			}

			return false;
		}

		bool CInstancedMeshSceneNode::removeInstanceById(s32 id)
		{
			const u32 size = instanceNodeArray.size();

			for (u32 i = 0; i < size; ++i)
			{
				if (instanceNodeArray[i]->getID() == id)
				{
					removeChild(instanceNodeArray[i]);

					instanceNodeArray[i]->drop();
					instanceNodeArray[i] = instanceNodeArray[instanceNodeArray.size() - 1];
					instanceNodeArray.erase(instanceNodeArray.size() - 1);

					return true;
				}
			}

			return false;
		}

		ISceneNode* CInstancedMeshSceneNode::getInstance(u32 index) const
		{
			if (index > instanceNodeArray.size())
				return NULL;

			return instanceNodeArray[index];
		}

		ISceneNode* CInstancedMeshSceneNode::getInstanceById(s32 id) const
		{
			const u32 size = instanceNodeArray.size();

			for (u32 i = 0; i < size; ++i)
			{
				if (instanceNodeArray[i]->getID() == id)
				{
					return instanceNodeArray[i];
				}
			}

			return NULL;
		}

		u32 CInstancedMeshSceneNode::getInstanceCount() const
		{
			return instanceNodeArray.size();
		}

		//! renders the node.
		void CInstancedMeshSceneNode::render()
		{
			if (!IsVisible || !SceneManager->getActiveCamera())
				return;

			if (!baseMesh || baseMesh->getMeshBuffer(0)->getVertexBuffer(1)->getVertexCount() == 0)
				return;

			IMeshBuffer* renderBuffer = baseMesh->getMeshBuffer(0);

			video::IVideoDriver* driver = SceneManager->getVideoDriver();
			driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
			driver->setMaterial(readOnlyMaterial ? material : renderBuffer->getMaterial());

			driver->drawMeshBuffer(renderBuffer);

			// for debug purposes only:
			if (DebugDataVisible)
			{
				video::SMaterial m;
				m.Lighting = false;
				driver->setMaterial(m);

				if (DebugDataVisible & scene::EDS_BBOX)
					driver->draw3DBox(box, video::SColor(255, 255, 255, 255));

				if (DebugDataVisible & scene::EDS_BBOX_BUFFERS)
				{
					const u32 size = instanceNodeArray.size();

					for (u32 i = 0; i < size; ++i)
					{
						core::aabbox3df box = renderBuffer->getBoundingBox();
						driver->setTransform(video::ETS_WORLD, instanceNodeArray[i]->getAbsoluteTransformation());
						driver->draw3DBox(box, video::SColor(255, 255, 255, 255));
					}
				}
			}
		}

		void CInstancedMeshSceneNode::OnAnimate(u32 timeMs)
		{
			ISceneNode::OnAnimate(timeMs);

			if (!isStatic())
				updateInstances();
		}

		//! returns the axis aligned bounding box of this node
		const core::aabbox3d<f32>& CInstancedMeshSceneNode::getBoundingBox() const
		{
			return box;
		}

		void CInstancedMeshSceneNode::updateInstances()
		{
			if (!baseMesh || baseMesh->getMeshBuffer(0)->getVertexBufferCount() != 2)
				return;

			IMeshBuffer* renderBuffer = baseMesh->getMeshBuffer(0);

			box.reset(0, 0, 0);

			const u32 size = instanceNodeArray.size();

			renderBuffer->getVertexBuffer(1)->clear();
			renderBuffer->getVertexBuffer(1)->setDirty();
			renderBuffer->getVertexBuffer(1)->set_used(size);

			for (u32 i = 0; i < size; ++i)
			{
				core::aabbox3df instanceBox = renderBuffer->getBoundingBox();
				instanceNodeArray[i]->getAbsoluteTransformation().transformBoxEx(instanceBox);

				if (!SceneManager->isCulled(instanceNodeArray[i]))
					renderBuffer->getVertexBuffer(1)->setVertex(i, &instanceNodeArray[i]->getAbsoluteTransformation());

				box.addInternalPoint(instanceNodeArray[i]->getAbsolutePosition());
			}

			renderBuffer->getVertexBuffer(1)->setDirty();
		}

		bool CInstancedMeshSceneNode::isStatic() const
		{
			return staticInstances;
		}

		void CInstancedMeshSceneNode::setStatic(bool staticInstances)
		{
			this->staticInstances = staticInstances;
		}

		//! returns the material based on the zero based index i. To get the amount
		//! of materials used by this scene node, use getMaterialCount().
		//! This function is needed for inserting the node into the scene hierarchy on a
		//! optimal position for minimizing renderstate changes, but can also be used
		//! to directly modify the material of a scene node.
		video::SMaterial& CInstancedMeshSceneNode::getMaterial(u32 i)
		{
			if (readOnlyMaterial)
				return material;

			return baseMesh->getMeshBuffer(0)->getMaterial();
		}


		//! returns amount of materials used by this scene node.
		u32 CInstancedMeshSceneNode::getMaterialCount() const
		{
			return 1;
		}


		//! Sets a new mesh
		void CInstancedMeshSceneNode::setMesh(IMesh* mesh)
		{
			video::IVideoDriver* driver = SceneManager->getVideoDriver();
			if (baseMesh)
			{
				baseMesh->drop();
				baseMesh = NULL;
			}

			if (mesh)
			{
				mesh->grab();

				baseMesh = mesh;

				IMeshBuffer* renderBuffer = baseMesh->getMeshBuffer(0);

				while (renderBuffer->getVertexBufferCount() > 1)
					renderBuffer->removeVertexBuffer(1);

				// TODO add getter and so that the smaller value isnt erased
				// maybe the func isnt longer needed so delete it
				driver->setMinHardwareBufferVertexCount(renderBuffer->getVertexBuffer()->getVertexCount());

				renderBuffer->setHardwareMappingHint(scene::EHM_STATIC);

				video::IVertexDescriptor* index = SceneManager->getVideoDriver()->getVertexDescriptor("BaseInstanceIndex");

				if (!index)
				{
					video::IVertexDescriptor* stdv = SceneManager->getVideoDriver()->getVertexDescriptor(0);
					index = SceneManager->getVideoDriver()->addVertexDescriptor("BaseInstanceIndex");

					for (u32 i = 0; i < stdv->getAttributeCount(); ++i)
					{
						index->addAttribute(stdv->getAttribute(i)->getName(), stdv->getAttribute(i)->getElementCount(), stdv->getAttribute(i)->getSemantic(), stdv->getAttribute(i)->getType(), stdv->getAttribute(i)->getBufferID());
					}

					index->addAttribute("InstancingMatrix1", 4, video::EVAS_TEXCOORD1, video::EVAT_FLOAT, 1);
					index->addAttribute("InstancingMatrix2", 4, video::EVAS_TEXCOORD2, video::EVAT_FLOAT, 1);
					index->addAttribute("InstancingMatrix3", 4, video::EVAS_TEXCOORD3, video::EVAT_FLOAT, 1);
					index->addAttribute("InstancingMatrix4", 4, video::EVAS_TEXCOORD4, video::EVAT_FLOAT, 1);

					index->setInstanceDataStepRate(video::EIDSR_PER_INSTANCE, 1);
				}

				IVertexBuffer* instanceBuffer = new CVertexBuffer<core::matrix4>();

				renderBuffer->setVertexDescriptor(index);
				renderBuffer->addVertexBuffer(instanceBuffer);

				renderBuffer->getVertexBuffer(0)->setHardwareMappingHint(scene::EHM_STATIC);
				renderBuffer->getVertexBuffer(1)->setHardwareMappingHint(scene::EHM_STATIC);
				renderBuffer->getIndexBuffer()->setHardwareMappingHint(scene::EHM_STATIC);
				instanceBuffer->drop();
			}
		}

		//! Creates a clone of this scene node and its children.
		ISceneNode* CInstancedMeshSceneNode::clone(ISceneNode* newParent, ISceneManager* newManager)
		{
			if (!newParent)
				newParent = Parent;
			if (!newManager)
				newManager = SceneManager;

			CInstancedMeshSceneNode* nb = new CInstancedMeshSceneNode(baseMesh, newParent,
				newManager, ID, RelativeTranslation, RelativeRotation, RelativeScale);

			nb->cloneMembers(this, newManager);

			nb->readOnlyMaterial = readOnlyMaterial;
			nb->instanceNodeArray = instanceNodeArray;
			nb->baseMesh = baseMesh;

			if (baseMesh)
				baseMesh->grab();

			nb->box = box;
			nb->material = material;

			if (newParent)
				nb->drop();
			return nb;
		}

		IMesh* CInstancedMeshSceneNode::getMesh(void)
		{
			return baseMesh;
		}


		IShadowVolumeSceneNode* CInstancedMeshSceneNode::addShadowVolumeSceneNode(const IMesh* shadowMesh,
			s32 id, bool zfailmethod, f32 infinity)
		{
			return NULL;
		}

		void CInstancedMeshSceneNode::setReadOnlyMaterials(bool readonly)
		{
			readOnlyMaterial = readonly;
		}

		bool CInstancedMeshSceneNode::isReadOnlyMaterials() const
		{
			return readOnlyMaterial;
		}
	} // end namespace scene
} // end namespace irr

