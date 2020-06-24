fips_begin_app(ShadowDirectDbg cmdline)
fips_vs_disable_warnings(4819 4091)
fips_files_ex(direct_dbg *.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(ShadowDirect cmdline)
fips_vs_disable_warnings(4819 4091)
fips_files_ex(direct *.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

fips_begin_app(ShadowPoint cmdline)
fips_vs_disable_warnings(4819 4091 4312)
fips_files_ex(point *.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()

## 尝试用Instancing技术优化, 但还是很卡
fips_begin_app(ShadowPoint0 cmdline)
fips_vs_disable_warnings(4819 4091 4312)
fips_files_ex(point0 *.cc GROUP "SOURCES")
fips_deps(Common)
fips_end_app()