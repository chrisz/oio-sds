add_definitions(-DG_LOG_DOMAIN="oio.gridd.stats")

include_directories(BEFORE . ../.. ../../..)

add_library(msg_stats MODULE msg_stats.c msg_stats.h)
set_target_properties(msg_stats PROPERTIES PREFIX "" SUFFIX .so)
target_link_libraries(msg_stats metautils ${GLIB2_LIBRARIES})

install(TARGETS msg_stats
        LIBRARY DESTINATION ${GRIDD_PLUGINS_DIRECTORY}
        RUNTIME DESTINATION bin)

