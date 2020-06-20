#ifndef F9SIM_GRAPHICS_SHADERUTILS_H_
#define F9SIM_GRAPHICS_SHADERUTILS_H_
#pragma once


namespace bgfx {
struct ShaderHandle;
}

namespace F9Sim {
namespace Graphics {


bgfx::ShaderHandle loadShader(const char* filename);
}
} // namespace F9Sim


#endif F9SIM_GRAPHICS_SHADERUTILS_H_

