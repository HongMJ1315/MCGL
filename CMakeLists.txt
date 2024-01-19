cmake_minimum_required(VERSION 3.12)

project("Perlin"
    VERSION 0.1.0
    LANGUAGES CXX C
    DESCRIPTION "Perlin"
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
else()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
endif()

message(STATUS "C Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "C Flags: ${CMAKE_C_FLAGS}")
message(STATUS "C Debug Flags: ${CMAKE_C_FLAGS_DEBUG}")
message(STATUS "C Release Flags: ${CMAKE_C_FLAGS_RELEASE}")

find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenCV 4 REQUIRED)
find_package(Eigen3 REQUIRED)

link_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(Codes
    "main"
)

foreach(ex IN LISTS Codes)
    message(STATUS "Configuring ${ex}...")

    add_executable(${ex})
    target_sources(${ex} PRIVATE "${ex}.cpp")
    include_directories(${ex} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
    link_directories(${ex} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

    target_include_directories(${ex} PRIVATE ${OpenCV_INCLUDE_DIRS})
    target_link_libraries(${ex} PRIVATE
        glfw
        OpenGL32
        GLUT::GLUT
        ${OpenCV_LIBS}
        Eigen3::Eigen
    )
endforeach()
