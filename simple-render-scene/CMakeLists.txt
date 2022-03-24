project(simple-render-scene)

cmake_minimum_required(VERSION 3.1)

set(EXECUTABLE_OUTPUT_PATH ".")

# ===== Workarounds to mrpt built as static libs!! ================
# See docs for CMake FindThreads()
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads) # Defines imported target Threads::Threads
#/home/jlblanco/code/mrpt/build-Emscripten/3rdparty/libfyaml/install/lib/cmake/fyaml
find_package(Filesystem)

find_package(fyaml)
# hack: https://stackoverflow.com/questions/45401212/how-to-make-imported-target-global-afterwards
set_target_properties(fyaml::fyaml PROPERTIES IMPORTED_GLOBAL TRUE)

add_library(mrpt_libfyaml ALIAS fyaml::fyaml)
# ===== End of Workarounds ================

find_package(mrpt-opengl REQUIRED)

# So Emscripten creates a demoable HTML instead of the bare .js code:
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Define the executable target:
add_executable(${PROJECT_NAME} main.cpp)

target_compile_options(${PROJECT_NAME} PRIVATE -s USE_SDL=2)
target_link_options(${PROJECT_NAME} PRIVATE -s USE_SDL=2)

target_link_libraries(${PROJECT_NAME} 
    mrpt::opengl
    # Workarounds to static mrpt!
    
    Threads::Threads
    CXX::Filesystem
    )
