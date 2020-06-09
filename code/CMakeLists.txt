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

fips_setup()
fips_add_subdirectory(Ext)
fips_add_subdirectory(Lab)
fips_add_subdirectory(ComputerGraphicsWithOpenGL.3e)
fips_add_subdirectory(LearnOpenGL)
fips_finish()


