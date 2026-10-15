set_project("Inferno")
set_version("0.1.0")
set_languages("gnu11")
set_toolchains("clang")

add_rules("mode.debug", "mode.release")

set_warnings("all", "extra", "error")
add_cxxflags("-Wpedantic", "-Wconversion", "-Wshadow", { tools = { "gcc", "clang" } })

target("Inferno")
set_kind("binary")
add_files("virtual_space/srcs/*.c", { public = true })
add_includedirs("virtual_space/incs", { public = true })
add_headerfiles("virtual_space/incs/(*.h)")
