# library
file(GLOB plugin_sources
  Neuron.cpp
  RegisterPlugin.cpp)
file(GLOB plugin_includes
  NeuronExports.h
  Neuron.h
  RegisterPlugin.h)
file(GLOB test_sources TestNeuron.cpp)

# create plugin
set(target_plugin Neuron)
add_library(${target_plugin} SHARED ${plugin_sources} ${plugin_includes})
target_link_libraries (${target_plugin} ${OpenSim_LIBRARIES})
set_target_properties(
  ${target_plugin} PROPERTIES
  FOLDER "06_model_component_neuron"
)

# add executable
set(target TestNeuron)
add_executable(${target} ${test_sources})
target_link_libraries(${target} ${OpenSim_LIBRARIES} ${target_plugin})
set_target_properties(
  ${target} PROPERTIES
  FOLDER "06_model_component_neuron"
)

set(ADDITIONAL_FILES
  "visualize_sto.py"
)

foreach(dataFile ${ADDITIONAL_FILES})
  file(COPY "${dataFile}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endforeach()
