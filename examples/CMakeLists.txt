project(${PROJECT_NAME})

set(EXECUTABLE uart_firmware)
set(PROJECT_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(
	${EXECUTABLE}
##	uart_test.cpp
 ##"gpio_test.cpp"
 "bootloader_test.cpp")

target_link_libraries(
	${EXECUTABLE}
	PRIVATE
	cortexm3
)

set_target_properties(${EXECUTABLE} PROPERTIES SUFFIX ".elf")


target_include_directories(${EXECUTABLE} PRIVATE
    ${PROJECT_INCLUDE_DIRECTORIES})

#message(FATAL_ERROR ${PROJECT_INCLUDE_DIRECTORIES})
###############################################################################

target_compile_options(${EXECUTABLE} PRIVATE
    ${CPU_PARAMETERS}
    ${SOURCE_COMPILE_OPTIONS}
)

target_link_options(${EXECUTABLE} PRIVATE
    -T${MCU_LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    ${MCU_LINKER_OPTIONS}
    -Wl,-Map=${EXECUTABLE}.map
)

###############################################################################

# The last command can take a couple of seconds on larger project, usefull for debugging
add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${EXECUTABLE}>
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.hex
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${EXECUTABLE}> ${EXECUTABLE}.bin
    COMMAND "openocd" -f "board/stm32f103c8_blue_pill.cfg"
   ## COMMAND cmd post_build.bat
)

option(DUMP_ASM "Create full assembly of final executable" OFF)
if (${DUMP_ASM})
    add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_OBJDUMP} -D -C $<TARGET_FILE:${EXECUTABLE}> > ${EXECUTABLE}.s)
endif()