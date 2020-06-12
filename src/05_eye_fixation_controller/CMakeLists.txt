# library
file(GLOB plugin_sources
  FixationController.cpp
  RegisterPlugin.cpp)
file(GLOB plugin_includes
  FixationControllerExports.h
  FixationController.h
  RegisterPlugin.h)
file(GLOB test_sources TestFixationController.cpp)

# create plugin
set(target_plugin FixationController)
add_library(${target_plugin} SHARED ${plugin_sources} ${plugin_includes})
target_link_libraries (${target_plugin} ${OpenSim_LIBRARIES})
set_target_properties(
  ${target_plugin} PROPERTIES
  FOLDER "05_eye_fixation_controller"
)

# add executable
set(target TestFixationController)
add_executable(${target} ${test_sources})
target_link_libraries(${target} ${OpenSim_LIBRARIES} ${target_plugin})
set_target_properties(
  ${target} PROPERTIES
  FOLDER "05_eye_fixation_controller"
)

set(ADDITIONAL_FILES
  "pupil.jpg"
  "pupil.obj"
  "sclera.jpg"
  "sclera.obj"
  "UPAT_Eye_Model_Passive_Pulleys_v2.osim"
)

foreach(dataFile ${ADDITIONAL_FILES})
  file(COPY "${dataFile}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endforeach()
