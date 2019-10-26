# vim: set sts=4 sw=4 et :

import sys
import subprocess

def syscall(argv):
    pfp = subprocess.Popen(
        argv,
        stdout=subprocess.PIPE,
    )
    sub_output_stripped = None
    try:
        sub_stdout, sub_stderr, = pfp.communicate()
        sub_output_stripped = sub_stdout.strip()
        # Python 2 -> Python 3
        if not isinstance(sub_output_stripped, str):
            sub_output_stripped = sub_output_stripped.decode("utf-8")
        else:
            sub_output_stripped = sub_output_stripped

        assert " " not in sub_output_stripped
        assert "\t" not in sub_output_stripped
        assert "\n" not in sub_output_stripped
        assert "\r" not in sub_output_stripped
        assert "\x00" not in sub_output_stripped

    finally:
        sub_return = pfp.wait()
        assert sub_return == 0
    return sub_output_stripped

# WARNINGS_ARE_ERRORS:
# Setting this to True will treat all warnings as errors.
#
# This should be True when you are working on the code.
#
# If you are merely trying to get this to run,
# feel free to set this to False.
#
WARNINGS_ARE_ERRORS = True

vars = Variables("build_settings.py", ARGUMENTS)
vars.Add("CC", "C compiler", "gcc")
vars.Add("CXX", "C++ compiler", "g++")

env = Environment(
    variables = vars,

    COMMON_FLAGS = ["-g", "-Og"],
    C_AND_CXX_FLAGS = [
        "${COMMON_FLAGS}",
        "-Wall",
        "-Wextra",
        "-pedantic",
    ],
    CFLAGS = [
        "${C_AND_CXX_FLAGS}",
        "-std=c11",
    ],
    CXXFLAGS = [
        "${C_AND_CXX_FLAGS}",
        "-std=c++11",
    ],
)

env.Replace(
    TARGET_SYSTEM = syscall([env['CC'], '-dumpmachine'])
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

if "gcc" in env["CC"] or ("g++" in env["CXX"] and "clang++" not in env["CXX"]):
    env.Append(
        C_AND_CXX_FLAGS = [
            "-Wconversion",
            "-Wpedantic",
            "-Wlogical-op",
            "-Wformat=2",
            "-Wformat-overflow=2",
            "-Wformat-signedness",
            "-Wformat-truncation=2",
            "-Wnull-dereference",
            "-Wuninitialized",
            "-Wstrict-aliasing=2",
            "-Wstringop-truncation",
            "-Wduplicated-cond",
            "-Walloca",
            "-Warray-bounds=2",
            "-Wtrampolines",
            "-Wfloat-equal",
            "-Wshadow",
            "-Wpointer-arith",
            "-Wcast-align=strict",
            "-Wwrite-strings",
            "-Wcast-qual",
            "-Wdate-time",
        ],
        CFLAGS = [
            "-Wc++-compat",
            "-Wbad-function-cast",
        ],
        CXXFLAGS = [
        ],
    )

if "clang" in env["CC"]:
    env["CFLAGS"] += ["-Weverything",]
if "clang" in env["CXX"]:
    env["CXXFLAGS"] += ["-Weverything",]

if WARNINGS_ARE_ERRORS:
    env.MergeFlags(["-Werror",])

# Get engine version
#engine_version = "0.0" # It will be a long time before SemVer could make any sense --GM
engine_version = "git"
git_commit_version = syscall([ "git", "show", "-s", "--format=%h", "HEAD" ])
engine_version += "+" + git_commit_version
env.Replace(
    ENGINE_VERSION = engine_version,
)

Export("env")

#print(env.Dump())
env.SConscript(
    "src/SConscript",
    variant_dir="build/${TARGET_SYSTEM}/",
    duplicate=False,
)
