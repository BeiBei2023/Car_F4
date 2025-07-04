cmake_minimum_required(VERSION 3.22)

#
# 该文件仅生成一次，
# 如果转换器被多次调用，则不会重新生成。
#
# 用户可以根据需要随意修改文件
#

# 设置编译器设置
set(CMAKE_C_STANDARD 11)    # 设置C++标准
set(CMAKE_C_STANDARD_REQUIRED ON)   # 设置C++标准为必需
set(CMAKE_C_EXTENSIONS ON)  # 设置C++扩展为必需


# 定义构建类型
if(NOT CMAKE_BUILD_TYPE)    # 如果未设置构建类型
    set(CMAKE_BUILD_TYPE "Debug")   # 设置默认构建类型为Debug
endif()

# Set the project name     # 设置项目名称
set(CMAKE_PROJECT_NAME ces) # 设置项目名称为ces

# Include toolchain file    # 包含工具链文件
include("cmake/gcc-arm-none-eabi.cmake")    # 包含gcc-arm-none-eabi工具链文件

# Enable compile command to ease indexing with e.g. clangd  # 启用编译命令以方便使用clangd等工具进行索引
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE) # 启用编译命令

# Enable CMake support for ASM and C languages  # 启用CMake对ASM和C语言的支持
enable_language(C ASM)  # 启用C和ASM语言

# Core project settings # 核心项目设置
project(${CMAKE_PROJECT_NAME})  # 设置项目名称
message("Build type: " ${CMAKE_BUILD_TYPE}) # 输出构建类型

# Create an executable object type  # 创建可执行对象类型
add_executable(${CMAKE_PROJECT_NAME}
        BSP/CALLBACK/uart_callback.h
        BSP/CALLBACK/uart_callback.c
        BSP/LOG/elog.c
        BSP/LOG/elog.h
        BSP/LOG/elog_utils.c
        BSP/LOG/elog_port.c
        BSP/LOG/elog_cfg.h
        BSP/MODBUS/MODBUS_BASE/modbus.c
        BSP/MODBUS/MODBUS_BASE/modbus.h
        BSP/PERIPHERAL/Peripheral_Management.c
        BSP/PERIPHERAL/Peripheral_Management.h
        BSP/MODBUS/MODBUS_CRC/modbus_crc.c
        BSP/MODBUS/MODBUS_CRC/modbus_crc.h
        BSP/MODBUS/MODBUS_INIT/modbus_init.c
        BSP/MODBUS/MODBUS_INIT/modbus_init.h
        BSP/MODBUS/MODBUS_FUNCTION/modbus_function_code.c
        BSP/MODBUS/MODBUS_FUNCTION/modbus_function_code.h
        BSP/MODBUS/Holding_Register/holding_register.c
        BSP/MODBUS/Holding_Register/holding_register.h
        BSP/AHT20/aht20.c
        BSP/AHT20/aht20.h
        BSP/ADC_VOLTAGE/system_voltage.c
        BSP/ADC_VOLTAGE/system_voltage.h
        BSP/MODBUS/Input_Register/input_register.c
        BSP/MODBUS/Input_Register/input_register.h
        BSP/MODBUS/updata_registers_value.c
        BSP/MODBUS/updata_registers_value.h
        BSP/MODBUS/MODBUS_COIL/coil.c
        BSP/MODBUS/MODBUS_COIL/coil.h
        BSP/MODBUS/CONFIG/modbus_config.h
        BSP/SYSTEM_RUN_TIME/system_run_time.c
        BSP/SYSTEM_RUN_TIME/system_run_time.h
        BSP/IMU_QMI8658A/qmi8658a.c
        BSP/IMU_QMI8658A/qmi8658a.h
        BSP/IMU_QMI8658A/kalman.c
        BSP/IMU_QMI8658A/kalman.h
        BSP/SBUS/sbus.c
        BSP/SBUS/sbus.h
        BSP/M3508/CAN/can_callback.c
        BSP/M3508/CAN/can_callback.h
        BSP/M3508/PID/pid.c
        BSP/M3508/PID/pid.h
        BSP/M3508/MOTOR/motor_command.c
        BSP/M3508/MOTOR/motor_command.h
        BSP/M3508/MOTOR/motor_control.c
        BSP/M3508/MOTOR/motor_control.h
        BSP/M3508/MOTOR/motor_target_speed.c
        BSP/M3508/MOTOR/motor_target_speed.h
        # 添加可执行文件
)

# Add STM32CubeMX generated sources # 添加STM32CubeMX生成的源文件
add_subdirectory(cmake/stm32cubemx) # 添加stm32cubemx子目录

# Link directories setup    # 链接目录设置
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE   # 链接目录
    # Add user defined library search paths # 添加用户定义的库搜索路径

)

# Add sources to executable # 添加源文件到可执行文件

target_sources(${CMAKE_PROJECT_NAME} PRIVATE    # 添加源文件
    # Add user sources here # 在这里添加用户源文件





)

# Add include paths # 添加包含路径
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE    # 添加包含路径
    # Add user defined include paths    # 添加用户定义的包含路径

        BSP/CALLBACK
        BSP/MODBUS
        BSP/MODBUS/CONFIG
        BSP/MODBUS/MODBUS_BASE
        BSP/MODBUS/MODBUS_INIT
        BSP/MODBUS/MODBUS_FUNCTION
        BSP/MODBUS/Holding_Register
        BSP/MODBUS/MODBUS_CRC
        BSP/MODBUS/Input_Register
        BSP/MODBUS/MODBUS_COIL
        BSP/LOG
        BSP/PERIPHERAL
        BSP/AHT20
        BSP/ADC_VOLTAGE
        BSP/SYSTEM_RUN_TIME
        BSP/IMU_QMI8658A
        BSP/SBUS
        BSP/M3508
        BSP/M3508/CAN
        BSP/M3508/PID
        BSP/M3508/MOTOR
)

# Add project symbols (macros)  # 添加项目符号（宏）
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE    # 添加项目符号（宏）
    # Add user defined symbols  # 添加用户定义的符号
)

# Add linked libraries  # 添加链接库
target_link_libraries(${CMAKE_PROJECT_NAME}     # 添加链接库
    stm32cubemx # 添加STM32CubeMX

    # Add user defined libraries    # 添加用户定义的库
)

#开启浮点打印支持
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -mfpu=fpv4-sp-d16 -mfloat-abi=hard")
set(CMAKE_C_LINK_FLAGS "${CMAKE_C_LINK_FLAGS} -mfpu=fpv4-sp-d16 -mfloat-abi=hard -u _printf_float")


