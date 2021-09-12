cmake_minimum_required(VERSION 3.7)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/volt_hunt.cmake)

# set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/inst/") # For testing

project(volt_gfx VERSION 0.0.17)

include(ExternalProject)
set(EXTERNAL_INSTALL_LOCATION ${CMAKE_BINARY_DIR}/dep)
include_directories(${EXTERNAL_INSTALL_LOCATION}/include)

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

# Add in examples
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples/)
