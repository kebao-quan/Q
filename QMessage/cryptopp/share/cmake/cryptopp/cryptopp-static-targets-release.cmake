#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "cryptopp::cryptopp" for configuration "Release"
set_property(TARGET cryptopp::cryptopp APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(cryptopp::cryptopp PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "ASM_MASM;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/cryptopp.lib"
  )

list(APPEND _IMPORT_CHECK_TARGETS cryptopp::cryptopp )
list(APPEND _IMPORT_CHECK_FILES_FOR_cryptopp::cryptopp "${_IMPORT_PREFIX}/lib/cryptopp.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
