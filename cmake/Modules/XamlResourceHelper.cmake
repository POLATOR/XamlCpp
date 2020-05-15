# target_add_rc(target FILES <file1> [file2 ...] DESTINATION <path> DEPENDS <depend1> [depend2 ...] WORKING_DIRECTORY)
function(target_add_rc target)
    set(TARGET_ADD_RC_ONE_VALUE_ARGS DESTINATION WORKING_DIRECTORY)
    set(TARGET_ADD_RC_MULTI_VALUE_ARGS FILES DEPENDS)
    cmake_parse_arguments(TARGET_ADD_RC "" "${TARGET_ADD_RC_ONE_VALUE_ARGS}" "${TARGET_ADD_RC_MULTI_VALUE_ARGS}" ${ARGN})
    add_custom_command(
        OUTPUT ${TARGET_ADD_RC_DESTINATION}
        DEPENDS ${TARGET_ADD_RC_DEPENDS} ${TARGET_ADD_RC_FILES}
        COMMAND ${XAMLRC_PATH} ${TARGET_ADD_RC_FILES} -o ${TARGET_ADD_RC_DESTINATION} --no-logo
        WORKING_DIRECTORY ${TARGET_ADD_RC_WORKING_DIRECTORY}
    )
    target_sources(${target} PRIVATE ${TARGET_ADD_RC_DESTINATION})
endfunction()
