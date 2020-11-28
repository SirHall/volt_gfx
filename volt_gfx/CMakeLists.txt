cmake_minimum_required(VERSION 3.7)

# Find all source files
file(GLOB_RECURSE VOLT_GFX_SRCS
    "src/*.cpp"
    "src/*.c"
    RELATIVE_PATH
)

# Copy resource files to build output 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR})

set(OpenGL_GL_PREFERENCE GLVND)

find_package (Threads)

# Boost
hunter_add_package(Boost COMPONENTS system filesystem)
find_package(Boost CONFIG REQUIRED system filesystem)
# glew
hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)
# glfw
hunter_add_package(glfw)
find_package(glfw3 CONFIG REQUIRED)
# glm
hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)
# volt_event
hunter_add_package(volt_event)
find_package(volt_event CONFIG REQUIRED)

add_library(volt_gfx ${VOLT_GFX_SRCS})
target_include_directories(volt_gfx PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>")
target_link_libraries(volt_gfx ${CMAKE_THREAD_LIBS_INIT} volt_event glfw glew::glew glm Boost::boost Boost::system Boost::filesystem)
set_property(TARGET volt_gfx PROPERTY CXX_STANDARD 17)

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(config_install_dir "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(TARGETS_EXPORT_NAME "${PROJECT_NAME}Targets")

write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)

# Don't know how much we need this given this is a header-only library
install(
    TARGETS              volt_gfx
    EXPORT               "${TARGETS_EXPORT_NAME}"
    LIBRARY DESTINATION  "${CMAKE_INSTALL_LIBDIR}" # Probably not needed
    ARCHIVE DESTINATION  "${CMAKE_INSTALL_LIBDIR}" # Probably not needed
    RUNTIME DESTINATION  "${CMAKE_INSTALL_BINDIR}" # Probably not needed
    INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
)

# Headers
install(
    DIRECTORY "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/volt>"
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
    # FILES_MATCHING PATTERN "*.hpp"
)

# Configs
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

# Config
install(
    EXPORT "${TARGETS_EXPORT_NAME}"
    # NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)