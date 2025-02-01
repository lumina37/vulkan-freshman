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
    vkf::SwapChainManager swapchainMgr{deviceMgr, surfaceMgr, queueFamilyMgr, extent};

    windowMgr.loop();

    vkf::WindowManager::globalDestroy();
}
