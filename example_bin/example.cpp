#include <renderer/pipeline.hpp>
#include <fstream>
#include <format>
#include "tools/cpp/runfiles/runfiles.h"
#include <thread>

using bazel::tools::cpp::runfiles::Runfiles;

std::vector<const char*> instanceExtensions={
    VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
    VK_KHR_SURFACE_EXTENSION_NAME,
    "VK_KHR_win32_surface",
};

std::vector<const char*> instanceLayers={
    "VK_LAYER_KHRONOS_validation"
};

std::vector<const char*> deviceExtensions={
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

const std::vector<Vertex> vertices = {
    {{0.0f, -0.5f}, {1.0f, 1.0f, 1.0f}},
    {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
    {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
};

static std::vector<char> readFile(const std::string& path){
    std::ifstream file(path,std::ios::binary | std::ios::ate);
    if(!file.is_open()){
        std::runtime_error(std::format("Failed to open file: {}",errno));
    }

    int size=file.tellg();
    std::vector<char> buffer(size);
    file.seekg(0,std::ios::beg);
    file.read(buffer.data(),size);
    file.close();
    return buffer;
}

int main(int argc, char** argv){
    std::string error;
    std::unique_ptr<Runfiles> runfiles(Runfiles::Create(argv[0], &error));
    std::string vertPath = runfiles->Rlocation("_main/example_bin/data/shaders/vert.spv");
    std::string fragPath = runfiles->Rlocation("_main/example_bin/data/shaders/frag.spv");
    GLFWwindow* handle=vo::create::window(600,500,"Vulkan");
    vk::raii::Context context{};
    vk::raii::Instance instance=vo::create::instance(
        context,
        "Vulkan",
        "No engine",
        instanceLayers,
        instanceExtensions
    );
    vk::raii::PhysicalDevice physicalDevice = vo::create::physicalDevice(instance);
    QueueFamily family=vo::utils::findQueueFamily(physicalDevice);
    vk::raii::Device device=vo::create::logicalDevice(
        physicalDevice, family, {}, deviceExtensions
    );
    vk::raii::Queue graphicsQueue=vo::create::queue(device, family);
    vk::raii::SurfaceKHR surface=vo::create::surface(instance, handle);
    SwapchainInfo swapchainInfo=vo::utils::querySwapChainInfo(
        physicalDevice,
        device,
        surface,
        handle
    );
    ImageInfo imageInfo={
        swapchainInfo.surfaceFormat.format,
        swapchainInfo.extent.width,
        swapchainInfo.extent.height
    };
    std::vector<vk::Image> images=vo::create::images(swapchainInfo.swapchain);
    std::vector<vk::raii::ImageView> imageViews=vo::create::imageViews(
        device,images,swapchainInfo.surfaceFormat.format
    );

    vk::raii::Buffer vertexBuffer=vo::create::vertexbuffer(device, vertices);
    vk::MemoryRequirements memRequirements=vertexBuffer.getMemoryRequirements();
    uint32_t memType=vo::utils::findMemoryType(
        physicalDevice,
        memRequirements.memoryTypeBits,
        vk::MemoryPropertyFlagBits::eHostVisible |
        vk::MemoryPropertyFlagBits::eHostCoherent
    );

    vk::raii::DeviceMemory memory=vo::utils::allocateBuffer(device,memRequirements,memType);
    vo::utils::fillBuffer(vertexBuffer,memory,memRequirements,vertices);
    auto vertexCode=readFile(vertPath);
    auto fragmentCode=readFile(fragPath);
    vk::raii::ShaderModule vertexShaderModule=vo::create::shaderModule(device,vertexCode);
    vk::raii::ShaderModule fragmentShaderModule=vo::create::shaderModule(device,fragmentCode);
    vk::raii::RenderPass renderpass=vo::create::renderpass(device,imageInfo);
    vk::raii::PipelineLayout layout=vo::create::layout(device);
    vk::raii::Pipeline pipeline=vo::create::pipeline(
        device, vertexShaderModule,fragmentShaderModule, renderpass, 
        layout,swapchainInfo
    );
    std::vector<vk::raii::Framebuffer> framebuffers=vo::create::framebuffers(
        device,
        renderpass,
        imageViews,
        imageInfo
    );
    vk::raii::CommandPool pool=vo::create::commandpool(device,family);
    vk::raii::CommandBuffer commandbuffer=vo::create::commandbuffer(device,pool);

    vk::raii::Semaphore imageAcquiredSemaphore(device,vk::SemaphoreCreateInfo());
    vk::raii::Semaphore renderFinishedSemaphore(device,vk::SemaphoreCreateInfo());
    vk::raii::Fence fence(device,vk::FenceCreateInfo(vk::FenceCreateFlagBits::eSignaled));


    while(!glfwWindowShouldClose(handle)){
        glfwPollEvents();
        auto [waitRes,presentRes]=vo::utils::drawFrame(
            device,
            swapchainInfo,
            renderpass,
            pipeline,
            commandbuffer,
            imageAcquiredSemaphore,
            renderFinishedSemaphore,
            fence,
            framebuffers,
            graphicsQueue,
            vertexBuffer,
            vertices.size()
        );
        device.waitIdle();
    }
}