#!/bin/bash

echo "Compiling shaders..."

glslc shaders/simple_shader.vert -o shaders/simple_shader_v.spv
if [ $? -ne 0 ]; then
    echo "ERROR: Vertex shader compilation failed"
    exit 1
fi

glslc shaders/simple_shader_debug.vert -o shaders/simple_shader_debug_v.spv
if [ $? -ne 0 ]; then
    echo "ERROR: Vertex shader compilation failed"
    exit 1
fi

echo "Vertex shaders compiled successfully"

glslc shaders/simple_shader.frag -o shaders/simple_shader_f.spv
if [ $? -ne 0 ]; then
    echo "ERROR: Fragment shader compilation failed"
    exit 1
fi

glslc shaders/simple_shader_debug.frag -o shaders/simple_shader_debug_f.spv
if [ $? -ne 0 ]; then
    echo "ERROR: Fragment shader compilation failed"
    exit 1
fi
echo "Fragment shaders compiled successfully"

echo "All shaders compiled successfully"