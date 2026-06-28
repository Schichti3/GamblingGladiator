SET(Name "gambling-gladiator-server")

add_library(${Name}
  STATIC
  server.c
  server_config.c
)

target_include_directories(${Name}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../include
)

target_link_libraries(${Name}
  PUBLIC
    thread
    PkgConfig::ENET
)

target_compile_options(${Name}
  PRIVATE
    $<$<C_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic>
)

install(TARGETS ${Name}
  ARCHIVE DESTINATION lib
)

install(FILES
  server_config.h
  server.h
  DESTINATION include/gambling-gladiator/server
)
