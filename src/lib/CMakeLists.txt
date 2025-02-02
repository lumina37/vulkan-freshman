add_library(vkf-lib INTERFACE)

target_compile_features(vkf-lib INTERFACE
        cxx_std_23
)

target_compile_definitions(vkf-lib INTERFACE
        GLFW_INCLUDE_VULKAN
)

target_link_libraries(vkf-lib INTERFACE
        Vulkan::Vulkan
        glfw
)

target_include_directories(vkf-lib INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/include>
        $<INSTALL_INTERFACE:include>
)

add_dependencies(vkf-lib vkf-shader)
