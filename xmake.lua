set_project("Raytracer")
set_version("0.0.1")

set_languages("c++23")
set_warnings("allextra")

add_requires("stb", "glm", "fmt")

target("Raytracer")
    set_kind("binary")

    add_files("src/*.cpp")

    add_packages("stb", "glm", "fmt")