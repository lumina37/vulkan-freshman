#include "vkf.hpp"

int main(int argc, char** argv) {
    vkf::WindowManager::globalInit();

    vkf::InstanceManager instanceManager;
    vkf::PhyDeviceManager phyDeviceManager{instanceManager};
    vkf::DeviceManager deviceManager{phyDeviceManager};

    vkf::WindowManager windowManager{640, 480};
    windowManager.loop();

    vkf::WindowManager::globalDestroy();
}
