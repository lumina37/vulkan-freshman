cmake_minimum_required(VERSION 3.15)

project(VulkanFreshman
        VERSION 0.0.1
        DESCRIPTION "Vulkan triangle with SDL3"
        LANGUAGES CXX
)

# Dependencies
include(FetchContent)

find_package(Vulkan REQUIRED)

FetchContent_Declare(
        SDL
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-3.2.0
)
FetchContent_MakeAvailable(SDL)

# Sources
add_subdirectory(src)
