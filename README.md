# slime-apocalypse
Run away from crowds of 3D deformable slimes. In real-time....

# Authors
Dalton Hildreth
Caleb Biasco
Nathan Heller

# Build
directory structure:
/
  Headers/
  Libraries/
  Resources/
  Shaders/
  Sources/
  CMakeLists.txt
  README.md

To Init:
```
git clone --recursive https://github.umn.edu/hildr039/slime-apocalypse.git
```

To Build:
```
cd Build
# UNIX Makefile
# Although, this might work anywhere.
cmake ..

# Mac OSX
# cmake -G "Xcode" ..

# Microsoft Windows
# cmake -G "Visual Studio 14" ..
# cmake -G "Visual Studio 14 Win64" ..
```
