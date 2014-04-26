set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(AssetLoading Main.cpp)
target_link_libraries(AssetLoading Xli XliMedia XliMain)
