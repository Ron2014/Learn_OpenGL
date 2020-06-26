#
# project: code
#
cmake_minimum_required(VERSION 2.8)
project(code)
# include the fips main cmake file
get_filename_component(FIPS_ROOT_DIR "../fips" ABSOLUTE)
include("${FIPS_ROOT_DIR}/cmake/fips.cmake")

# include_directories(src)
fips_include_directories(Ext/glfw-3.3.2/include)
fips_include_directories(Ext/glad/include)
fips_include_directories(Ext/freeglut-3.2.1/include)
fips_include_directories(Ext/glm)
fips_include_directories(Ext/assimp-5.0.1/include)

fips_setup()

# POSIX platform defines
if (FIPS_POSIX)
    add_definitions(-DFUTURE_POSIX=1)
    if (FIPS_LINUX)
        add_definitions(-DFUTURE_LINUX=1)
    endif()
    if (FIPS_OSX)
        add_definitions(-DFUTURE_OSX=1)
        if (FIPS_MACOS)
            add_definitions(-DFUTURE_OSX=1)
        endif()
        if (FIPS_IOS)
            add_definitions(-DFUTURE_IOS=1)
        endif()
    endif()
    if (FIPS_ANDROID)
        add_definitions(-DFUTURE_ANDROID=1)
    endif()
endif()

# Windows platform defines
if (FIPS_WINDOWS)
    add_definitions(-DFUTURE_WINDOWS=1)
    if (FIPS_WIN32)
        add_definitions(-DFUTURE_WIN32=1)
    endif()
    if (FIPS_WIN64)
        add_definitions(-DFUTURE_WIN64=1)
    endif()
endif()

fips_add_subdirectory(Ext)
if(FIPS_WINDOWS)
fips_add_subdirectory(Lab)
fips_add_subdirectory(ComputerGraphicsWithOpenGL.3e)
endif()
fips_add_subdirectory(LearnOpenGL)
fips_finish()


