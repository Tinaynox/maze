[![License](https://img.shields.io/badge/License-Zlib-brightgreen.svg)](/LICENSE.md)

![MAZE](/res/branding/MazeLogoLight01_540x270.png?raw=true "MAZE")


**MAZE** (**M**y **A**ma**Z**ing **E**ngine) is the self-written open-source cross-platform game engine in the active development stage.
At the moment it is my main pet project, developed for the purpose of learning and preserving different game dev technologies.


## Projects on this engine
**Space Pressure 3D: Prelude**

[![Space Pressure 3D: Prelude](/res/art/projects/sp3d/SP3DPromo00.gif)](https://store.steampowered.com/app/1864190/Space_Pressure_3D_Prelude/)

[![Steam](/res/art/SteamIcon_32x32.png?raw=true "Steam")](https://store.steampowered.com/app/1864190/Space_Pressure_3D_Prelude/) [![itch.io](/res/art/ItchIcon_32x32.png?raw=true "itch.io")](https://tinaynox.itch.io/space-pressure-3d-prelude)


## Getting Started
Start by cloning the repository with `git clone --recursive https://github.com/Tinaynox/maze`.

### ![Windows](/res/art/WindowsIcon_16x16.png?raw=true "Windows") Windows » ![Windows](/res/art/WindowsIcon_16x16.png?raw=true "Windows") Windows
- Download and install Visual Studio 2017 or 2019
- Download and install Windows SDK
- Download and install CMake 3.6+
- Download and install Python 3.8+

**Engine**
- Execute the suitable batch file `engine/prj/win/configure-*.bat`
- Open the generated project solution `prj/maze/*`

**Examples**
- Execute suitable batch file `examples/*/prj/win/make-data-*.bat`
- Execute suitable batch file `examples/*/prj/win/configure-*.bat`
- Open the generated project solution `prj/*/*`

### ![Windows](/res/art/WindowsIcon_16x16.png?raw=true "Windows") Windows » ![Android](/res/art/AndroidIcon_16x16.png?raw=true "Android") Android
- Download and install Android Studio
- Install latest Android SDK via SDK Manager
- Install latest NDK via SDK Manager
- Download and install Python 3.8+

**Examples**
- Execute the suitable batch file `examples/*/prj/win/configure-gradle-android.bat`
- Import `prj/*/prj.gradle-android` folder via Android Studio (Import project Gradle option)

### ![Windows](/res/art/WindowsIcon_16x16.png?raw=true "Windows") Windows » ![Emscripten](/res/art/EmscriptenIcon_16x16.png?raw=true "Emscripten") Emscripten
- Download and install Emscripten SDK
- Download and install CMake 3.6+
- Download and install Python 3.8+

**Examples**
- Execute suitable batch file `examples/*/prj/win/configure-emscripten-*.bat`
- Execute suitable batch file `examples/*/prj/win/compile-emscripten-*.bat`

### ![macOS](/res/art/MacOSIcon_16x16.png?raw=true "macOS") macOS » ![macOS](/res/art/MacOSIcon_16x16.png?raw=true "macOS") macOS
- Download and install XCode
- Download and install CMake 3.6+
- Download and install Python 3.8+

**Engine**
- Execute the suitable shell script `engine/prj/osx/configure-*.sh`
- Open the generated project solution `prj/maze/*`

**Examples**
- Execute suitable shell script `examples/*/prj/osx/make-data-*.bat`
- Execute suitable shell script `examples/*/prj/osx/configure-*.bat`
- Open the generated project solution `prj/*/*`

### ![Linux](/res/art/LinuxIcon_16x16.png?raw=true "Linux") Linux » ![Linux](/res/art/LinuxIcon_16x16.png?raw=true "Linux") Linux
*[Work In Progress]*

### ![macOS](/res/art/MacOSIcon_16x16.png?raw=true "macOS") macOS » ![iOS](/res/art/IOSIcon_16x16.png?raw=true "iOS") iOS
*[Work In Progress]*

## Features Demonstration
<details>
  <summary>Water, Terrain, and Sky</summary>
  <img src="/res/art/Demo00.gif" loading="lazy" />
</details>
<details>
  <summary>Particles</summary>
  <img src="/res/art/Particles00.gif" loading="lazy" />
</details>
<details>
  <summary>Particle Editor</summary>
  <img src="/res/art/ParticleEditor00.gif" loading="lazy" />
</details>
<details>
  <summary>Terrain Generator</summary>
  <img src="/res/art/Terrain00.gif" loading="lazy" />
</details>
<details>
  <summary>Trail Renderer</summary>
  <img src="/res/art/TrailRenderer00.gif" loading="lazy" />
</details>
<details>
  <summary>Editor</summary>
  <img src="/res/art/Editor00.gif" loading="lazy" />
</details>
<details>
  <summary>Physics 2D</summary>
  <img src="/res/art/Physics2D00.gif" loading="lazy" />
</details>