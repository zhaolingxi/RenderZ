#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "asl" for configuration "Debug"
set_property(TARGET asl APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(asl PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/asl.lib"
  )

list(APPEND _cmake_import_check_targets asl )
list(APPEND _cmake_import_check_files_for_asl "${_IMPORT_PREFIX}/lib/asl.lib" )

# Import target "asl2" for configuration "Debug"
set_property(TARGET asl2 APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(asl2 PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_DEBUG "C"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/lib/asl2.lib"
  )

list(APPEND _cmake_import_check_targets asl2 )
list(APPEND _cmake_import_check_files_for_asl2 "${_IMPORT_PREFIX}/lib/asl2.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
