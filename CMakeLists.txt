cmake_minimum_required (VERSION 3.13)

# Project Version
project(SuperLU_MT C)
set(VERSION_MAJOR "4")
set(VERSION_MINOR "0")
set(VERSION_BugFix "0")
set(PROJECT_VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_BugFix})

# Compatiblitly to xSDK standard
# (Extreme-scale Scientific Software Development Kit)
set(USE_XSDK_DEFAULTS TRUE)

# The XSDK standard does not allow using internally built BLAS
if (NOT "${enable_internal_blaslib}" STREQUAL "")
  if (USE_XSDK_DEFAULTS)
    set(enable_blaslib_xSDK OFF)
  else()
    set(enable_blaslib_xSDK ON)
  endif()
else()
  set(enable_blaslib_xSDK ${enable_internal_blaslib})
endif()

if (NOT "${enable_fortran}" STREQUAL "")
  if (XSDK_ENABLE_Fortran)
    set(enable_fortran_xSDK ON)
  else()
    set(enable_fortran_xSDK OFF)
  endif()
else()
  set(enable_fortran_xSDK ${enable_fortran})
endif()

set (CMAKE_BUILD_TYPE Release CACHE STRING "Build type")

# set up options
option(enable_internal_blaslib  "Build the CBLAS library" ${enable_blaslib_xSDK})
option(enable_single    "Enable single precision library" ON)
option(enable_double    "Enable double precision library" ON)
option(enable_complex   "Enable complex precision library" ON)
option(enable_complex16 "Enable complex16 precision library" ON)
option(enable_matlabmex "Build the Matlab mex library" OFF)
option(enable_doc       "Add target 'doc' to build Doxygen documentation" OFF)
option(enable_examples  "Build examples" ON)
option(enable_fortran   "Build Fortran interface" ${enable_fortran_xSDK})
option(enable_tests     "Build tests" ON)
option(BUILD_SHARED_LIBS "shared/static" OFF)

option (LONGINT "use 64-bit integers for indexing sparse matrices (default is 32-bit)" OFF)

#-- BLAS
option(TPL_ENABLE_INTERNAL_BLASLIB  "Build the CBLAS library" ${enable_internal_blaslib})
option(TPL_BLAS_LIBRARIES "List of absolute paths to blas libraries [].")

# global modules
include(CTest)
include(GNUInstallDirs)
include(FeatureSummary)
include(CheckLibraryExists)
include(cmake/DisableTargetCompilerWarnings.cmake)

# set install targets
set(INSTALL_RUNTIME_DIR ${CMAKE_INSTALL_BINDIR})
set(INSTALL_CONFIG_DIR  ${CMAKE_INSTALL_LIBDIR}/cmake/superlu_mt)
set(INSTALL_PKGCONFIG_DIR  ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
set(INSTALL_LIBRARY_DIR ${CMAKE_INSTALL_LIBDIR})
set(INSTALL_INCLUDE_DIR ${CMAKE_INSTALL_INCLUDEDIR}/superlu_mt)

check_library_exists(m sin "" HAVE_LIB_M)

# export all symbols
if(BUILD_SHARED_LIBS)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
endif()

#---- set RPATH handling
if(NOT APPLE)
  set(CMAKE_INSTALL_RPATH $origin)
endif()

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# set a debug postfix
set(CMAKE_DEBUG_POSTFIX "d")

######################################################################
#
# Find packages
#
######################################################################
#

#--------------------- BLAS ---------------------
if(NOT TPL_ENABLE_INTERNAL_BLASLIB)
  # Use explicit supplied BLAS library
  if (TPL_BLAS_LIBRARIES)
    set(BLAS_FOUND TRUE)
    message("-- Using TPL_BLAS_LIBRARIES='${TPL_BLAS_LIBRARIES}'")
    set(BLAS_LIB ${TPL_BLAS_LIBRARIES})

  # use cmake to find BLAS
  else()
    find_package(BLAS)

    # if found, set the BLAS_LIB for linking into the library later
    if (BLAS_FOUND)
      
      set(BLAS_LIB BLAS::BLAS)

      # logic to determine type of BLAS and import it directly if possible. Importing directly populates more 
      # information about the import library than FindBLAS does. This is important in windows where we need to 
      # know the location of the imported DLL.
      if(BLAS_LIBRARIES MATCHES "[Oo][Pp][Ee][Nn][Bb][Ll][Aa][Ss]")
        message(STATUS "Using OpenBLAS import library.")
        find_package(OpenBLAS REQUIRED)
        set(BLAS_LIB OpenBLAS::OpenBLAS)

      # add more logic to directly import other BLAS implementations
      else()
        message(STATUS "Using cmake found BLAS.")
      endif()
    endif()

  endif()
endif()

# Build the internal BLAS as static library
if(NOT BLAS_FOUND)
  message("-- Did not find or specify BLAS so configure to build internal CBLAS ...")
  #add_subdirectory(CBLAS)
  set(BLAS_LIB blas)
endif()

# get threads and openmp packages
find_package(Threads)
if(Threads_FOUND)
  if(CMAKE_USE_PTHREADS_INIT)
    set(PT "_PTHREAD")
  else()
    message(STATUS "No pthreads, disabling threads version.")
    unset(Threads_FOUND)
  endif()
endif()

find_package(OpenMP)
if(OpenMP_FOUND)
  set(OMP "_OPENMP")
endif()
if(NOT (Threads_FOUND OR OpenMP_FOUND))
  message(SEND_ERROR "missing Threads and OpenMP")
endif()

# get wingetopt package
find_package(wingetopt)

enable_language(C)
if(enable_fortran)
  enable_language(Fortran)
endif()
set(SUPERLU_MT_VERSION "${PROJECT_VERSION}")

add_subdirectory(SRC)
add_subdirectory(EXAMPLE)

if(enable_tests)
  enable_testing()
  add_subdirectory(TESTING)
endif()

if (enable_doc)
   add_subdirectory(DOC)
endif()

