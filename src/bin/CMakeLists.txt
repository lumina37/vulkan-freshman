add_executable(vkf-bin
        main.cpp
)

target_link_libraries(vkf-bin PRIVATE
        vkf-lib
)

copy_dlls_if_needed(vkf-bin)
