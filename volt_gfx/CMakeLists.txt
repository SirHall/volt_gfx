cmake_minimum_required(VERSION 3.7)

# Find all source files
file(GLOB_RECURSE VOLT_GFX_SRCS
    "src/*.cpp"
    "src/*.c"
    RELATIVE_PATH
)

# Copy resource files to build output 
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR}/.)

set(OpenGL_GL_PREFERENCE GLVND)

# find_package (Threads)

add_library(volt_gfx ${VOLT_GFX_SRCS})
target_include_directories(volt_gfx PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_link_libraries(volt_gfx ${CMAKE_THREAD_LIBS_INIT})
# target_link_libraries(volt_gfx ${CONAN_LIBS})
# target_link_libraries(volt_gfx volt_event)
# target_link_libraries(volt_gfx LINK_PUBLIC ${Boost_LIBRARIES})

find_package(glfw3 CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
add_dependencies(volt_gfx volt_event)
find_package(Boost CONFIG REQUIRED system filesystem)
target_link_libraries(volt_gfx PUBLIC volt_event glfw glew::glew glm Boost::system Boost::filesystem)



set_property(TARGET volt_gfx PROPERTY CXX_STANDARD 17)

# Flags
# if(MSVC)
#     target_compile_options(volt_gfx PRIVATE /W4 /WX)
# else()
#     set(WARNING_FLAGS "-Wall -Wextra -pedantic -Werror -Wno-unknown-pragmas -Wno-unused-parameter")
#     if(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE EQUAL "Debug")
#         target_compile_options(volt_gfx PRIVATE "${WARNING_FLAGS} -g -fprofile-arcs -ftest-coverage")
#     endif()
#     if(DEFINED CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE EQUAL "Release")
#         target_compile_options(volt_gfx PRIVATE "${WARNING_FLAGS} -flto -O3")
#     endif()
# endif()
