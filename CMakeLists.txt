cmake_minimum_required(VERSION 3.10)
project("Flup3D")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Sources
file(GLOB SRC_CPP "src/*.cpp")
file(GLOB SRC_C "src/*.c")
# Libraries
file(GLOB LIB_WIN "lib/*.lib")
file(GLOB LIB_LINUX "lib/*.o" "lib/*.a")


# Add Files To Final Compilation
add_executable(Flup3D ${SRC_CPP} ${SRC_C})

# Include Directories
target_include_directories(Flup3D PRIVATE "include")

# Link Libraries
if (WIN32)
    target_link_libraries(Flup3D PRIVATE ${LIB_WIN})
elseif (UNIX)
    target_link_libraries(Flup3D PRIVATE ${LIB_LINUX})
endif()