#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "GSL::gsl" for configuration "Debug"
set_property(TARGET GSL::gsl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(GSL::gsl PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/gsl.lib"
  )

list(APPEND _cmake_import_check_targets GSL::gsl )
list(APPEND _cmake_import_check_files_for_GSL::gsl "${_IMPORT_PREFIX}/lib/gsl.lib" )

# Import target "GSL::gslcblas" for configuration "Debug"
set_property(TARGET GSL::gslcblas APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(GSL::gslcblas PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/gslcblas.lib"
  )

list(APPEND _cmake_import_check_targets GSL::gslcblas )
list(APPEND _cmake_import_check_files_for_GSL::gslcblas "${_IMPORT_PREFIX}/lib/gslcblas.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
