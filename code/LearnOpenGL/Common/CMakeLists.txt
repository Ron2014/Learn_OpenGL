fips_begin_lib(Common)
fips_vs_disable_warnings(4819 4006 4312)
add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
# add_definitions(-DSTBIDEF)
fips_files_ex(. *.cc GROUP "SOURCES")
fips_files_ex(. *.h GROUP "HEADERS")
fips_deps(glfw glad assimp)
fips_end_lib()