#!/usr/bin/env python
import os
import sys

# 初始化环境
env = SConscript("godot-cpp/SConstruct")

# 添加头文件路径
env.Append(CPPPATH=["src/", "src/thirdparty/fmod/inc"])
sources = Glob("src/*.cpp")

# --- 关键修复：清理 Windows 污染 ---
# 如果在 Windows 上编译 Android，SCons 可能会错误地保留 MinGW 的链接库。
# 我们需要手动清洗 LIBS 列表。
if env["platform"] == "android":
    windows_libs_blacklist = [
        "mingw32", "gcc", "gcc_s", "moldname", "mingwex", "msvcrt", 
        "advapi32", "shell32", "user32", "kernel32", "stdc++"
    ]
    # 过滤掉黑名单中的库
    if "LIBS" in env:
        clean_libs = [lib for lib in env["LIBS"] if str(lib) not in windows_libs_blacklist]
        env.Replace(LIBS=clean_libs)

# --- Android 平台配置 ---
if env["platform"] == "android":
    env.Append(CXXFLAGS=['-std=c++17'])
    
    # 获取架构 (godot-cpp 传入的架构名称通常是 arm64, arm32, x86_64, x86_32)
    arch = env["arch"]
    
    # 映射 Godot 架构名 -> FMOD 文件夹名
    # 请务必确认你的 FMOD 文件夹结构真的是这样！
    # 标准 FMOD Android SDK 路径通常是: core/lib/arm64-v8a/
    fmod_arch_dir = ""
    if arch == "arm64":
        fmod_arch_dir = "arm64-v8a"
    elif arch == "arm32":
        fmod_arch_dir = "armeabi-v7a"
    elif arch == "x86_64":
        fmod_arch_dir = "x86_64"
    elif arch == "x86_32":
        fmod_arch_dir = "x86"
    else:
        print(f"Warning: Unknown architecture {arch}, defaulting to arm64-v8a")
        fmod_arch_dir = "arm64-v8a"

    # 构建绝对路径以避免相对路径错误
    base_path = os.getcwd()
    fmod_lib_path = os.path.join(base_path, "src/thirdparty/fmod/lib/android", fmod_arch_dir)
    
    print(f"--- Debug: FMOD Lib Path for {arch}: {fmod_lib_path}")
    
    # 添加库路径
    env.Append(LIBPATH=[fmod_lib_path])
    
    # 添加链接库
    if env["target"] == "template_release":
        env.Append(LIBS=["fmod"])  # 对应 libfmod.so
    else:
        env.Append(LIBS=["fmodL"]) # 对应 libfmodL.so (Logging 版)

    # 编译选项
    if env["target"] == "template_release":
        env.Append(CXXFLAGS=['-O3'])
    else:
        env.Append(CXXFLAGS=['-g', '-O0'])
    
    # 设置输出动态库路径
    library = env.SharedLibrary(
        "addons/bin/libfmod_player{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

# --- macOS 平台配置 ---
elif env["platform"] == "macos":
    library = env.SharedLibrary(
        "addons/bin/fmod_player.{}.{}.framework/fmod_player.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )

# --- iOS 平台配置 ---
elif env["platform"] == "ios":
    if env["ios_simulator"]:
        library = env.StaticLibrary(
            "addons/bin/fmod_player.{}.{}.simulator.a".format(env["platform"], env["target"]),
            source=sources,
        )
    else:
        library = env.StaticLibrary(
            "addons/bin/fmod_player.{}.{}.a".format(env["platform"], env["target"]),
            source=sources,
        )

# --- Windows/Linux 平台配置 ---
else:
    # 你的 Windows 配置逻辑
    env.Append(CCFLAGS=["/utf-8"])
    env.Append(CXXFLAGS=["/utf-8"])
    env.Append(CCFLAGS=["/wd4828"])
    # 注意：这里路径最好也检查一下是否正确
    env.Append(LIBPATH=["src/thirdparty/fmod/lib/x64"])
    env.Append(LIBS=["fmod_vc"])
    library = env.SharedLibrary(
        "addons/bin/fmod_player{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
