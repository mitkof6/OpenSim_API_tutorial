file(GLOB tutorial tutorial.cpp)
file(GLOB solution tutorial_solutions.cpp)

# Lab
set(target tutorial_03)
add_executable(${target} ${tutorial})
target_link_libraries(${target} ${OpenSim_LIBRARIES})
set_target_properties(
  ${target} PROPERTIES
  FOLDER "03_perform_optimization"
)

set(target solution_03)
add_executable(${target} ${solution})
target_link_libraries(${target} ${OpenSim_LIBRARIES})
set_target_properties(
  ${target} PROPERTIES
  FOLDER "03_perform_optimization"
)

set(ADDITIONAL_FILES
    "../01_build_model/cube.obj"
    "../01_build_model/Dennis.osim"
)

foreach(dataFile ${ADDITIONAL_FILES})
  file(COPY "${dataFile}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endforeach()
