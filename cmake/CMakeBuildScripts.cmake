add_custom_command(TARGET FOrMit POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/templates"
        "${CMAKE_BINARY_DIR}/templates"
        COMMENT "Phrase > Copy templates"
)