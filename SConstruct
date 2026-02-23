#!/usr/bin/env python
import os
import sys

# ��ʼ������
env = SConscript("godot-cpp/SConstruct")

# ����ͷ�ļ�·��
env.Append(CPPPATH=["src/", "src/thirdparty/fmod/inc"])
sources = (Glob("src/core/*.cpp") + Glob("src/audio/*.cpp") + Glob("src/playback/*.cpp") + 
           Glob("src/mixer/*.cpp") + Glob("src/dsp/*.cpp") + Glob("src/nodes/*.cpp") + Glob("src/editor/*.cpp"))

# --- ���� Windows ��Ⱦ ---
# ����� Windows �ϱ��� Android��SCons ���ܻ����ر��� MinGW �����ӿ⡣
# ������Ҫ�ֶ���ϴ LIBS �б���
if env["platform"] == "android":
    windows_libs_blacklist = [
        "mingw32", "gcc", "gcc_s", "moldname", "mingwex", "msvcrt", 
        "advapi32", "shell32", "user32", "kernel32", "stdc++"
    ]
    # ���˵��������еĿ�
    if "LIBS" in env:
        clean_libs = [lib for lib in env["LIBS"] if str(lib) not in windows_libs_blacklist]
        env.Replace(LIBS=clean_libs)

# --- Android ƽ̨���� ---
if env["platform"] == "android":
    env.Append(CXXFLAGS=['-std=c++17'])
    
    # ��ȡ�ܹ� (godot-cpp ����ļܹ�����ͨ���� arm64, arm32, x86_64, x86_32)
    arch = env["arch"]
    
    # ӳ�� Godot �ܹ��� -> FMOD �ļ�����
    # �����ȷ����� FMOD �ļ��нṹ�����������
    # ��׼ FMOD Android SDK ·��ͨ����: core/lib/arm64-v8a/
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

    # ��������·���Ա������·������
    base_path = os.getcwd()
    fmod_lib_path = os.path.join(base_path, "src/thirdparty/fmod/lib/android", fmod_arch_dir)
    
    print(f"--- Debug: FMOD Lib Path for {arch}: {fmod_lib_path}")
    
    # ���ӿ�·��
    env.Append(LIBPATH=[fmod_lib_path])
    
    # �������ӿ�
    if env["target"] == "template_release":
        env.Append(LIBS=["fmod"])  # ��Ӧ libfmod.so
    else:
        env.Append(LIBS=["fmodL"]) # ��Ӧ libfmodL.so (Logging ��)

    # ����ѡ��
    if env["target"] == "template_release":
        env.Append(CXXFLAGS=['-O3'])
    else:
        env.Append(CXXFLAGS=['-g', '-O0'])
    
    # ���������̬��·��
    library = env.SharedLibrary(
        "addons/bin/libfmod_player{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

# --- macOS ƽ̨���� ---
elif env["platform"] == "macos":
    library = env.SharedLibrary(
        "addons/bin/fmod_player.{}.{}.framework/fmod_player.{}.{}".format(
            env["platform"], env["target"], env["platform"], env["target"]
        ),
        source=sources,
    )

# --- iOS ƽ̨���� ---
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

# --- Windows/Linux ƽ̨���� ---
else:
    # ��� Windows �����߼�
    env.Append(CCFLAGS=["/utf-8"])
    env.Append(CXXFLAGS=["/utf-8"])
    env.Append(CCFLAGS=["/wd4828"])
    # ע�⣺����·�����Ҳ���һ���Ƿ���ȷ
    env.Append(LIBPATH=["src/thirdparty/fmod/lib/x64"])
    env.Append(LIBS=["fmod_vc"])
    library = env.SharedLibrary(
        "addons/bin/fmod_player{}{}".format(env["suffix"], env["SHLIBSUFFIX"]),
        source=sources,
    )

Default(library)
