cmake_minimum_required(VERSION 3.15)

project(VulkanGraphicsDemo
        VERSION 0.0.1
        DESCRIPTION "Vulkan graphics demo"
        LANGUAGES CXX
)

# Dependencies
include(FetchContent)

find_package(Vulkan REQUIRED)

FetchContent_Declare(
        glfw
        DOWNLOAD_EXTRACT_TIMESTAMP ON
        URL https://github.com/glfw/glfw/archive/refs/tags/3.4.tar.gz
)
FetchContent_MakeAvailable(glfw)

# Helper
function(copy_dlls_if_needed name)
    if (WIN32)
        add_custom_command(TARGET ${name} POST_BUILD COMMAND ${CMAKE_COMMAND} -E
                copy_if_different $<TARGET_RUNTIME_DLLS:${name}> $<TARGET_FILE:${name}> $<TARGET_FILE_DIR:${name}> COMMAND_EXPAND_LISTS)
    endif ()
endfunction()

# Sources
add_subdirectory(src)
