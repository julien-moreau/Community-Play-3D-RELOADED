#ifndef H_C_SHADER_PP
#define H_C_SHADER_PP

#include <irrlicht.h>
#include <iostream>

namespace cp3d {
namespace rendering {

class CShaderPreprocessor {
public:
	CShaderPreprocessor(irr::video::IVideoDriver* driverIn);

	irr::core::stringc ppShader(irr::core::stringc shaderProgram);
	irr::core::stringc ppShaderFF(irr::core::stringc shaderProgram);

	void addShaderDefine(const irr::core::stringc name, const irr::core::stringc value = "");
	void removeShaderDefine(const irr::core::stringc name);
	void clearDefines();

	std::string getFileContent(const std::string pFile);

private:
	void initDefineMap();

	irr::video::IVideoDriver* driver;
	irr::core::map<irr::core::stringc , irr::core::stringc> DefineMap;

};

} /// End namespace rendering
} /// End namespace cp3d

#endif
