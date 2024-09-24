set_project("Raytracer")
set_version("0.0.1")

add_rules("mode.debug", "mode.release")

set_languages("c++23")
set_warnings("allextra")

add_requires("stb", "glm", "fmt", "doctest")

target("Raytracer")
    set_kind("binary")

    -- add_ldflags("-Wl,-stack_size,0x1000000")

    add_includedirs("include/")
    add_headerfiles("include/(**/*.h)")
    add_files("src/*.cpp")

    for _, file in ipairs(os.files("test/test_*.cpp")) do
        add_tests(path.basename(file), {
            files        = file,
            remove_files = "src/main.cpp",
            languages    = "c++23",
            packages     = "doctest",
            defines      = "DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN"
        })
    end

    add_packages("stb", "glm", "fmt")