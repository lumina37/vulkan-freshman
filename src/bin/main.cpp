#include "vkf.hpp"

int main(int argc, char** argv) {
    vk::Extent2D extent{640, 480};

    vkf::WindowManager::globalInit();

    vkf::InstanceManager instMgr;
    vkf::PhyDeviceManager phyDeviceMgr{instMgr};
    vkf::WindowManager windowMgr{extent};
    vkf::SurfaceManager surfaceMgr{instMgr, windowMgr};
    vkf::QueueFamilyManager queueFamilyMgr{instMgr, phyDeviceMgr, surfaceMgr};
    vkf::DeviceManager deviceMgr{phyDeviceMgr, queueFamilyMgr};
    vkf::QueueManager queueMgr{deviceMgr, queueFamilyMgr};
    vkf::SwapChainManager swapchainMgr{deviceMgr, surfaceMgr, queueFamilyMgr, extent};
    vkf::ShaderManager vertShaderMgr{deviceMgr, "../shader/triangle.vert.spv"};
    vkf::ShaderManager fragShaderMgr{deviceMgr, "../shader/triangle.frag.spv"};
    vkf::RenderPassManager renderPassMgr{deviceMgr};
    vkf::PipelineManager pipelineMgr{deviceMgr, extent, vertShaderMgr, fragShaderMgr, renderPassMgr};
    vkf::ImageManager imageMgr{deviceMgr, extent, swapchainMgr, renderPassMgr};
    vkf::CommandPoolManager commandPoolMgr{deviceMgr};
    vkf::CommandBufferManager commandBufferMgr{deviceMgr, commandPoolMgr};

    vkf::Renderer renderer{deviceMgr,     swapchainMgr, commandBufferMgr, pipelineMgr,
                           renderPassMgr, imageMgr,     extent,           queueMgr};

    // Main Loop
    while (!glfwWindowShouldClose(windowMgr.getWindow())) {
        renderer.render();
        glfwPollEvents();
    }

    vkf::WindowManager::globalDestroy();
}
