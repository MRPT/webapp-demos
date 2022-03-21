# mrpt-webapp-demos 

What is this? 
(Write me!)


# Build instructions

## Build the MRPT JavaScript port 

Following [these steps](https://github.com/MRPT/mrpt/wiki/Emscripten-builds-for-web-apps).

## Build webapps

```bash
cd <APP_DIRECTORY>
mkdir build && cd build

cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=$HOME/code/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake \
  -DCMAKE_EXE_LINKER_FLAGS="-s ASSERTIONS=1 -s ALLOW_MEMORY_GROWTH=1 -s USE_GLFW=3 -s FULL_ES2=1 -s FULL_ES3=1 -s MAX_WEBGL_VERSION=2" \
  -DCMAKE_CXX_FLAGS="-s NO_DISABLE_EXCEPTION_CATCHING  -s MAX_WEBGL_VERSION=2" \
  -DCMAKE_REQUIRED_FLAGS="-s USE_LIBJPEG=1 -s USE_GLFW=3  -s FULL_ES2=1 -s FULL_ES3=1 -s MAX_WEBGL_VERSION=2" \
  -Dmrpt-poses_DIR=~/code/mrpt/build-ems/
```

Manual fixes to cmake variables:
- as


then simply:

```bash
make
```
  
