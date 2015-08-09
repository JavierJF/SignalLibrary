set(EXTRA_C_FLAGS "")
set(EXTRA_C_FLAGS_RELEASE "")
set(EXTRA_C_FLAGS_DEBUG "")
set(EXTRA_EXE_LINKER_FLAGS "")
set(EXTRA_EXE_LINKER_FLAGS_RELEASE "")
set(EXTRA_EXE_LINKER_FLAGS_DEBUG "")

if (UNIX)
        if (BUILD_ANDROID)
                set (CMAKE_C_FLAGS_RELEASE          "${CMAKE_C_FLAGS_RELEASE} -O3")
                set (CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wl,--no-warn-mismatch")
                set (CMAKE_CXX_FLAGS_DEBUG          "${CMAKE_CXX_FLAGS_DEBUG} -g")
                set (CMAKE_CXX_FLAGS_RELEASE        "${CMAKE_CXX_FLAGS_RELEASE} -O3")
        elseif (USE_CLANG)
                set (CMAKE_C_FLAGS_RELEASE          "-O3")
                set (CMAKE_CXX_FLAGS                "")
                set (CMAKE_CXX_FLAGS_DEBUG          "-g -O0")
                set (CMAKE_CXX_FLAGS_RELEASE        "-O3")
        else()
                set (CMAKE_C_FLAGS_RELEASE          "-O3")
                set (CMAKE_CXX_FLAGS                "")
                set (CMAKE_CXX_FLAGS_DEBUG          "-g")
                set (CMAKE_CXX_FLAGS_RELEASE        "-O3")
        endif()
elseif (MSVC)
        string(REGEX REPLACE "^  *| * $" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        string(REGEX REPLACE "^  *| * $" "" CMAKE_CXX_FLAGS_INIT "${CMAKE_CXX_FLAGS_INIT}")
        if(CMAKE_CXX_FLAGS STREQUAL CMAKE_CXX_FLAGS_INIT)
                # override cmake default exception handling option
                string(REPLACE "/EHsc" "/EHa" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
                set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}"  CACHE STRING "Flags used by the compiler during all build types." FORCE)
        endif()
endif()


if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")

        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wall -Wextra -std=c++1y -Wno-deprecated-declarations")

        set(EXTRA_C_FLAGS_RELEASE "-DNDEBUG ${EXTRA_C_FLAGS_RELEASE}")

        if(WARNINGS_ANSI_ISO)
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -pedantic")
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-unknown-pragmas")
        else()
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-attributes -Wno-unused-parameter")
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-unused-local-typedefs -Wno-maybe-uninitialized")
        endif()

        if(WARNINGS_ARE_ERRORS)
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Werror")
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Werror=return-type")
        endif()

endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)

		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -mtune=native")
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wnon-virtual-dtor")
		set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fdevirtualize-speculatively")

        execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
        if (NOT (GCC_VERSION VERSION_GREATER 4.8 OR GCC_VERSION VERSION_EQUAL 4.8))
                message(FATAL_ERROR "${PROJECT_NAME} c++1y support requires g++ 4.8 or greater.")
        endif ()

        if (GCC_VERSION VERSION_GREATER 4.9 OR GCC_VERSION VERSION_EQUAL 4.9)
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fdiagnostics-color=always")
		endif()

        if(WARNINGS_ANSI_ISO)
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wstrict-aliasing=3")
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wreturn-type")
        else()
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -Wno-narrowing -Wno-delete-non-virtual-dtor")
        endif()

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)

        if(NOT EXISTS ${CMAKE_CXX_COMPILER})
                message( FATAL_ERROR "Clang++ not found. " )
        endif()

        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} -fcolor-diagnostics")  # color output for clang, even if piped to less

elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL MSVC)

        if(NOT PLATFORM MATCHES "64")
                message(FATAL_ERROR "Windows tool chain requires MSVC x64 compiler")
        endif()

        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /wd4503 /W3")
        set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /Gy") # Remove unreferenced functions: function level linking

        set(EXTRA_C_FLAGS_RELEASE "/MD /O2 /Ob2 /DNDEBUG") # Overriding default release flags

        set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG} /DEBUG")

        set(EXTRA_EXE_LINKER_FLAGS "${EXTRA_EXE_LINKER_FLAGS} /ignore:4049 /ignore:4217 /ignore:4099 /INCREMENTAL:NO")
        set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE} /DEBUG /OPT:REF /OPT:ICF /NODEFAULTLIB:libcmt")
        set(EXTRA_EXE_LINKER_FLAGS_DEBUG "${EXTRA_EXE_LINKER_FLAGS_DEBUG} /OPT:NOREF /OPT:NOICF /NODEFAULTLIB:MSVCRT")

        if(NOT MSVC_VERSION LESS 1400) # From VS 9.0 in advance
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /bigobj")
        endif()

        if(WARNINGS_ARE_ERRORS)
                set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS} /WX")
        endif()

else()

        message(FATAL_ERROR "${CMAKE_CXX_COMPILER_ID} compiler not supported in this project.")

endif()


# ==========================================================
# Add user supplied extra options (optimization, etc...)
# ==========================================================
set(EXTRA_C_FLAGS "${EXTRA_C_FLAGS}" CACHE INTERNAL "Extra compiler options")
set(EXTRA_C_FLAGS_RELEASE "${EXTRA_C_FLAGS_RELEASE}" CACHE INTERNAL "Extra compiler options for Release build")
set(EXTRA_C_FLAGS_DEBUG "${EXTRA_C_FLAGS_DEBUG}" CACHE INTERNAL "Extra compiler options for Debug build")
set(EXTRA_EXE_LINKER_FLAGS "${EXTRA_EXE_LINKER_FLAGS}" CACHE INTERNAL "Extra linker flags")
set(EXTRA_EXE_LINKER_FLAGS_RELEASE "${EXTRA_EXE_LINKER_FLAGS_RELEASE}" CACHE INTERNAL "Extra linker flags for Release build")
set(EXTRA_EXE_LINKER_FLAGS_DEBUG "${EXTRA_EXE_LINKER_FLAGS_DEBUG}" CACHE INTERNAL "Extra linker flags for Debug build")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_C_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${EXTRA_C_FLAGS_RELEASE}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${EXTRA_C_FLAGS_DEBUG}")

set(CMAKE_EXE_LINKER_FLAGS         "${CMAKE_EXE_LINKER_FLAGS} ${EXTRA_EXE_LINKER_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${EXTRA_EXE_LINKER_FLAGS_RELEASE}")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${EXTRA_EXE_LINKER_FLAGS_DEBUG}")
