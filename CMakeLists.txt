cmake_minimum_required(VERSION 3.25)

project(Example_Project)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/tools/cmake/modules)
set(CMAKE_PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})

include(FetchContent)
include(CTest)
# Enable code coverage for GCC-based compilers
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

include(Doxygen)
include(PlantUml)
include(CodeCoverage)


# Set C++ standard to C++17 or higher
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG f8d7d77c06936315286eb55f8de22cd23c188571
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

# Add subdirectories
add_subdirectory(app)
add_subdirectory(libs)

# CMakeLists.txt in the app directory
add_executable(${PROJECT_NAME} app/source/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE app)


add_custom_target(DependencyGraph
COMMAND cmake --graphviz=DependencyGraph.dot .
COMMAND dot -Tsvg DependencyGraph.dot -o DependencyGraph.svg
WORKING_DIRECTORY ${CMAKE_BINARY_DIR})



