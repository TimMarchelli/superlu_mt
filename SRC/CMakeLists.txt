
file (GLOB sources "*.c")

if(Threads_FOUND)
  add_library(superlu_mt${PT} ${sources})
endif()

if(OpenMP_FOUND)
  add_library(superlu_mt${OMP} ${sources})
endif()

# set all common definitions for the two libraries
foreach(SUF ${PT} ${OMP})
  # use PIC for shared libraries
  if(UNIX)
    set_target_properties(superlu_mt${SUF} PROPERTIES POSITION_INDEPENDENT_CODE ON)
  endif()

  set_property(TARGET superlu_mt${SUF} PROPERTY C_STANDARD 99)

  if(HAVE_LIB_M)
    target_link_libraries (superlu_mt${SUF} PRIVATE m)
  endif ()
  
  target_link_libraries(superlu_mt${SUF} PRIVATE ${BLAS_LIB})
  target_compile_definitions(superlu_mt${SUF} PRIVATE Add_)

  if(LONGINT)
    target_compile_definitions (superlu_mt${SUF} PUBLIC _LONGINT)
  endif()

  target_include_directories(superlu_mt${SUF}
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
      $<INSTALL_INTERFACE:include>
  )

  # create an alias
  add_library(superlu_mt::superlu_mt${SUF} ALIAS superlu_mt${SUF})
endforeach()

if(Threads_FOUND)
  target_compile_definitions(superlu_mt${PT} PUBLIC __PTHREAD)
  target_link_libraries(superlu_mt${PT} PRIVATE ${CMAKE_THREAD_LIBS_INIT})
endif()
if(OpenMP_FOUND)
  target_compile_definitions(superlu_mt${OMP} PUBLIC __OPENMP)
  target_link_libraries (superlu_mt${OMP} PRIVATE OpenMP::OpenMP_C)
endif ()

# Install targets
foreach(SUF ${PT} ${OMP})
  install(TARGETS superlu_mt${SUF} DESTINATION ${CMAKE_INSTALL_LIBDIR})
endforeach()

file(GLOB headers "*.h")
install(FILES ${headers} DESTINATION ${INSTALL_INCLUDE_DIR})

# install suplerlu_mt
if(Threads_FOUND AND OpenMP_FOUND)
  install(TARGETS superlu_mt_PTHREAD superlu_mt_OPENMP
    EXPORT superlu_mtTargets
    RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
    INCLUDES DESTINATION ${INSTALL_INCLUDE_DIR}
  )
elseif(Threads_FOUND)
  install(TARGETS superlu_mt_PTHREAD
    EXPORT superlu_mtTargets
    RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
    INCLUDES DESTINATION ${INSTALL_INCLUDE_DIR}
  )
else()
  install(TARGETS superlu_mt_OPENMP
    EXPORT superlu_mtTargets
    RUNTIME DESTINATION ${INSTALL_RUNTIME_DIR}
    LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR}
    ARCHIVE DESTINATION ${INSTALL_LIBRARY_DIR}
    INCLUDES DESTINATION ${INSTALL_INCLUDE_DIR}
  )
endif()

install(FILES ${headers}
  DESTINATION ${INSTALL_INCLUDE_DIR}
)

# generate config files
include(CMakePackageConfigHelpers)
configure_package_config_file(
  superlu_mtConfig.cmake.in superlu_mtConfig.cmake
  INSTALL_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/superlu_mtConfig.cmake"
)

# install configuration files
install(EXPORT superlu_mtTargets
  FILE superlu_mtTargets.cmake
  NAMESPACE superlu_mt::
  DESTINATION ${INSTALL_CONFIG_DIR}
)

install(
  FILES
      "${CMAKE_CURRENT_BINARY_DIR}/superlu_mtConfig.cmake"
  DESTINATION ${INSTALL_CONFIG_DIR}
)