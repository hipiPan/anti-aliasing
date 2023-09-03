#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;
layout(location = 0) out vec3 out_normal;
layout(location = 1) out vec2 out_texcoord;
#ifdef MOTION_VECTORS
layout(location = 2) out vec4 screen_position;
layout(location = 3) out vec4 prev_screen_position;
#endif

layout(std140, set = 0, binding = 0) uniform SceneBuffer
{
    mat4 transform;
} scene_buffer;

struct ViewData
{
    mat4 view_matrix;
    mat4 proj_matrix;
    vec4 view_position;
    vec2 taa_jitter;
};

layout(std140, set = 0, binding = 1) uniform ViewBuffer
{
    ViewData cur;
    ViewData prev;
} view_buffer;

void main() 
{
    #ifdef MOTION_VECTORS
    prev_screen_position = view_buffer.prev.proj_matrix * view_buffer.prev.view_matrix * scene_buffer.transform * vec4(in_position, 1.0);
    screen_position = view_buffer.cur.proj_matrix * view_buffer.cur.view_matrix * scene_buffer.transform * vec4(in_position, 1.0);
    gl_Position = screen_position;
    #else
    gl_Position = view_buffer.cur.proj_matrix * view_buffer.cur.view_matrix * scene_buffer.transform * vec4(in_position, 1.0);
    #endif
    out_texcoord = in_texcoord;
    out_normal = mat3(transpose(inverse(scene_buffer.transform))) * in_normal;
}