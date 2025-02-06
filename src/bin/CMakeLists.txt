add_executable(vkg-bin
        main.cpp
)

target_link_libraries(vkg-bin PRIVATE
        vkg-lib
)

copy_dlls_if_needed(vkg-bin)
