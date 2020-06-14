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

## Lighting Map

fips_begin_app(LightingMap0 cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightingMaps lighting_map0.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(LightingMap cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightingMaps lighting_map.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(LightingMapSpecular cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightingMaps lighting_map_specular.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(ColorfulSpecular cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightingMaps colorful_specular.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(EmissionLetters cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightingMaps emission_letters.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## Light Casters

fips_begin_app(DirectLight cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightCasters direct_light.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(PointLight cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightCasters point_light.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(PointLightFollow cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightCasters point_light_follow.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(SpotLight cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightCasters spotlight.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(SpotLightFollow cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightCasters spotlight_follow.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(SpotLightFadeout cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(LightCasters spotlight_fadeout.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## Multiple Lights

fips_begin_app(MultipleLights cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(MultipleLights multiple_lights.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

