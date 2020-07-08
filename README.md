# Dempsta

<center><img src="media/dempsta_icon.png" height="300"></center>

[![pipeline status](https://gitlab.com/mrcoalp/dempsta-engine/badges/master/pipeline.svg)](https://gitlab.com/mrcoalp/dempsta-engine/-/commits/master)
[![contributions](https://img.shields.io/badge/contributions-welcome-informational)](https://gitlab.com/mrcoalp/dempsta-engine/-/blob/master/CONTRIBUTING.md)

## README

Dempsta is a personal project meant to learn step-by-step what it takes to build a game engine.

The idea is to make it as cross-platform as possible. Currently tested in Linux and Windows, using OpenGL as the only graphics API. Planned to add Vulkan and D3D.

Using Lua as scripting language.

### Compiling

#### Linux

Simply run:

* `$ ./build.sh` <br>

**Note** : Using by default Ninja as the build system, can be changed to Make with `--make` option. Run `$ ./build.sh -h` to see all the available options.

#### Windows

Open the project in Visual Studio, build and run dempsta_editor exe.

### Tests

To compile and run the tests:

* `$ ./build.sh -i -t`
* `$ ./test.sh`

### Features

* [x] 2D renderer capable of render quads; 
* [x] Basic capability to expose C++ to Lua and vice-versa; 
* [x] Pseudo editor using ImGui; 
* [ ] Entity component system; 
* [ ] Entity edition in editor; 
