set(TARGET_NAME testapp)
set(TARGET ${TARGET_NAME}.elf)

set(LD_SCRIPT ${TESTAPP_ROOT}/testapp.ld)

set(SOURCE_FILE_PATHS
    ${NANOBOOT_SOURCES}
    ${TESTAPP_ROOT}/main.cpp
)

set(INCLUDE_PATHS
    ${NANOBOOT_INCLUDES}
)

# Add a new executable that we want CMake to compile
add_executable(${TARGET} ${SOURCE_FILE_PATHS})

target_include_directories(${TARGET} PUBLIC
    ${INCLUDE_PATHS}
)

target_compile_definitions(${TARGET} PRIVATE ${NANOBOOT_DEFINES})

set_target_properties(${TARGET} PROPERTIES LINK_DEPENDS ${LD_SCRIPT})

# Import all compile options defined in the toolchain
target_compile_options(${TARGET} PUBLIC ${NANOBOOT_COMPILE_FLAGS})

target_compile_options(${TARGET} PUBLIC -Os)

# Import all link options defined in the toolchain
target_link_libraries(${TARGET} ${NANOBOOT_LINK_FLAGS})

# ARM link libraries
target_link_libraries(${TARGET} --specs=nosys.specs)
target_link_libraries(${TARGET} --specs=nano.specs)
target_link_libraries(${TARGET} -Wl,--start-group)
target_link_libraries(${TARGET} m)
target_link_libraries(${TARGET} c)
target_link_libraries(${TARGET} nosys)
target_link_libraries(${TARGET} gcc)
target_link_libraries(${TARGET} stdc++)
target_link_libraries(${TARGET} -Wl,--end-group)
target_link_libraries(${TARGET} -Wl,-Map=${TARGET_NAME}.map)
target_link_libraries(${TARGET} -Wl,--gc-sections)


#target_link_libraries(${TARGET} -L${GCC_LD_SEARCH})

target_link_libraries(${TARGET} -T${LD_SCRIPT})
target_link_libraries(${TARGET} --verbose)

set(HEX_FILE ${TARGET_NAME}.hex)
set(BIN_FILE ${TARGET_NAME}.bin)
add_custom_command(TARGET ${TARGET} POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${TARGET_NAME}.elf> ${HEX_FILE}
    COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${TARGET_NAME}.elf> ${BIN_FILE}
    COMMENT "Building ${HEX_FILE} \nBuilding ${BIN_FILE}")