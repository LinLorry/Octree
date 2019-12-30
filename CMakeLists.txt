project(Octree)

cmake_minimum_required(VERSION 3.15)

find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    link_libraries(${OPENGL_LIBRARIES})
    include_directories(${OPENGL_INCLUDE_DIR})
endif(OPENGL_FOUND)

find_package(glfw3 3.3 REQUIRED)
link_libraries(glfw ${CMAKE_DL_LIBS})

find_package(glm REQUIRED) 

find_package(GLUT REQUIRED)

include_directories(include)

add_subdirectory(src)