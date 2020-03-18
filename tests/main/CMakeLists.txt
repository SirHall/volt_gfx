cmake_minimum_required(VERSION 3.7)


add_executable(ts_volt_gfx_test_main ${CMAKE_CURRENT_SOURCE_DIR}/main_test.cpp)
target_link_libraries(ts_volt_gfx_test_main volt_gfx)
set_property(TARGET ts_volt_gfx_test_main PROPERTY CXX_STANDARD 17)