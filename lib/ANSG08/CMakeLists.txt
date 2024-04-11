add_library(ANSG08 ANSG08.cpp)
target_link_libraries(ANSG08 PUBLIC PicoPeripherals)
target_include_directories(ANSG08 INTERFACE .)
target_include_directories(ANSG08 PUBLIC . PicoPeripherals)