project(${PROJECT_NAME})


add_library(
	cortexm3
	"driver_uart.cpp"
  "core_system.cpp" "core_scb.cpp" 
  "core_rcc.cpp" "core_pwr.cpp" 
  "core_nvic.cpp" "core_time.cpp" 
  "driver_port.cpp" "common_defs.cpp" 
  "driver_adc.cpp" "driver_timer.cpp"
 "app_pwm.cpp" "driver_spi.cpp" "app_stream.cpp" "driver_flash.cpp" "app_bootloader.cpp" "driver_wifi.cpp")

target_include_directories(
	cortexm3
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/../include
)