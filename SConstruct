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
    COMMON_FLAGS = ["-g", "-Og", "-std=c++11",],
    CFLAGS = ["${COMMON_FLAGS}", "-Wall", "-Wextra", "-pedantic",],
    CXXFLAGS = ["${COMMON_FLAGS}", "-Wall", "-Wextra", "-pedantic",],
    LINKFLAGS = ["${COMMON_FLAGS}",],
)

env.Replace(
    TARGET_SYSTEM = "x86_64-linux",
)

env.MergeFlags([
    "!allegro-config --cflags",
    "!allegro-config --libs",
])

if WARNINGS_ARE_ERRORS:
    env.MergeFlags(["-Werror",])

Export("env")

#print(env.Dump())
env.SConscript(
    "src/SConscript",
    variant_dir="build/${TARGET_SYSTEM}/",
    duplicate=False,
)

