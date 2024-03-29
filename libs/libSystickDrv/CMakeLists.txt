# --- Configure the library ---
set(SUBMODULE_NAME systickdrv)
set(SUBMODULE_DEPS mmr)
set(SUBMODULE_TYPE STATIC)

# --- build the library ---

# --- build the library ---
set(SUBMODULE_PUB_INCS_DIR ${CMAKE_CURRENT_LIST_DIR}/api)
set(SUBMODULE_PRV_INCS_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
set(SUBMODULE_SRCS_DIR ${CMAKE_CURRENT_LIST_DIR}/source)
file(GLOB SUBMODULE_SRCS ${SUBMODULE_SRCS_DIR}/*.cpp)

add_library(${SUBMODULE_NAME} ${SUBMODULE_TYPE})

if (SUBMODULE_TYPE STREQUAL "INTERFACE")
    target_include_directories(${SUBMODULE_NAME} INTERFACE ${SUBMODULE_PUB_INCS_DIR})
else()
    target_include_directories(${SUBMODULE_NAME} PUBLIC ${SUBMODULE_PUB_INCS_DIR} PRIVATE ${SUBMODULE_PRV_INCS_DIR})
endif()

target_sources(${SUBMODULE_NAME} PRIVATE ${SUBMODULE_SRCS})

if (SUBMODULE_DEPS)
    add_dependencies(${SUBMODULE_NAME} ${SUBMODULE_DEPS})
    target_link_libraries(${SUBMODULE_NAME} PRIVATE ${SUBMODULE_DEPS})
endif()
add_subdirectory(tests)