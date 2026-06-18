if(NOT DEFINED EXE_PATH)
    message(FATAL_ERROR "EXE_PATH is not defined")
endif()

if(NOT DEFINED OUT_DIR)
    message(FATAL_ERROR "OUT_DIR is not defined")
endif()

find_program(LDD_EXECUTABLE
    NAMES mingw-ldd.exe mingw-ldd ldd.exe ldd 
    PATHS
        "$ENV{MSYS2_ROOT}/ucrt64/bin"
        "$ENV{MSYS2_ROOT}/usr/bin"
    NO_DEFAULT_PATH
)

execute_process(
    COMMAND "${LDD_EXECUTABLE}" "${EXE_PATH}"
    OUTPUT_VARIABLE LDD_OUTPUT
    ERROR_VARIABLE LDD_ERROR
    RESULT_VARIABLE LDD_RESULT
)

if(NOT LDD_RESULT EQUAL 0)
    message(WARNING "ldd failed: ${LDD_ERROR}")
    return()
endif()

string(REGEX MATCHALL "([A-Za-z]:)?[/\\\\][^ \t\r\n]*ucrt64[/\\\\]bin[/\\\\][^ \t\r\n]+\\.dll" DLL_PATHS "${LDD_OUTPUT}")

list(REMOVE_DUPLICATES DLL_PATHS)

foreach(DLL_PATH IN LISTS DLL_PATHS)

    # Convert MSYS2 path to real Windows path.
    if(DLL_PATH MATCHES "^/ucrt64/bin/")
        if(DEFINED ENV{MSYS2_ROOT})
            file(TO_CMAKE_PATH "$ENV{MSYS2_ROOT}" MSYS2_ROOT_CMAKE)
            string(REPLACE "/ucrt64/bin" "${MSYS2_ROOT_CMAKE}/ucrt64/bin" DLL_PATH "${DLL_PATH}")
        else()
            message(WARNING "MSYS2_ROOT is not set; cannot convert ${DLL_PATH}")
            continue()
        endif()
    endif()

    # Normalize backslashes just in case.
    file(TO_CMAKE_PATH "${DLL_PATH}" DLL_PATH)
    file(COPY "${DLL_PATH}" DESTINATION "${OUT_DIR}")
endforeach()