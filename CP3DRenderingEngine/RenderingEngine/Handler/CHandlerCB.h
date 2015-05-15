#ifndef __H_HANDLER_CB_INCLUDED__
#define __H_HANDLER_CB_INCLUDED__

#include "CHandler.h"

using namespace irr;
using namespace scene;
using namespace video;
using namespace core;

namespace cp3d {
namespace rendering {

class DepthShaderCB : public ICP3DHandlerCustomCallback
{

public:
	DepthShaderCB(CCP3DHandler* effectIn) : ICP3DHandlerCustomCallback(true, false)
	{ };

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);

		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		ICP3DHandlerCustomCallback::OnSetConstants(services, userData);
	}

private:

	core::matrix4 worldViewProj;
};

class ShadowShaderCB : public ICP3DHandlerCustomCallback
{

public:

	ShadowShaderCB(CCP3DHandler* effectIn) : ICP3DHandlerCustomCallback(false, true)
	{ }

	virtual void OnSetMaterial(const SMaterial& material)
	{ }

	virtual void OnSetConstants(video::IMaterialRendererServices* services, s32 userData) {
		IVideoDriver* driver = services->getVideoDriver();

		matrix4 worldViewProj = driver->getTransform(video::ETS_PROJECTION);			
		worldViewProj *= driver->getTransform(video::ETS_VIEW);
		worldViewProj *= driver->getTransform(video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj", worldViewProj.pointer(), 16);

		ICP3DHandlerCustomCallback::OnSetConstants(services, userData);
	}

};

class CCustomPostProcessCB : public rendering::IPostProcessingRenderCallback {

public:

	CCustomPostProcessCB(std::function<void(ICP3DHandler *handler)> onPreRender,
						 std::function<void(ICP3DHandler *handler)> onPostRender)
	{
		OnPreRenderLambda = onPreRender;
		OnPostRenderLambda = onPostRender;
	}

	void OnPreRender(ICP3DHandler *handler) {
		OnPreRenderLambda(handler);
	}
	void OnPostRender(ICP3DHandler *handler) {
		OnPostRenderLambda(handler);
	}

private:

	std::function<void(ICP3DHandler *handler)> OnPreRenderLambda, OnPostRenderLambda;

};

class CScreenQuadCB : public irr::video::IShaderConstantSetCallBack {

public:

	CScreenQuadCB(CCP3DHandler* effectIn, bool defaultV = true)
		: effect(effectIn), defaultVertexShader(defaultV)
	{ }

	virtual void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
		if(services->getVideoDriver()->getDriverType() == irr::video::EDT_OPENGL) {
			irr::u32 TexVar = 0;
			services->setPixelShaderConstant("ColorMapSampler", (irr::s32*)(&TexVar), 1); 

			TexVar = 1;
			services->setPixelShaderConstant("ScreenMapSampler", (irr::s32*)(&TexVar), 1); 

			TexVar = 2;
			services->setPixelShaderConstant("DepthMapSampler", (irr::s32*)(&TexVar), 1); 

			TexVar = 3;
			services->setPixelShaderConstant("UserMapSampler", (irr::s32*)(&TexVar), 1);
		}

		if(defaultVertexShader) {
			const irr::core::dimension2du currentRTTSize = services->getVideoDriver()->getCurrentRenderTargetSize();
			const irr::f32 screenX = (irr::f32)currentRTTSize.Width, screenY = (irr::f32)currentRTTSize.Height;

			services->setVertexShaderConstant("screenX", &screenX, 1);
			services->setVertexShaderConstant("screenY", &screenY, 1);

			irr::scene::ISceneManager* smgr = effect->getActiveSceneManager();
			irr::scene::ICameraSceneNode* cam = smgr->getActiveCamera();

			const irr::core::position2di tLeft = services->getVideoDriver()->getViewPort().UpperLeftCorner;
			const irr::core::position2di bRight = services->getVideoDriver()->getViewPort().LowerRightCorner;

			const irr::core::line3df sLines[4] = {
				smgr->getSceneCollisionManager()->getRayFromScreenCoordinates
				(irr::core::position2di(tLeft.X, tLeft.Y), cam),
				smgr->getSceneCollisionManager()->getRayFromScreenCoordinates
				(irr::core::position2di(bRight.X, tLeft.Y), cam),
				smgr->getSceneCollisionManager()->getRayFromScreenCoordinates
				(irr::core::position2di(tLeft.X, bRight.Y), cam),
				smgr->getSceneCollisionManager()->getRayFromScreenCoordinates
				(irr::core::position2di(bRight.X, bRight.Y), cam)
			};

			services->setVertexShaderConstant("LineStarts0", &sLines[0].start.X, 3);
			services->setVertexShaderConstant("LineStarts1", &sLines[1].start.X, 3);
			services->setVertexShaderConstant("LineStarts2", &sLines[2].start.X, 3);
			services->setVertexShaderConstant("LineStarts3", &sLines[3].start.X, 3);

			services->setVertexShaderConstant("LineEnds0", &sLines[0].end.X, 3);
			services->setVertexShaderConstant("LineEnds1", &sLines[1].end.X, 3);
			services->setVertexShaderConstant("LineEnds2", &sLines[2].end.X, 3);
			services->setVertexShaderConstant("LineEnds3", &sLines[3].end.X, 3);
		}

		if(uniformDescriptors.size()) {
			irr::core::map<irr::core::stringc, SUniformDescriptor>::Iterator mapIter = uniformDescriptors.getIterator();

			for(;!mapIter.atEnd();mapIter++) {
				if(mapIter.getNode()->getValue().fPointer == 0)
					continue;

				services->setPixelShaderConstant(mapIter.getNode()->getKey().c_str(), mapIter.getNode()->getValue().fPointer,
					mapIter.getNode()->getValue().paramCount);
			}
		}
	}

	struct SUniformDescriptor {
		SUniformDescriptor() : fPointer(0), paramCount(0) {}

		SUniformDescriptor(const irr::f32* fPointerIn, irr::u32 paramCountIn)
			: fPointer(fPointerIn), paramCount(paramCountIn) {}

		const irr::f32* fPointer;
		irr::u32 paramCount;
	};

	CCP3DHandler* effect;
	bool defaultVertexShader;
	irr::core::map<irr::core::stringc, SUniformDescriptor> uniformDescriptors;
};

} /// End namespace rendering
} /// End namespace cp3d

#endif
