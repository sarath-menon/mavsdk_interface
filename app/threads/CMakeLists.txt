
# ###############################################################################
# # Library of behaviours
# ###############################################################################


add_library(threads INTERFACE)

target_link_libraries(threads INTERFACE
offboard
publish_position
)

add_subdirectory(offboard)
add_subdirectory(publish_position)
