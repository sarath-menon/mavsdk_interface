
add_library(publish_position
include/publish_position_thread.h
src/publish_position_thread.cpp
)

target_include_directories(publish_position PUBLIC include)

# QT depenencies
target_link_libraries(publish_position PRIVATE 
Qt${QT_VERSION_MAJOR}::Widgets 
)

# mavsdk dependencies
target_link_libraries(publish_position PRIVATE 
    MAVSDK::mavsdk_telemetry
    MAVSDK::mavsdk_action
    MAVSDK::mavsdk_offboard
    MAVSDK::mavsdk
)


# Other dependencies
target_link_libraries(publish_position PRIVATE 
comm_lib
)