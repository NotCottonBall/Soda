# Soda

[![License](https://img.shields.io/badge/license-MIT-blue.svg)](LICENSE)
## Description

Soda is an open-source Game Engine I'm making to learn more about graphics development. <br>


## Progress Till Now
![SodaEngine2D](https://github.com/CottonBall74/Soda/assets/64028676/98a43d97-fb66-4201-8d7d-488a839f04a7)

Soda is a Game Engine that is constantly in development by a single person. And it will probably stay that way for a while because i want to keep adding new and fun features/mechanics to the Engine. If you have any specific features or improvements you'd like to see, please open an issue or a discussion. I would appreciate that.

**Note:** The roadmap is subject to change, and new features may be added or adjusted based on community feedback and other needs.



## Getting Started
Follow these instructions to get a copy of the project up and running on your local machine for development and testing purposes.


## Deps
make sure you have these installed on your computer <br>
- [cmake](https://cmake.org/download/)
- [clang](https://github.com/llvm/llvm-project/releases/tag/llvmorg-17.0.1)
- [ninja](https://github.com/ninja-build/ninja/releases) <br>
if you are on windows then you can also use [scoop](https://scoop.sh/) or <b>winget</b> (which should be already installed). <br>
```bash
# for scoop
scoop bucket add main
scoop install cmake ninja clangd llvm
```
```bash
# for winget
winget install Kitware.CMake Ninja-build.Ninja LLVM.LLVM LLVM.clangd
```
<br>
in case you dont know what scoop is, check it out, its a great tool.

### NOTE: make sure you add cmake, clang and ninja to your $SYSTEM_PATH


<br>
<br>

# Building (Doesn't Support Linux Yet)
## cloning the repo
```bash
# NOTE: Make sure you clone the repo with the --recursive flag
git clone https://github.com/CottonBall74/Soda.git --recursive

# open the folder
cd Soda

# update the submodules (just in case)
git submodule init
git submodule update
```
## compiling
```bash
# if you are on windows
./scripts/Run.bat
# if you are on a unix device
./scripts/Run.sh
```
## Running the exec
#### NOTE: Please use Nvidia's GPUs as AMD's GPUs have this weird glitch effect. This will be solved later when the engine uses bindless textures.
if everything went well then the exec should be in bin/system-build_type-arch/SodaCan.* (by * i mean whatever the exec file is called in your system .exe .out etc...)<br>
i suggest not runnig the file yet. <br>
- first, you should copy the SodaCan.* to a new folder
- then, you should copy the imgui.ini in SodaCan/ project folder to the folder you put the SodaCan.* in

if you are facing any troubles while installing, you can always open an issue
