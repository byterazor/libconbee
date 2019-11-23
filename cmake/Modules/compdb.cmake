find_program(COMPDB_PATH
      NAME compdb
      PATHS ~/.local/bin/
            /bin
            /sbin
            /usr/bin
            /usr/sbin
            /usr/local/bin
            /usr/local/sbin
  )

if (COMPDB_PATH)
  execute_process(
    COMMAND ${COMPDB_PATH} -p ${CMAKE_CURRENT_BINARY_DIR} list
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_FILE compile_commands.json
  )
endif()
