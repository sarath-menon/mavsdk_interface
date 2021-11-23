
# ###############################################################################
# # Library of behaviours
# ###############################################################################


add_library(threads INTERFACE)

target_link_libraries(threads INTERFACE
offboard
)

add_subdirectory(offboard)
