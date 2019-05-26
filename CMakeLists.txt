cmake_minimum_required(VERSION 3.0)
project(Asteroids)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Asteroids/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Asteroids/Vendor/assimp)

add_subdirectory(Asteroids/Vendor/freetype)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Asteroids/Headers/
                    Asteroids/Vendor/assimp/include/
                    Asteroids/Vendor/glad/include/
                    Asteroids/Vendor/glfw/include/
                    Asteroids/Vendor/glm/
                    Asteroids/Vendor/stb/
                    Asteroids/Vendor/freetype/include/)

file(GLOB VENDORS_SOURCES Asteroids/Vendor/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Asteroids/Headers/*.hpp)
file(GLOB PROJECT_SOURCES Asteroids/Sources/*.cpp)
file(GLOB PROJECT_SHADERS Asteroids/Shaders/*.vs
                          Asteroids/Shaders/*.fs)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw freetype
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} )
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})