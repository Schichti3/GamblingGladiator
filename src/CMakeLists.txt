add_executable(gambling-gladiator
  main.cpp
)

target_link_libraries(gambling-gladiator
  PRIVATE
    PkgConfig::RAYLIB
    PkgConfig::ENET
)

target_compile_options(gambling-gladiator
  PRIVATE
    $<$<CXX_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic>
)

install(TARGETS gambling-gladiator
  RUNTIME DESTINATION bin
)
