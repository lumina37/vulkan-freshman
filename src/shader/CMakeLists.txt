function(vkg_add_shader src)
    set(__VKG_IN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${src})
    set(__VKG_OUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${src}.spv)
    add_custom_command(
            OUTPUT ${src}
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${__VKG_IN_PATH} -o ${__VKG_OUT_PATH}
            DEPENDS ${__VKG_IN_PATH}
            COMMENT "Compile shader - ${__VKG_IN_PATH}"
    )
endfunction()

set(_VKG_SHADERS triangle.frag triangle.vert)
foreach (_VKG_SHADER ${_VKG_SHADERS})
    vkg_add_shader(${_VKG_SHADER})
endforeach ()
add_custom_target(vkg-shader DEPENDS ${_VKG_SHADERS})
