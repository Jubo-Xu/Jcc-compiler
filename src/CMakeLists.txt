set(SOURCE_FILE Jcc.cpp)
set(TARGET_NAME JCC)


add_executable(${TARGET_NAME} ${SOURCE_FILE})
# add_custom_target(SECOND_TARGET_NAME
#     COMMAND chmod +x SECOND_SOURCE_FILE
#     WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
#     COMMENT "Building test executable"
# )

