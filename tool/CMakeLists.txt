add_executable(
  coredumper_tool
  main.c
)

target_link_libraries(coredumper_tool coredumper)

# Needs access to internal headers
target_include_directories(
  coredumper_tool PRIVATE
  $<BUILD_INTERFACE:${COREDUMPER_SRC_DIR}/include/>
)
