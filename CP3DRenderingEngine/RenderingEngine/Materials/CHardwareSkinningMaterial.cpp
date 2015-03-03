#include "Stdafx.h"
#include "CHardwareSkinningMaterial.h"

#include "../Handler/CP3DHandler.h"
#include "CMaterialCreator.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;

namespace cp3d {
namespace rendering {

CHardwareSkinningMaterial::CHardwareSkinningMaterial(CCP3DHandler *handler, IAnimatedMeshSceneNode* node, u32 refresh)
	: ICP3DHardwareSkinningMaterial(), Handler(handler), Delay(refresh), Node(node)
{
	setupNode();
}

CHardwareSkinningMaterial::~CHardwareSkinningMaterial() {
	Handler->getVideoDriver()->getMaterialRenderer(Material)->drop();
	drop();
}

void CHardwareSkinningMaterial::setupNode() {

	Mesh = (ISkinnedMesh*)Node->getMesh();
	Mesh->setHardwareMappingHint(EHM_DYNAMIC);

	ISceneManager* smgr = Handler->getActiveSceneManager();
	IVideoDriver* driver = Handler->getVideoDriver();

	for (u32 i = 0; i < Mesh->getMeshBuffers().size(); i++) {
		for (u32 g = 0; g < Mesh->getMeshBuffers()[i]->getVertexCount(); g++) {
			Mesh->getMeshBuffers()[i]->getVertex(g)->Color = SColor(0, 0, 0, 0);
		}
	}

	for (u32 z = 0; z < Mesh->getAllJoints().size(); ++z) {
		for (u32 j = 0; j < Mesh->getAllJoints()[z]->Weights.size(); ++j) {
			int buffId = Mesh->getAllJoints()[z]->Weights[j].buffer_id;

			int vertexId = Mesh->getAllJoints()[z]->Weights[j].vertex_id;
			SColor* vColor = &Mesh->getMeshBuffers()[buffId]->getVertex(vertexId)->Color;

			if (vColor->getRed() == 0)
				vColor->setRed(z + 1);
			else if (vColor->getGreen() == 0)
				vColor->setGreen(z + 1);
			else if (vColor->getBlue() == 0)
				vColor->setBlue(z + 1);
			else if (vColor->getAlpha() == 0)
				vColor->setAlpha(z + 1);
		}
	}

	#ifdef _DEBUG
	printf("Joint Num: %d, current limit is 55 joints\n", Mesh->getAllJoints().size());
	#endif

	CMaterialCreator cm(driver);
	cm.addDefine("MAX_JOINT_NUM", stringc(Mesh->getAllJoints().size()));

	Material = cm.createMaterialFromFiles("shaders/Materials/HardwareSkinning.vertex.fx", "", EMT_SOLID, this);
	DepthMaterial = cm.createCustomDepthMaterialFromFiles("shaders/Materials/HardwareSkinning.vertex.fx", "shaders/Materials/HardwareSkinning.fragment.fx", EMT_SOLID, this);
	ShadowsMaterial = cm.createCustomShadowsMaterialFromFile("shaders/Materials/HardwareSkinning.vertex.fx", EMT_SOLID, this);

	Mesh->setHardwareSkinning(true);
	Node->setMaterialType((E_MATERIAL_TYPE)Material);
}

void CHardwareSkinningMaterial::copyMat(f32* pointer, matrix4 &mat) {
	for (u32 i = 0; i < 16; i++)
		*pointer++ = mat[i];
}

void CHardwareSkinningMaterial::OnSetConstants(video::IMaterialRendererServices* services, s32 userData)
{
	if (services->getVideoDriver()->getDriverType() == EDT_DIRECT3D9) {
		core::matrix4 worldViewProj;
		worldViewProj = services->getVideoDriver()->getTransform(video::ETS_PROJECTION);
		worldViewProj *= services->getVideoDriver()->getTransform(video::ETS_VIEW);
		worldViewProj *= services->getVideoDriver()->getTransform(video::ETS_WORLD);

		core::matrix4 world;
		world = services->getVideoDriver()->getTransform(video::ETS_WORLD)[0];
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);
		services->setVertexShaderConstant("mWorld", world.pointer(), 16);
	}

	u32 timer = Handler->getIrrlichtDevice()->getTimer()->getRealTime();

	if (timer - Oldtimer >= Delay) {
		Oldtimer = timer;

		if (Mesh) {
			f32* JointArray = new f32[Mesh->getAllJoints().size() * 16];
			int copyIncrement = 0;

			for (u32 i = 0; i < Mesh->getAllJoints().size(); i++) {
				core::matrix4 JointVertexPull(core::matrix4::EM4CONST_NOTHING);
				JointVertexPull.setbyproduct(
					Mesh->getAllJoints()[i]->GlobalAnimatedMatrix,
					Mesh->getAllJoints()[i]->GlobalInversedMatrix);

				copyMat(JointArray + copyIncrement, JointVertexPull);
				copyIncrement += 16;
			}

			bool success = services->setVertexShaderConstant("JointTransform", JointArray, Mesh->getAllJoints().size() * 16);
			delete[] JointArray;
		}
	}

	ICP3DHandlerCustomCallback::OnSetConstants(services, userData);
}

} /// End namespace rendering
} /// End namespace cp3d
