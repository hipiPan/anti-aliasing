#pragma once
#include <rhi/ez_vulkan.h>
#include <glm/glm.hpp>

class Scene;
class Camera;

struct SceneBufferType
{
    glm::mat4 transform;
    glm::mat4 pad0;
    glm::mat4 pad1;
    glm::mat4 pad2;
};

struct ViewBufferType
{
    glm::mat4 view_matrix;
    glm::mat4 proj_matrix;
    glm::mat4 prev_view_matrix;
    glm::mat4 prev_proj_matrix;
    glm::vec2 taa_jitter;
    glm::vec2 prev_taa_jitter;
};

class Renderer
{
public:
    enum AA
    {
        NONE,
        MSAA,
        FXAA,
        TAA,
    };

    Renderer();

    ~Renderer();

    void render(EzSwapchain swapchain);

    void set_scene(Scene* scene);

    void set_camera(Camera* camera);

    void set_aa(AA aa);

private:
    void update_rendertarget();

    void update_scene_buffer();

    void update_view_buffer();

    AA _aa = AA::NONE;
    uint32_t _width = 0;
    uint32_t _height = 0;
    uint64_t _frame_number = 0;
    Scene* _scene;
    bool _scene_dirty = true;
    Camera* _camera;
    glm::mat4 _last_view_matrix;
    glm::mat4 _last_proj_matrix;
    glm::vec2 _last_taa_jitter;
    EzBuffer _scene_buffer = VK_NULL_HANDLE;
    EzBuffer _view_buffer = VK_NULL_HANDLE;
    EzTexture _color_rt = VK_NULL_HANDLE;
    EzTexture _depth_rt = VK_NULL_HANDLE;
    EzTexture _velocity_rt = VK_NULL_HANDLE;
    EzTexture _resolve_rt = VK_NULL_HANDLE;
    EzTexture _post_rt = VK_NULL_HANDLE;
    friend class BasePass;
    BasePass* _base_pass;
    friend class FXAAPass;
    FXAAPass* _fxaa;
    friend class TAAPass;
    TAAPass* _taa;
};