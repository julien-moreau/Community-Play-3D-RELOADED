#ifndef _H_ICP3D_HANDLER_CALLBACKS_INCLUDED__
#define _H_ICP3D_HANDLER_CALLBACKS_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

enum E_CP3D_HANDLER_PASS_TYPE {
	ECHPT_DEPTH = 0,
	ECHPT_SHADOWS,

	ECHPT_NONE
};

//! Depth shader callback
class ICP3DHandlerDepthCallback {

public:

	//! Constructor
	ICP3DHandlerDepthCallback()
	{ }

	//! Destructor
	virtual ~ICP3DHandlerDepthCallback()
	{ }

	//! OnSetConstants
	virtual inline void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
		irr::video::IVideoDriver* driver = services->getVideoDriver();
		services->setVertexShaderConstant("MaxD", &FarLink, 1);
	}

public:

	irr::f32 FarLink;

};

//! Shadows shader callback
class ICP3DHandlerShadowsCallback {

public:

	//! Constructor
	ICP3DHandlerShadowsCallback()
	{ }

	//! Destructor
	virtual ~ICP3DHandlerShadowsCallback()
	{ }

	//! OnSetConstants
	virtual inline void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
		irr::video::IVideoDriver* driver = services->getVideoDriver();

		#ifdef _IRR_COMPILE_WITH_DIRECT3D_11_
		if (driver->getDriverType() == irr::video::EDT_DIRECT3D11) {
			irr::core::matrix4 worldTransposed = driver->getTransform(irr::video::ETS_WORLD);
			worldTransposed = worldTransposed.getTransposed();
			services->setVertexShaderConstant("mWorldTrans", worldTransposed.pointer(), 16);
		}
		#endif

		irr::core::matrix4 worldViewProj2 = ProjLink;
		worldViewProj2 *= ViewLink;
		worldViewProj2 *= driver->getTransform(irr::video::ETS_WORLD);
		services->setVertexShaderConstant("mWorldViewProj2", worldViewProj2.pointer(), 16);

		driver->getTransform(irr::video::ETS_WORLD).getInverse(invWorld);
		irr::core::vector3df lightPosOS = LightLink;
		invWorld.transformVect(lightPosOS);
		services->setVertexShaderConstant("LightPos", reinterpret_cast<irr::f32*>(&lightPosOS.X), 3);

		services->setVertexShaderConstant("MaxD", reinterpret_cast<irr::f32*>(&FarLink), 1);
		services->setVertexShaderConstant("MAPRES", &MapRes, 1);

		services->setPixelShaderConstant("LightColour", reinterpret_cast<irr::f32*>(&LightColour.r), 4);
	}

public:

	irr::core::matrix4 invWorld;
	irr::video::SColorf LightColour;
	irr::core::matrix4 ProjLink;
	irr::core::matrix4 ViewLink;
	irr::core::vector3df LightLink;
	irr::f32 FarLink, MapRes;

};

//! Custom Callback
class ICP3DHandlerCustomCallback : public irr::video::IShaderConstantSetCallBack {
	friend class CCP3DHandler;
public:

	//! Constructor
	ICP3DHandlerCustomCallback(const bool computeDepth, const bool computeShadows) : PassType(ECHPT_NONE)
	{
		DepthCallback = (computeDepth) ? new ICP3DHandlerDepthCallback() : 0;
		ShadowsCallback = (computeShadows) ? new ICP3DHandlerShadowsCallback() : 0;
	}

	//! Destructor
	virtual ~ICP3DHandlerCustomCallback() {
		delete DepthCallback;
		delete ShadowsCallback;
	}

	//! Sets constants to the active material
	//! Should be use by internel engine
	virtual inline void OnSetConstants(irr::video::IMaterialRendererServices* services, irr::s32 userData) {
		if (PassType == ECHPT_NONE)
			return;

		if (PassType == ECHPT_DEPTH && DepthCallback)
			DepthCallback->OnSetConstants(services, userData);
		else if (PassType == ECHPT_SHADOWS && ShadowsCallback)
			ShadowsCallback->OnSetConstants(services, userData);
	}

	//! Returns the depth callback
	inline ICP3DHandlerDepthCallback *getDepthCallback() { return DepthCallback; }

	//! Returns the shadows callback
	inline ICP3DHandlerShadowsCallback *getShadowsCallback() { return ShadowsCallback; }

private:

	ICP3DHandlerDepthCallback *DepthCallback;
	ICP3DHandlerShadowsCallback *ShadowsCallback;

protected:

	//! Pass Type
	E_CP3D_HANDLER_PASS_TYPE PassType;
};

} /// End rendering namespace
} /// End cp3d namespace

#endif
