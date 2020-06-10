## Hello Window
fips_include_directories(Common)

fips_begin_app(HelloWindow cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(HelloWindow main.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

## Hello Triangle

fips_begin_app(HelloTriangle cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(HelloTriangle main.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

fips_begin_app(HelloTriangleTwo cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(HelloTriangle occulsion.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

fips_begin_app(HelloTriangleWithEBO cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(HelloTriangle with_ebo.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

fips_begin_app(HelloTriangleExercise cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(HelloTriangle exercise.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

## Shaders

fips_begin_app(ShadersCheck cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Shaders check.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

fips_begin_app(ColorInVertexShader cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Shaders color_in_vertex_shader.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

fips_begin_app(Uniform cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Shaders uniform.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

fips_begin_app(MoreAttribs cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Shaders more_attributes.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

fips_begin_app(ShaderClass cmdline)
fips_vs_disable_warnings(4819)
fips_dir(Common)
fips_files(shader.cc)
fips_files_ex(Shaders use_shader_class.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

## Textures

fips_begin_app(Textures cmdline)
fips_vs_disable_warnings(4819)
add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
fips_dir(Common)
fips_files(shader.cc texture2d.cc)
fips_files_ex(Textures main.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()

fips_begin_app(TexturesMix cmdline)
fips_vs_disable_warnings(4819)
add_definitions(-DSTB_IMAGE_IMPLEMENTATION)
fips_dir(Common)
fips_files(shader.cc texture2d.cc)
fips_files_ex(Textures mix_two.cc GROUP "SOURCES")
fips_deps(glfw glad)
fips_end_app()