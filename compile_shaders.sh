#!/bin/bash

echo "Compiling shaders..."

glslc shaders/simple_shader.vert -o shaders/simple_shader_v.spv
if [ $? -ne 0 ]; then
    echo "ERROR: Vertex shader compilation failed"
    exit 1
fi
echo "Vertex shader compiled successfully"

glslc shaders/simple_shader.frag -o shaders/simple_shader_f.spv
if [ $? -ne 0 ]; then
    echo "ERROR: Fragment shader compilation failed"
    exit 1
fi
echo "Fragment shader compiled successfully"

echo "All shaders compiled successfully"