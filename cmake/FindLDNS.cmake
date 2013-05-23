find_path (LDNS_INCLUDE_DIR NAMES ldns/ldns.h)
mark_as_advanced(LDNS_INCLUDE_DIR)

find_library(LDNS_LIBRARY NAMES
  ldns
  libldns
  )
mark_as_advanced(LDNS_LIBRARY)

if(LDNS_INCLUDE_DIR)
  foreach(_ldns_version_header util.h)
    if(EXISTS "${LDNS_INCLUDE_DIR}/ldns/${_ldns_version_header}")
      file(STRINGS "${LDNS_INCLUDE_DIR}/ldns/${_ldns_version_header}" ldns_version_str REGEX "^#define[\t ]+LDNS_VERSION[\t ]+\".*\"")

      string(REGEX REPLACE "^#define[\t ]+LDNS_VERSION[\t ]+\"([^\"]*)\".*" "\\1" LDNS_VERSION_STRING "${ldns_version_str}")
      unset(ldns_version_str)
      break()
    endif()
  endforeach()
endif()

include(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(LDNS
                                  REQUIRED_VARS LDNS_LIBRARY LDNS_INCLUDE_DIR
                                  VERSION_VAR LDNS_VERSION_STRING)

if(LDNS_FOUND)
  set(LDNS_LIBRARIES ${LDNS_LIBRARY})
  set(LDNS_INCLUDE_DIRS ${LDNS_INCLUDE_DIR})
endif()
#find_program (CMAKE_LDNS_LDNSCONFIG_EXECUTABLE ldns-config)

#if(CMAKE_LDNS_LDNSCONFIG_EXECUTABLE)
  #set(LDNS_CONFIG_ARGS_LIBS "--libs")
  #set(LDNS_CONFIG_CFLAGS "--cflags")

  #set(LDNS_LIBRARIES "`${CMAKE_LDNS_LDNSCONFIG_EXECUTABLE} ${LDNS_CONFIG_ARGS_LIBS}`")
  #set(CMAKE_LDNS_C_FLAGS "`${CMAKE_LDNS_LDNSCONFIG_EXECUTABLE} ${LDNS_CONFIG_CFLAGS}`")

  #exec_program(${CMAKE_LDNS_LDNSCONFIG_EXECUTABLE}
    #ARGS ${LDNS_CONFIG_ARGS_LIBS}
    #OUTPUT_VARIABLE LDNS_CONFIG_LIBS )

  #string(REGEX MATCHALL "[-][L]([^ ;])+" LDNS_LINK_DIRECTORIES_WITH_PREFIX "${LDNS_CONFIG_LIBS}" )

  #if(LDNS_LINK_DIRECTORIES_WITH_PREFIX)
    #string(REGEX REPLACE "[-][L]" ";" LDNS_LINK_DIRECTORIES ${LDNS_LINK_DIRECTORIES_WITH_PREFIX} )
  #endif()

  #separate_arguments( LDNS_LINK_DIRECTORIES )

  #mark_as_advanced(
    #CMAKE_LDNS_C_FLAGS
    #LDNS_INCLUDE_DIRS
    #LDNS_LIBRARIES
    #CMAKE_LDNS_LDNSCONFIG_EXECUTABLE
    #)

#endif()

#if(LDNS_LIBRARIES)
  #if(CMAKE_LDNS_LDNSCONFIG_EXECUTABLE)
    #set(LDNS_FOUND 1)
  #endif()
#endif()
