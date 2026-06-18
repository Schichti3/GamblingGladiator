SET(Name "gambling-gladiator")

add_executable(${Name}
  main.c
)

target_link_libraries(${Name}
  PRIVATE
    PkgConfig::RAYLIB
    gambling-gladiator-client
    gambling-gladiator-server
)

target_compile_options(${Name}
  PRIVATE
    $<$<C_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic>
)

if(WIN32 AND MINGW)
    add_custom_command(TARGET ${Name} POST_BUILD
        COMMAND ${CMAKE_COMMAND}
            -DEXE_PATH="$<TARGET_FILE:${Name}>"
            -DOUT_DIR="$<TARGET_FILE_DIR:${Name}>"
            -P "${CMAKE_SOURCE_DIR}/cmake/CopyMsys2RuntimeDeps.cmake"
        COMMENT "Copying MSYS2/UCRT64 runtime DLLs"
    )
endif()

install(TARGETS ${Name}
  RUNTIME DESTINATION bin
)

if(WIN32 AND MINGW)
    install(DIRECTORY "$<TARGET_FILE_DIR:${Name}>/"
        DESTINATION bin
        FILES_MATCHING
            PATTERN "*.dll"
    )
endif()