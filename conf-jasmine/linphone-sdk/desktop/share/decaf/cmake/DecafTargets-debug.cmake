#----------------------------------------------------------------
# Generated CMake target import file for configuration "Debug".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "decaf" for configuration "Debug"
set_property(TARGET decaf APPEND PROPERTY IMPORTED_CONFIGURATIONS DEBUG)
set_target_properties(decaf PROPERTIES
  IMPORTED_IMPLIB_DEBUG "${_IMPORT_PREFIX}/lib/decaf.lib"
  IMPORTED_LOCATION_DEBUG "${_IMPORT_PREFIX}/bin/decaf.dll"
  )

list(APPEND _IMPORT_CHECK_TARGETS decaf )
list(APPEND _IMPORT_CHECK_FILES_FOR_decaf "${_IMPORT_PREFIX}/lib/decaf.lib" "${_IMPORT_PREFIX}/bin/decaf.dll" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
