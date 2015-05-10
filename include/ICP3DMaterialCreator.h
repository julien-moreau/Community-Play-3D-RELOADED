#ifndef _H_ICP3D_MATERIAL_CREATOR_INCLUDED__
#define _H_ICP3D_MATERIAL_CREATOR_INCLUDED__

#include <irrlicht.h>

namespace cp3d {
namespace rendering {

class ICP3DMaterialCreator {
public:

	//! Creates a material from files (vertex & pixel)
	//! \param vertexFilename: the path to the vertex shader file
	//! \param pixelFilename: the path to the pixel shader file
	//! \param baseMaterial: the base material of the created material
	//! \param callback: the material's callback
	virtual irr::s32 createMaterialFromFiles(const irr::core::stringc &vertexFilename, const irr::core::stringc &pixelFilename,
											 irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
											 irr::video::IShaderConstantSetCallBack *callback = 0) = 0;
	//! Creates a material from strings (vertex & pixel)
	//! \param vertexShader: the string containing the vertex shader code
	//! \param pixelShader: the string containing the pixel shader code
	//! \param baseMaterial: the base material of the created material
	//! \param callback: the material's callback
	virtual irr::s32 createMaterialFromStrings(const irr::core::stringc &vertexShader, const irr::core::stringc &pixelShader,
											   irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
											   irr::video::IShaderConstantSetCallBack *callback = 0) = 0;

	//! Creates a custom depth material from files (vertex & pixel)
	//! \param vertexShader: the string containing the vertex shader code
	//! \param pixelShader: the string containing the pixel shader code
	//! \param baseMaterial: the base material of the created material
	//! \param callback: the material's callback
	virtual irr::s32 createCustomDepthMaterialFromFiles(const irr::core::stringc &vertexFilename, const irr::core::stringc &pixelFilename,
														irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
														irr::video::IShaderConstantSetCallBack *callback = 0) = 0;
	//! Creates a custom depth material from strings (vertex & pixel)
	//! \param vertexShader: the string containing the vertex shader code
	//! \param pixelShader: the string containing the pixel shader code
	//! \param baseMaterial: the base material of the created material
	//! \param callback: the material's callback
	virtual irr::s32 createCustomDepthMaterialFromStrings(const irr::core::stringc &vertexShader, const irr::core::stringc &pixelShader,
														  irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
														  irr::video::IShaderConstantSetCallBack *callback = 0) = 0;

	//! Creates a custom shadows material from file (vertex)
	//! \param vertexFilename: the string containing the vertex filename
	//! \param baseMaterial: the base material of the created material
	//! \param callback: the material's callback
	virtual irr::s32 createCustomShadowsMaterialFromFile(const irr::core::stringc &vertexFilename,
														 irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
														 irr::video::IShaderConstantSetCallBack *callback = 0) = 0;

	//! Creates a custom shadows material from string (vertex)
	//! \param vertexShader: the string containing the vertex shader code
	//! \param baseMaterial: the base material of the created material
	//! \param callback: the material's callback
	virtual irr::s32 createCustomShadowsMaterialFromString(const irr::core::stringc &vertexShader,
															irr::video::E_MATERIAL_TYPE baseMaterial = irr::video::EMT_SOLID,
															irr::video::IShaderConstantSetCallBack *callback = 0) = 0;

	//! Adds a define for the conditional compilation
	//! Example:
	/*
		if (driver->getDriverType() == EDT_OPENGL)
			materialCreator->addDefine("OPENGL_DRIVER", "1");
	*/
	//! \param define: the define
	//! \param value: the define's value
	virtual void addDefine(const irr::core::stringc define, const irr::core::stringc value) = 0;

	//! Removes a shader define
	//! \param define: the define to remove
	virtual void removeDefine(const irr::core::stringc define) = 0;

	//! Clears the defined defines
	virtual void clearDefines() = 0;

};

} /// End rendering namespace
} /// End cp3d namespace

#endif