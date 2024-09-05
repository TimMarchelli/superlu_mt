include_directories(${PROJECT_SOURCE_DIR}/SRC)

# Libs linked to all of the tests
add_subdirectory(MATGEN)

set(NVAL  10 19)
set(NRHS   2)
set(LWORK  0 100000000)
set(NPROCS 1 4)

# create a batch generator
# use with cmake -DTARGET=<batch file> -DDLL_PATH=<DLL directories> -DCMD=<test command> -P ${BATCH_GEN_SCRIPT}
if(WIN32)
  set(BATCH_GEN_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/create_batch.cmake)
  set(BATCH_FILE_CMD
  [=[
  file(WRITE "${TARGET}" "@ECHO OFF\nSET PATH=${DLL_PATH}\;%PATH%\n${CMD}")
  ]=])
  file(WRITE ${BATCH_GEN_SCRIPT} ${BATCH_FILE_CMD})
endif()

function(add_superlumt_test target input)
  set(TEST_INPUT "${PROJECT_SOURCE_DIR}/EXAMPLE/${input}")
  foreach (s ${NRHS})
    foreach(l ${LWORK})
      foreach (p ${NPROCS})
        # LA tests
	      foreach (n ${NVAL})
          set(testName "${target}_${p}_${n}_${s}_${l}")
          if(UNIX)
            add_test(
              NAME ${testName}_LA
              COMMAND $<TARGET_FILE:${target}> -t "LA" -n ${n} -s ${s} -l ${l} -p ${p} 
            )

          elseif(WIN32)
            # Create a batch file with path to dependent dlls
            set(test_batch_file ${CMAKE_CURRENT_BINARY_DIR}/${testName}_LA.bat)
            add_custom_command(TARGET ${target} POST_BUILD
              COMMAND ${CMAKE_COMMAND} 
                "-DTARGET=${test_batch_file}"
                "-DDLL_PATH=$<SHELL_PATH:$<TARGET_RUNTIME_DLL_DIRS:${target}>>"
                "-DCMD=\"\\\"$<SHELL_PATH:$<TARGET_FILE:${target}>>\\\" -t LA -n ${n} -s ${s} -l ${l} -p ${p}\""
                -P ${BATCH_GEN_SCRIPT}
              DEPENDS ${BATCH_GEN_SCRIPT}
            )
            
            # Call the batch file for the test
            add_test(
              NAME ${testName}_LA
              COMMAND ${CMAKE_COMMAND} -E env -- cmd.exe /c "${test_batch_file}")
          endif()
        endforeach()

        # SP tests
        ## CTest commands are not run in a shell's context, cannot use redirection,
        ## so, need "sh" as the command to be called with add_test()		
        set(testName "${target}_${p}_${s}_${l}")
        if(UNIX)
          add_test(
            NAME ${testName}_SP
            COMMAND sh -c $<TARGET_FILE:${target}> -t "SP" -s ${s} -l ${l} -p ${p} < ${TEST_INPUT} 
          )

        elseif(WIN32)
          # Create a batch file with path to dependent dlls
          set(test_batch_file ${CMAKE_CURRENT_BINARY_DIR}/${testName}_SP.bat)
          add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} 
              "-DTARGET=${test_batch_file}"
              "-DDLL_PATH=$<SHELL_PATH:$<TARGET_RUNTIME_DLL_DIRS:${target}>>"
              "-DCMD=\"\\\"$<SHELL_PATH:$<TARGET_FILE:${target}>>\\\" -t SP -s ${s} -l ${l} -p ${p} < \\\"$<SHELL_PATH:${TEST_INPUT}>\\\"\""
              -P ${BATCH_GEN_SCRIPT}
            DEPENDS ${BATCH_GEN_SCRIPT}
          )

          # Call the batch file for the test
          add_test(
            NAME ${testName}_SP
            COMMAND ${CMAKE_COMMAND} -E env -- cmd.exe /c "${test_batch_file}"
          )
        endif()
      endforeach()
    endforeach()
  endforeach()

endfunction(add_superlumt_test)

# prevent creating testing for Windows if WinGetOps is not found
if(MSVC AND NOT wingetopt_FOUND)
  message("Disabled tests pstest, pdtest, pctest, and pztest because optinal dependency WinGetOpt is missing")
  return()
endif()

foreach(SUF ${PT} ${OMP})
  set(test_link_libs matgen${SUF})

  if(enable_single)
    add_executable(pstest${SUF}
      psdrive.c sp_sconvert.c psgst01.c psgst02.c psgst04.c psgst07.c
              psgssv.c psgssvx.c sp_ienv.c )
    target_link_libraries(pstest${SUF} ${test_link_libs})
    disable_target_compiler_warnings(pstest${SUF})
    add_superlumt_test(pstest${SUF} g10)
  endif()


  if(enable_double)
    add_executable(pdtest${SUF}
      pddrive.c sp_dconvert.c pdgst01.c pdgst02.c pdgst04.c pdgst07.c
      pdgssv.c pdgssvx.c sp_ienv.c )
    target_link_libraries(pdtest${SUF} ${test_link_libs})
    disable_target_compiler_warnings(pdtest${SUF})
    add_superlumt_test(pdtest${SUF} g10)
  endif()

  if(enable_complex)
    add_executable(pctest${SUF}
      pcdrive.c sp_cconvert.c pcgst01.c pcgst02.c pcgst04.c pcgst07.c
              pcgssv.c pcgssvx.c sp_ienv.c )
    target_link_libraries(pctest${SUF} ${test_link_libs})
    disable_target_compiler_warnings(pctest${SUF})
    add_superlumt_test(pctest${SUF} cg20.cua)
  endif()

  if(enable_complex16)
    add_executable(pztest${SUF}
      pzdrive.c sp_zconvert.c pzgst01.c pzgst02.c pzgst04.c pzgst07.c
      pzgssv.c pzgssvx.c sp_ienv.c )
    target_link_libraries(pztest${SUF} ${test_link_libs})
    disable_target_compiler_warnings(pztest${SUF})
    add_superlumt_test(pztest${SUF} cg20.cua)
  endif()
endforeach()

if(MSVC)
  get_property(targets DIRECTORY "." PROPERTY BUILDSYSTEM_TARGETS)
  foreach(target ${targets})
    get_target_property(target_type ${target} TYPE)
    if(target_type STREQUAL "EXECUTABLE")
      target_link_libraries(${target} wingetopt::wingetopt)
    endif()
  endforeach()
endif()
