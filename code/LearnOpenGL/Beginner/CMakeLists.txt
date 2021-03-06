
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
fips_files_ex(Shaders use_shader_class.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(MovingVertex cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Shaders moving_vertex.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## Textures

fips_begin_app(Textures cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Textures main.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(TexturesMix cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Textures mix_two.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(TexturesMixAndMirror cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Textures mix_and_mirror.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(Wrapping cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Textures wrapping.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(Scale cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Textures scale.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## Transformations

fips_begin_app(Translate cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Transformations translate.cc GROUP "SOURCES")
fips_end_app()

fips_begin_app(ScaleAndRotate cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Transformations scale_rotate.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(RotateByTime cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Transformations rotate_by_time.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(TwoBox cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Transformations two_box.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## CoordinateSystems

fips_begin_app(CoordinateTrans cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(CoordinateSystems coordinate_trans.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(Cube8 cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(CoordinateSystems cube8.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(Cube36 cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(CoordinateSystems cube36.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(CubeMore cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(CoordinateSystems cube_more.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## Camera

fips_begin_app(CameraAround cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Camera camera_around.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(CameraMoveAround cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Camera camera_move_around.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(CameraLookAround cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Camera camera_look_around.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(CameraZoom cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Camera camera_zoom.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(CameraClass cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Camera camera_use_class.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()