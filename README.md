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
  -DCMAKE_REQUIRED_FLAGS="-s USE_LIBJPEG=1 -s USE_GLFW=3  -s FULL_ES2=1 -s FULL_ES3=1" \
  -Dfyaml_DIR=~/code/mrpt/build-ems/3rdparty/libfyaml/install/lib/cmake/fyaml/ \
  -Dnanoflann_DIR=~/code/mrpt/build-ems/3rdparty/nanoflann/ \
  -Dmrpt-bayes_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-containers_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-core_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-expr_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-config_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-img_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-io_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-math_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-opengl_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-poses_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-random_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-rtti_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-serialization_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-system_DIR=/home/jlblanco/code/mrpt/build-ems \
  -Dmrpt-typemeta_DIR=/home/jlblanco/code/mrpt/build-ems \
  -DEP_eigen3_DIR=/home/jlblanco/code/mrpt/build-ems \
  -DCMAKE_MODULE_PATH=~/code/mrpt/cmakemodules
```

Manual fixes to cmake variables:
- as


then simply:

```bash
make
```
  
