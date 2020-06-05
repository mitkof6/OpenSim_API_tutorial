CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

# project
project(OpenSimAPI_Tutorials VERSION 1.0.0)

# for emacs completion
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# for grouping into folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# configure for new version of OpenSim
find_package(OpenSim REQUIRED)
include_directories(${OpenSim_INCLUDE_DIRS})
link_directories(${OpenSim_LIB_DIR})

# sub-modules
add_subdirectory(src)
