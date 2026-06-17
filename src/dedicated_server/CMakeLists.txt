SET(Name "gambling-gladiator-decicated-server")

add_executable(${Name}
  main.c
)

target_link_libraries(${Name}
  PRIVATE
    gambling-gladiator-server
)

target_compile_options(${Name}
  PRIVATE
    $<$<C_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic>
)

install(TARGETS ${Name}
  RUNTIME DESTINATION bin
)
