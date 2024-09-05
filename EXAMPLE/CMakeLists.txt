include_directories(${PROJECT_SOURCE_DIR}/SRC)

# build examples for target "all" only if enable_examples is set
if(NOT enable_examples)
  set(_DEPENDENCY_ALL "EXCLUDE_FROM_ALL")
endif()

# build for both omp and pthread
foreach(SUF ${PT} ${OMP})
  # targets to build examples
  add_custom_target(examples_complex${SUF})
  add_dependencies(examples_complex${SUF}
      pclinsol${SUF} pclinsolx${SUF} pclinsolx1${SUF} pclinsolx2${SUF} pcrepeat${SUF} pcspmd${SUF})
      
  add_custom_target(examples_double${SUF})
  add_dependencies(examples_double${SUF}
      pdlinsol${SUF} pdlinsolx${SUF} pdlinsolx1${SUF} pdlinsolx2${SUF} pdrepeat${SUF} pdspmd${SUF})
      
  add_custom_target(examples_float${SUF})
  add_dependencies(examples_float${SUF}
      pslinsol${SUF} pslinsolx${SUF} pslinsolx1${SUF} pslinsolx2${SUF} psrepeat${SUF} psspmd${SUF})
      
  add_custom_target(examples_doublecomplex${SUF})
  add_dependencies(examples_doublecomplex${SUF}
      pzlinsol${SUF} pzlinsolx${SUF} pzlinsolx1${SUF} pzlinsolx2${SUF} pzrepeat${SUF} pzspmd${SUF})

  add_custom_target(examples${SUF})
  add_dependencies(examples${SUF}
                  examples_complex${SUF}
                  examples_double${SUF}
                  examples_float${SUF}
                  examples_doublecomplex${SUF})


  # examples for complex
  add_executable(pclinsol${SUF}  ${_DEPENDENCY_ALL} pclinsol.c)
  target_link_libraries(pclinsol${SUF} superlu_mt${SUF})

  add_executable(pclinsolx${SUF}  ${_DEPENDENCY_ALL} pclinsolx.c)
  target_link_libraries(pclinsolx${SUF} superlu_mt${SUF})

  add_executable(pclinsolx1${SUF}  ${_DEPENDENCY_ALL} pclinsolx1.c)
  target_link_libraries(pclinsolx1${SUF} superlu_mt${SUF})

  add_executable(pclinsolx2${SUF}  ${_DEPENDENCY_ALL} pclinsolx2.c)
  target_link_libraries(pclinsolx2${SUF} superlu_mt${SUF})

  add_executable(pcrepeat${SUF}  ${_DEPENDENCY_ALL} pcrepeat.c)
  target_link_libraries(pcrepeat${SUF} superlu_mt${SUF})

  add_executable(pcspmd${SUF}  ${_DEPENDENCY_ALL} pcspmd.c)
  target_link_libraries(pcspmd${SUF} superlu_mt${SUF})

  # examples for double
  add_executable(pdlinsol${SUF}  ${_DEPENDENCY_ALL} pdlinsol.c)
  target_link_libraries(pdlinsol${SUF} superlu_mt${SUF})

  add_executable(pdlinsolx${SUF}  ${_DEPENDENCY_ALL} pdlinsolx.c)
  target_link_libraries(pdlinsolx${SUF} superlu_mt${SUF})

  add_executable(pdlinsolx1${SUF}  ${_DEPENDENCY_ALL} pdlinsolx1.c)
  target_link_libraries(pdlinsolx1${SUF} superlu_mt${SUF})

  add_executable(pdlinsolx2${SUF}  ${_DEPENDENCY_ALL} pdlinsolx2.c)
  target_link_libraries(pdlinsolx2${SUF} superlu_mt${SUF})

  add_executable(pdrepeat${SUF}  ${_DEPENDENCY_ALL} pdrepeat.c)
  target_link_libraries(pdrepeat${SUF} superlu_mt${SUF})

  add_executable(pdspmd${SUF}  ${_DEPENDENCY_ALL} pdspmd.c)
  target_link_libraries(pdspmd${SUF} superlu_mt${SUF})

  # examples for float
  add_executable(pslinsol${SUF}  ${_DEPENDENCY_ALL} pslinsol.c)
  target_link_libraries(pslinsol${SUF} superlu_mt${SUF})

  add_executable(pslinsolx${SUF}  ${_DEPENDENCY_ALL} pslinsolx.c)
  target_link_libraries(pslinsolx${SUF} superlu_mt${SUF})

  add_executable(pslinsolx1${SUF}  ${_DEPENDENCY_ALL} pslinsolx1.c)
  target_link_libraries(pslinsolx1${SUF} superlu_mt${SUF})

  add_executable(pslinsolx2${SUF}  ${_DEPENDENCY_ALL} pslinsolx2.c)
  target_link_libraries(pslinsolx2${SUF} superlu_mt${SUF})

  add_executable(psrepeat${SUF}  ${_DEPENDENCY_ALL} psrepeat.c)
  target_link_libraries(psrepeat${SUF} superlu_mt${SUF})

  add_executable(psspmd${SUF}  ${_DEPENDENCY_ALL} psspmd.c)
  target_link_libraries(psspmd${SUF} superlu_mt${SUF})


  # examples for double complex
  add_executable(pzlinsol${SUF}  ${_DEPENDENCY_ALL} pzlinsol.c)
  target_link_libraries(pzlinsol${SUF} superlu_mt${SUF})

  add_executable(pzlinsolx${SUF}  ${_DEPENDENCY_ALL} pzlinsolx.c)
  target_link_libraries(pzlinsolx${SUF} superlu_mt${SUF})

  add_executable(pzlinsolx1${SUF}  ${_DEPENDENCY_ALL} pzlinsolx1.c)
  target_link_libraries(pzlinsolx1${SUF} superlu_mt${SUF})

  add_executable(pzlinsolx2${SUF}  ${_DEPENDENCY_ALL} pzlinsolx2.c)
  target_link_libraries(pzlinsolx2${SUF} superlu_mt${SUF})

  add_executable(pzrepeat${SUF}  ${_DEPENDENCY_ALL} pzrepeat.c)
  target_link_libraries(pzrepeat${SUF} superlu_mt${SUF})

  add_executable(pzspmd${SUF}  ${_DEPENDENCY_ALL} pzspmd.c)
  target_link_libraries(pzspmd${SUF} superlu_mt${SUF})
