cmake_minimum_required(VERSION 3.7)

# Setup Hunter 
# set(HUNTER_ROOT ${CMAKE_BINARY_DIR}/hunter/)
# Prefer pre-built binaries over building from source
set(HUNTER_USE_CACHE_SERVERS YES)

include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/HunterGate.cmake")

HunterGate(
    URL "https://github.com/ruslo/hunter/archive/v0.23.214.tar.gz"
    SHA1 "e14bc153a7f16d6a5eeec845fb0283c8fad8c358"
)

project(volt_ge_gfx)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# Setup Conan
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()
# message(STATUS "SDL2_Libs: ${CONAN_LIBS}")

# Copy resource files to build output 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/volt_gfx/res DESTINATION ${CMAKE_BINARY_DIR}/volt_gfx/.)

# Add in volt_gfx
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/volt_gfx/)

# Setup Dyygen
# find_package(Doxygen)
# if (DOXYGEN_FOUND)
#     set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
#     set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

#     configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
#     message("Doxygen build started")

#     add_custom_target( doc_doxygen_volt_net ALL
#         COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
#         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#         COMMENT "Generating API documentation with Doxygen"
#         VERBATIM )
# else (DOXYGEN_FOUND)
#     message("Doxygen need to be installed to generate the doxygen documentation")
# endif ()

enable_testing()

# Add in tests
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests/client_server/)
