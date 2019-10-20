# vim: set sts=4 sw=4 et :

# WARNINGS_ARE_ERRORS:
# Setting this to True will treat all warnings as errors.
#
# This should be True when you are working on the code.
#
# If you are merely trying to get this to run,
# feel free to set this to False.
#
WARNINGS_ARE_ERRORS = True

env = Environment(
    COMMON_FLAGS = ["-g", "-Og"],
    CFLAGS = ["${COMMON_FLAGS}", "-Wall", "-Wextra", "-pedantic", "-std=c11",],
    CXXFLAGS = ["${COMMON_FLAGS}", "-Wall", "-Wextra", "-pedantic", "-std=c++11",],
    LINKFLAGS = ["${COMMON_FLAGS}",],
)

env.Replace(
    TARGET_SYSTEM = "x86_64-linux",
)

env.MergeFlags([
    "!sdl2-config --cflags",
    "!sdl2-config --libs",
])

env.Append(
    LIBS=[
        "SDL2_image",
        "SDL2_ttf",
    ],
)

if WARNINGS_ARE_ERRORS:
    env.MergeFlags(["-Werror",])

Export("env")

#print(env.Dump())
env.SConscript(
    "src/SConscript",
    variant_dir="build/${TARGET_SYSTEM}/",
    duplicate=False,
)

