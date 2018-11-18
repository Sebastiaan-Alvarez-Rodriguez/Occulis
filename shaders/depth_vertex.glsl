#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vertexPosition_modelspace;

// Values that stay constant for the whole mesh.
uniform mat4 cam_model;
uniform mat4 cam_view;
uniform mat4 cam_projection;

void main(){
    mat4 depthMVP = cam_projection * cam_view * cam_model;
    gl_Position =  depthMVP * vertexPosition_modelspace;
}