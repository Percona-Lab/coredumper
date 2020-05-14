
add_library(
  libcoredumper
  coredumper.c
  thread_lister.c
  elfcore.c
  linuxthreads.c
  )

target_include_directories(
  libcoredumper PUBLIC 
  $<BUILD_INTERFACE:${COREDUMPER_SRC_DIR}/include/>
  $<INSTALL_INTERFACE:include>
  )
set_target_properties(
  libcoredumper PROPERTIES PUBLIC_HEADER
  "../include/coredumper/coredumper.h"
  )

target_link_libraries(libcoredumper Threads::Threads)


include(CheckCCompilerFlag)
check_c_compiler_flag("-fno-strict-aliasing" FLAG_NO_STRICT_ALIASING)
if(FLAG_NO_STRICT_ALIASING)
  target_compile_options(libcoredumper PUBLIC "-fno-strict-aliasing")
endif()

export(TARGETS libcoredumper FILE "libcoredumper.cmake")
install(
  TARGETS libcoredumper
  EXPORT libcoredumper
  PUBLIC_HEADER DESTINATION include/coredumper
  LIBRARY DESTINATION lib)
install(EXPORT libcoredumper DESTINATION cmake)
