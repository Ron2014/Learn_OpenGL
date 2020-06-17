## Model

fips_begin_app(LoadModel cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Model load_model.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(ModelInLights cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Model model_in_lights.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(ModelTutorial cmdline)
fips_vs_disable_warnings(4819)
fips_files_ex(Model/tutorial/ model_loading.cpp GROUP "SOURCES")
fips_deps(Common)
fips_end_app()