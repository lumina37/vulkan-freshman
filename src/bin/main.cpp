#include "vkf.hpp"

int main(int argc, char** argv) {
    vk::Extent2D extent{640, 480};

    vkf::WindowManager::globalInit();

    vkf::InstanceManager instanceManager;
    vkf::PhyDeviceManager phyDeviceManager{instanceManager};
    vkf::WindowManager windowManager{extent};
    vkf::SurfaceManager surfaceManager{instanceManager, windowManager};
    vkf::QueueFamilyManager queueFamilyManager{instanceManager, phyDeviceManager, surfaceManager};
    vkf::DeviceManager deviceManager{phyDeviceManager, queueFamilyManager};
    vkf::SwapChainManager swapchainManager{deviceManager, surfaceManager, queueFamilyManager, extent};

    windowManager.loop();

    vkf::WindowManager::globalDestroy();
}
