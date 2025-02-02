function(vkf_add_shader src)
    set(__VKF_IN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    set(__VKF_OUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${src}.spv)
    add_custom_command(
            OUTPUT ${src}
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${__VKF_IN_PATH} -o ${__VKF_OUT_PATH}
            DEPENDS ${__VKF_IN_PATH}
            COMMENT "Generate SPIR-V - ${__VKF_OUT_PATH}"
    )
endfunction()

set(_VKF_SHADERS triangle.frag triangle.vert)
foreach (_VKF_SHADER ${_VKF_SHADERS})
    vkf_add_shader(${_VKF_SHADER})
endforeach ()
add_custom_target(vkf-shader DEPENDS ${_VKF_SHADERS})
