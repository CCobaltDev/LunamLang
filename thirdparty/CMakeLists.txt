add_subdirectory(argparse)
add_subdirectory(fast_float)
add_subdirectory(fmt)
add_subdirectory(magic_enum)
set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "")
add_subdirectory(spdlog)

target_include_directories(${PROJECT_NAME} PRIVATE utfcpp/source)
target_link_libraries(${PROJECT_NAME} PRIVATE argparse fast_float fmt magic_enum spdlog)