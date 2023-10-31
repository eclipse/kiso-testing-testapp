find_package(Python3 3.7 COMPONENTS Interpreter REQUIRED)

add_custom_command(
    OUTPUT
        ${PROJECT_BINARY_DIR}/memory_usage/memory_usage.html
        ${PROJECT_BINARY_DIR}/memory_usage/memory_usage.csv
    DEPENDS
        ${PROJECT_NAME}.elf
    COMMAND
        ${Python3_EXECUTABLE} ${CMAKE_CURRENT_LIST_DIR}/tools/MemoryUsage/memory_usage.py 
        --directory ${PROJECT_BINARY_DIR} 
        --map-file ${PROJECT_BINARY_DIR}/Platform/stm32l562qe/src/System/${PROJECT_NAME}.map
        --html ${PROJECT_BINARY_DIR}/memory_usage/memory_usage.html --csv ${PROJECT_BINARY_DIR}/memory_usage/memory_usage.csv
    COMMENT "Generating html and csv"
)

add_custom_target(memory_usage
    COMMENT "Analyzing Memory Usage..."
    DEPENDS
    ${PROJECT_BINARY_DIR}/memory_usage/memory_usage.html
    ${PROJECT_BINARY_DIR}/memory_usage/memory_usage.csv
)
