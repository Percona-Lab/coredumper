
add_library(
  coredumper
  coredumper.c
  thread_lister.c
  elfcore.c
  linuxthreads.c
  )

target_include_directories(
  coredumper PUBLIC
  $<BUILD_INTERFACE:${COREDUMPER_SRC_DIR}/include/>
  $<INSTALL_INTERFACE:include>
  )
set_target_properties(
  coredumper PROPERTIES PUBLIC_HEADER
  "../include/coredumper/coredumper.h"
  )

target_link_libraries(coredumper Threads::Threads)


include(CheckCCompilerFlag)
check_c_compiler_flag("-fno-strict-aliasing" FLAG_NO_STRICT_ALIASING)
if(FLAG_NO_STRICT_ALIASING)
  target_compile_options(coredumper PUBLIC "-fno-strict-aliasing")
endif()

export(TARGETS coredumper FILE "coredumper.cmake")
install(
  TARGETS coredumper
  EXPORT coredumper
  PUBLIC_HEADER DESTINATION include/coredumper
  LIBRARY DESTINATION lib)
install(EXPORT coredumper DESTINATION cmake)
