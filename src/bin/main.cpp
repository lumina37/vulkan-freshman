#include "vkg.hpp"

int main(int argc, char** argv) {
    vk::Extent2D extent{640, 480};

    vkg::WindowManager::globalInit();

    vkg::InstanceManager instMgr;
    vkg::PhyDeviceManager phyDeviceMgr{instMgr};
    vkg::WindowManager windowMgr{extent};
    vkg::SurfaceManager surfaceMgr{instMgr, windowMgr};
    vkg::QueueFamilyManager queueFamilyMgr{phyDeviceMgr, surfaceMgr};
    vkg::DeviceManager deviceMgr{phyDeviceMgr, queueFamilyMgr};
    vkg::QueueManager queueMgr{deviceMgr, queueFamilyMgr};
    vkg::SwapChainManager swapchainMgr{deviceMgr, surfaceMgr, queueFamilyMgr, extent};
    vkg::ShaderManager vertShaderMgr{deviceMgr, "../shader/triangle.vert.spv"};
    vkg::ShaderManager fragShaderMgr{deviceMgr, "../shader/triangle.frag.spv"};
    vkg::RenderPassManager renderPassMgr{deviceMgr};
    vkg::PipelineManager pipelineMgr{deviceMgr, extent, vertShaderMgr, fragShaderMgr, renderPassMgr};
    vkg::ImageManager imageMgr{deviceMgr, extent, swapchainMgr, renderPassMgr};
    vkg::CommandPoolManager commandPoolMgr{deviceMgr};

    vkg::Renderer renderer{deviceMgr,     swapchainMgr, commandPoolMgr, pipelineMgr,
                           renderPassMgr, imageMgr,     extent,         queueMgr};

    // Main Loop
    while (!glfwWindowShouldClose(windowMgr.getWindow())) {
        renderer.render();
        glfwPollEvents();
    }

    vkg::WindowManager::globalDestroy();
}
