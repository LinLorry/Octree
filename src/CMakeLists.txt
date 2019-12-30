add_library(GLAD glad.c)

add_executable(Octree main.cpp)

target_link_libraries(Octree GLAD)
target_link_libraries(Octree ${GLUT_LIBRARIES})
