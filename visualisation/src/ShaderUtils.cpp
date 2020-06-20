#include "ShaderUtils.h"

#include <bgfx/bgfx.h>
#include <fstream>


namespace F9Sim {
namespace Graphics {


bgfx::ShaderHandle loadShader(const char* filename) {
    char* data = new char[2048];
    std::ifstream file;
    size_t fileSize;

    file.open(filename);

    if(file.is_open()) {
        file.seekg(0, std::ios::end);
        fileSize = file.tellg();
        file.seekg(0, std::ios::beg);
        file.read(data, fileSize);
        file.close();
    }

    const bgfx::Memory* mem = bgfx::copy(data, fileSize + 1);
    mem->data[mem->size - 1] = '\0';
    bgfx::ShaderHandle handle = bgfx::createShader(mem);
    bgfx::setName(handle, filename);

    return handle;
}


} // namespace Graphics
} // namespace F9Sim
