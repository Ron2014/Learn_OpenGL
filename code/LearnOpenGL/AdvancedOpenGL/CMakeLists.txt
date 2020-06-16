
## Depth Testing

fips_begin_app(DepthFunc cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(DepthTesting depth_func.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(DepthValue cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(DepthTesting depth_value.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(DepthLinear cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(DepthTesting depth_linear.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## Stencil Testing

fips_begin_app(Outline cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(StencilTesting outline.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(OutlineDepth cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(StencilTesting outline_depth.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()