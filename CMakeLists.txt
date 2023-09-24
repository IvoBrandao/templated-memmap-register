cmake_minimum_required(VERSION 3.25)

project(Example_Project)
include(FetchContent)
include(CTest)

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


