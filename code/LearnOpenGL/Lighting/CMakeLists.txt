## Colors

fips_begin_app(LightingColor cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Colors main.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## Basic Lighting

fips_begin_app(Ambient cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(BasicLighting ambient.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(Diffuse cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(BasicLighting diffuse.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(Specular cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(BasicLighting specular.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(LightAround cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(BasicLighting light_around.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## Materials

fips_begin_app(Materials cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Materials materials.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(LightAttribute cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Materials light_attribute.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(LightColor cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Materials light_color.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()