SET(Name "os-utils")

add_library(${Name} STATIC
  os_utils.c
)

target_include_directories(${Name}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_options(${Name}
  PRIVATE
    $<$<C_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic>
)

install(TARGETS ${Name}
  RUNTIME DESTINATION bin
)
