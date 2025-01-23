#include "vkf.hpp"

int main(int argc, char** argv)
{
    vkf::WindowManager::globalInit();

    vkf::InstanceManager instanceManager{};
    instanceManager.bestPhysicalDevice();

    vkf::WindowManager windowManager{800, 600};
    windowManager.loop();

    vkf::WindowManager::globalDestroy();
}
