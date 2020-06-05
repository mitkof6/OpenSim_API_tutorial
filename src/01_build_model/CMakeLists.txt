file(GLOB tutorial tutorial.cpp)
file(GLOB solution tutorial_solutions.cpp)

# Lab
set(target tutorial_01)
add_executable(${target} ${tutorial})
target_link_libraries(${target} ${OpenSim_LIBRARIES})
set_target_properties(
  ${target} PROPERTIES
  FOLDER "01_build_model"
)


set(target solution_01)
add_executable(${target} ${solution})
target_link_libraries(${target} ${OpenSim_LIBRARIES})
set_target_properties(
  ${target} PROPERTIES
  FOLDER "01_build_model"
)

set(ADDITIONAL_FILES
    "cube.obj"
)

foreach(dataFile ${ADDITIONAL_FILES})
  file(COPY "${dataFile}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
endforeach()
