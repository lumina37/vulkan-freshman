add_library(vkg-lib INTERFACE)

target_compile_features(vkg-lib INTERFACE
        cxx_std_23
)

target_compile_definitions(vkg-lib INTERFACE
        GLFW_INCLUDE_VULKAN
)

target_link_libraries(vkg-lib INTERFACE
        Vulkan::Vulkan
        glfw
)

target_include_directories(vkg-lib INTERFACE
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src/include>
        $<INSTALL_INTERFACE:include>
)

add_dependencies(vkg-lib vkg-shader)
