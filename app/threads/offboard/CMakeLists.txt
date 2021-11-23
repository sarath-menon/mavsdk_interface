
add_library(offboard
include/offboard_thread.h
src/offboard_thread.cpp
)

target_include_directories(offboard PUBLIC include)

# QT depenencies
target_link_libraries(offboard PRIVATE 
Qt${QT_VERSION_MAJOR}::Widgets 
)

# mavsdk dependencies
target_link_libraries(offboard PRIVATE 
    MAVSDK::mavsdk_telemetry
    MAVSDK::mavsdk_action
    MAVSDK::mavsdk_offboard
    MAVSDK::mavsdk
)


# Other dependencies
target_link_libraries(offboard PRIVATE 
comm_lib
)