endforeach()

# disable compiler warnings
get_property(_targets DIRECTORY "." PROPERTY BUILDSYSTEM_TARGETS)
foreach(_target ${_targets})
  get_target_property(_target_type ${_target} TYPE)
  if(_target_type STREQUAL "EXECUTABLE")
    disable_target_compiler_warnings(${_target})
  endif()
endforeach()
unset(_target_type)
unset(_target)
unset(_targets)

if(MSVC AND wingetopt_FOUND)
  get_property(_targets DIRECTORY "." PROPERTY BUILDSYSTEM_TARGETS)
  foreach(_target ${_targets})
    get_target_property(_target_type ${_target} TYPE)
    if(_target_type STREQUAL "EXECUTABLE")
      target_link_libraries(${_target} wingetopt::wingetopt)
    endif()
  endforeach()
  unset(_target_type)
  unset(_target)
  unset(_targets)
endif()

# copy DLLS for windows
if(WIN32)
  # repeat for PTHREAD and OpenMP version
  foreach(SUF ${PT} ${OMP})
    # get a target, only need to do this once
    get_property(_targets DIRECTORY "." PROPERTY BUILDSYSTEM_TARGETS)
    
    foreach(_target ${_targets})
      # only add copy command to an executable target
      get_target_property(_val ${_target} TYPE)

      if(_val MATCHES "EXECUTABLE" AND _target MATCHES ${SUF})
        add_custom_command(TARGET ${_target} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${_target}> $<TARGET_RUNTIME_DLLS:${_target}>
          COMMAND_EXPAND_LISTS
        )
        break()
      endif()  
    endforeach()

    unset(_val)
    unset(_target)
    unset(_targets)
  endforeach()
endif()