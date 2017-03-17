#include "stdafx.h"
#include "CVREventReceiver.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

namespace cp3d {
namespace engine {

using namespace rendering;

CVREventReceiver::CVREventReceiver(ICP3DRenderingEngine *rengine, vr::IVRSystem *vrSystem) : Rengine(rengine), VRSystem(vrSystem) {
	memset(TrackedDeviceNodes, 0, vr::k_unMaxTrackedDeviceCount);
	memset(TrackedDeviceTextures, 0, vr::k_unMaxTrackedDeviceCount);

	// Load models
	for (u32 unTrackedDevice = vr::k_unTrackedDeviceIndex_Hmd + 1; unTrackedDevice < vr::k_unMaxTrackedDeviceCount; unTrackedDevice++)
	{
		if (!vrSystem->IsTrackedDeviceConnected(unTrackedDevice))
			continue;

		setupDeviceModel(unTrackedDevice);
	}
}

CVREventReceiver::~CVREventReceiver() {
	// Remove nodes
	for (u32 i = 0; i < vr::k_unMaxTrackedDeviceCount; i++) {
		// MeshSceneNode drop the meshes then it's cool
		if (TrackedDeviceNodes[i])
			TrackedDeviceNodes[i]->drop();

		if (TrackedDeviceTextures[i])
			TrackedDeviceTextures[i]->drop();
	}
}

void CVREventReceiver::onEvent(const vr::VREvent_t &event) {
	switch (event.eventType) {
		case vr::VREvent_TrackedDeviceActivated:
			setupDeviceModel(event.trackedDeviceIndex);
			break;
		default: break;
	}
}

void CVREventReceiver::setupDeviceModel(const vr::TrackedDeviceIndex_t &index){
	u32 modelNameBufferLen = VRSystem->GetStringTrackedDeviceProperty(index, vr::Prop_RenderModelName_String, 0, 0, 0);
	if (modelNameBufferLen == 0)
		return;

	char *modelNameBuffer = new char[modelNameBufferLen];
	modelNameBufferLen = VRSystem->GetStringTrackedDeviceProperty(index, vr::Prop_RenderModelName_String, modelNameBuffer, modelNameBufferLen, NULL);
	stringc modelName(modelNameBuffer);
	delete[] modelNameBuffer;

	// Load model
	vr::EVRRenderModelError error;
	vr::RenderModel_t *pModel;
	vr::RenderModel_TextureMap_t *pTexture;

	while (1) { // Model
		error = vr::VRRenderModels()->LoadRenderModel_Async(modelName.c_str(), &pModel);
		if (error != vr::VRRenderModelError_Loading)
			break;
	}

	if (error != vr::VRRenderModelError_None)
		return;

	while (1) { // Texture
		error = vr::VRRenderModels()->LoadTexture_Async(pModel->diffuseTextureId, &pTexture);
		if (error != vr::VRRenderModelError_Loading)
			break;
	}

	if (error != vr::VRRenderModelError_None) {
		vr::VRRenderModels()->FreeRenderModel(pModel);
		return;
	}

	// Create mesh buffer
	SMeshBuffer *buffer = new SMeshBuffer();
	for (u32 i = 0; i < pModel->unVertexCount; i++) {
		S3DVertex vertex;
		vertex.Pos.X = pModel->rVertexData[i].vPosition.v[0];
		vertex.Pos.Y = pModel->rVertexData[i].vPosition.v[1];
		vertex.Pos.Z = pModel->rVertexData[i].vPosition.v[2];

		vertex.Normal.X = pModel->rVertexData[i].vNormal.v[0];
		vertex.Normal.Y = pModel->rVertexData[i].vNormal.v[1];
		vertex.Normal.Z = pModel->rVertexData[i].vNormal.v[2];

		vertex.TCoords.X = pModel->rVertexData[i].rfTextureCoord[0];
		vertex.TCoords.Y = pModel->rVertexData[i].rfTextureCoord[1];

		buffer->Vertices.push_back(vertex);
	}

	buffer->Indices.set_used(pModel->unTriangleCount * 3);
	for (u32 i = 0; i < pModel->unTriangleCount * 3; i++)
		buffer->Indices[i] = pModel->rIndexData[i];

	// Create mesh
	SMesh *mesh = new SMesh();
	mesh->addMeshBuffer(buffer);
	mesh->setDirty();

	// Create texture image
	IImage *image = Rengine->getVideoDriver()->createImage(ECF_A8R8G8B8, dimension2du(pTexture->unWidth, pTexture->unHeight));
	u8 *data = (u8 *)image->lock();
	memcpy(data, pTexture->rubTextureMapData, pTexture->unWidth * pTexture->unHeight * 4);

	ITexture *texture = Rengine->getVideoDriver()->addTexture("VRTexture" + pModel->diffuseTextureId, image);

	// Create scene node
	IMeshSceneNode *node = Rengine->getSceneManager()->addMeshSceneNode(mesh);
	node->setName(modelName);
	node->setMaterialFlag(EMF_LIGHTING, false);
	node->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
	node->setMaterialFlag(EMF_FRONT_FACE_CULLING, true);
	node->setMaterialTexture(0, texture);

	Rengine->getHandler()->addShadowToNode(node, EFT_NONE, ESM_EXCLUDE);

	// Finish
	vr::VRRenderModels()->FreeRenderModel(pModel);
	vr::VRRenderModels()->FreeTexture(pTexture);

	TrackedDeviceNodes[index] = node;
	TrackedDeviceTextures[index] = texture;
}

} /// End namespace engine
} /// End namespace cp3d
