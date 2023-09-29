set(SUBMODULE_TESTS_NAME tests_libmmr)
set(SUBMODULE_TESTS_DEPS )


set(SUBMODULE_TESTS_MOCK_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/mocks/include)
set(SUBMODULE_TESTS_FAKES_INC_DIR ${CMAKE_CURRENT_LIST_DIR}/fakes/include)
set(SUBMODULE_TESTS_MOCK_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/mocks/sources)
set(SUBMODULE_TESTS_FAKES_SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/fakes/sources)

file(GLOB SUBMODULE_TESTS_MOCK_SRC ${SUBMODULE_TESTS_MOCK_SRC_DIR}/*.cpp)
file(GLOB SUBMODULE_TESTS_FAKES_SRC ${SUBMODULE_TESTS_FAKES_SRC_DIR}/*.cpp)
file(GLOB SUBMODULE_TESTS_TESTS_SRC ${CMAKE_CURRENT_LIST_DIR}/*.cpp)


include(GoogleTest)
# Define the test executable for this library's tests
add_executable(${SUBMODULE_TESTS_NAME} )

# Include the 'include' directory for library headers
target_include_directories(${SUBMODULE_TESTS_NAME}
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../api
        ${SUBMODULE_TESTS_MOCK_INC_DIR}
        ${SUBMODULE_TESTS_FAKES_INC_DIR}
        $<TARGET_PROPERTY:gtest,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:gtest_main,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:gmock,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
        $<TARGET_PROPERTY:gmock_main,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
    )

# Add source files for the tests within this directory
target_sources(${SUBMODULE_TESTS_NAME}
    PRIVATE
        ${SUBMODULE_TESTS_MOCK_SRC}
        ${SUBMODULE_TESTS_FAKES_SRC}
        ${SUBMODULE_TESTS_TESTS_SRC}
)

# Link the library being tested (e.g., 'libfoo')
target_link_libraries(${SUBMODULE_TESTS_NAME} ${SUBMODULE_TESTS_DEPS} gmock_main gtest_main)

# Add tests using CTest (optional)
gtest_add_tests(TARGET      ${SUBMODULE_TESTS_NAME}
                TEST_LIST   ${SUBMODULE_TESTS_NAME}
)
# Add tests using CTest (optional)
add_test(NAME ${SUBMODULE_TESTS_NAME} COMMAND ${SUBMODULE_TESTS_NAME})
