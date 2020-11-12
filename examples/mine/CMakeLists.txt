cmake_minimum_required(VERSION 3.7)

add_executable(ts_volt_gfx_test_mine ${CMAKE_CURRENT_SOURCE_DIR}/mine_example.cpp)
target_link_libraries(ts_volt_gfx_test_mine volt_gfx)
set_property(TARGET ts_volt_gfx_test_mine PROPERTY CXX_STANDARD 17)
