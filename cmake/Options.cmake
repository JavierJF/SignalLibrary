# Build type
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)
if(DEFINED CMAKE_BUILD_TYPE)
   SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES})
endif()

if (NOT CMAKE_BUILD_TYPE)
        message( WARNING "Release build mode selected by default")
        set(CMAKE_BUILD_TYPE "Release")
endif()

option(INSTALL_DOC              "Install documentation in system"                                       OFF)

set(CMAKE_INCLUDE_DIRS_CONFIGCMAKE "${CMAKE_INSTALL_PREFIX}/include")
set(CMAKE_LIB_DIRS_CONFIGCMAKE     "${CMAKE_INSTALL_PREFIX}/lib")
