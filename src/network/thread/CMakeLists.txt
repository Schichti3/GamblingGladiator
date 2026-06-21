SET(Name "thread")

add_library(${Name}
  STATIC
  thread.c
)

target_include_directories(${Name}
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ../include
)

if(UNIX AND NOT APPLE)
  target_link_libraries(${Name} PUBLIC pthread)
endif()

target_compile_options(${Name}
  PRIVATE
    $<$<C_COMPILER_ID:Clang,GNU>:-Wall -Wextra -Wpedantic>
)

install(TARGETS ${Name}
  ARCHIVE DESTINATION lib
)

install(FILES
  thread.h
  DESTINATION include/gambling-gladiator/thread
)
