# library
file(GLOB plugin_sources
  PerturbationForce.cpp
  RegisterPlugin.cpp)
file(GLOB plugin_includes
  PerturbationForceExports.h
  PerturbationForce.h
  RegisterPlugin.h)
file(GLOB test_sources TestPerturbationForce.cpp)

# create plugin
set(target_plugin PerturbationForce)
add_library(${target_plugin} SHARED ${plugin_sources} ${plugin_includes})
target_link_libraries (${target_plugin} ${OpenSim_LIBRARIES})
set_target_properties(
  ${target_plugin} PROPERTIES
  FOLDER "04_perturbation_force"
)

# add executable
set(target solution_04)
add_executable(${target} ${test_sources})
target_link_libraries(${target} ${OpenSim_LIBRARIES} ${target_plugin})
set_target_properties(
  ${target} PROPERTIES
  FOLDER "04_perturbation_force"
)

set(ADDITIONAL_FILES
  "test_plugin.py"
  "test_prescribed_force.py"
  "tug_of_war.osim"
)

foreach(dataFile ${ADDITIONAL_FILES})
  file(COPY "${dataFile}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endforeach()
