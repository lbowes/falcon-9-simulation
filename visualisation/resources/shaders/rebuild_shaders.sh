#!/bin/bash
# This script installs dependencies

# Vertex shader
/usr/local/bin/shaderc -f v_square.sc -o v_square.bin --platform linux --type vertex --verbose -i /usr/local/include/bgfx

# Fragment shader
/usr/local/bin/shaderc -f f_square.sc -o f_square.bin --platform linux --type fragment --verbose -i /usr/local/include/bgfx
