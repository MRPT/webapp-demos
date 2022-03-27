project(nanogui-web-demo)

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

find_package(EP_eigen3)
set_target_properties(Eigen PROPERTIES IMPORTED_GLOBAL TRUE)
add_library(eigen3 ALIAS Eigen)
# ===== End of Workarounds ================

find_package(mrpt-nanogui REQUIRED)

# So Emscripten creates a demoable HTML instead of the bare .js code:
set(CMAKE_EXECUTABLE_SUFFIX ".html")

# Define the executable target:
add_executable(${PROJECT_NAME} example3.cpp)

target_link_libraries(${PROJECT_NAME}
    mrpt::mrpt-nanogui
    # Workarounds to static mrpt!
    
    Threads::Threads
    CXX::Filesystem
    )

