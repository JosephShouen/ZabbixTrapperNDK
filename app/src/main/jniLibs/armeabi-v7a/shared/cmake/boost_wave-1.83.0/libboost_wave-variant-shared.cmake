# Generated by Boost 1.83.0

# address-model=32

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  _BOOST_SKIPPED("libboost_wave.so" "32 bit, need 64")
  return()
endif()

# layout=system

# toolset=clang

# link=shared

if(DEFINED Boost_USE_STATIC_LIBS)
  if(Boost_USE_STATIC_LIBS)
    _BOOST_SKIPPED("libboost_wave.so" "shared, Boost_USE_STATIC_LIBS=${Boost_USE_STATIC_LIBS}")
    return()
  endif()
else()
  if(WIN32 AND NOT _BOOST_SINGLE_VARIANT)
    _BOOST_SKIPPED("libboost_wave.so" "shared, default on Windows is static, set Boost_USE_STATIC_LIBS=OFF to override")
    return()
  endif()
endif()

# runtime-link=shared

if(Boost_USE_STATIC_RUNTIME)
  _BOOST_SKIPPED("libboost_wave.so" "shared runtime, Boost_USE_STATIC_RUNTIME=${Boost_USE_STATIC_RUNTIME}")
  return()
endif()

# runtime-debugging=off

if(Boost_USE_DEBUG_RUNTIME)
  _BOOST_SKIPPED("libboost_wave.so" "release runtime, Boost_USE_DEBUG_RUNTIME=${Boost_USE_DEBUG_RUNTIME}")
  return()
endif()

# threading=multi

# variant=release

if(NOT "${Boost_USE_RELEASE_LIBS}" STREQUAL "" AND NOT Boost_USE_RELEASE_LIBS)
  _BOOST_SKIPPED("libboost_wave.so" "release, Boost_USE_RELEASE_LIBS=${Boost_USE_RELEASE_LIBS}")
  return()
endif()

if(Boost_VERBOSE OR Boost_DEBUG)
  message(STATUS "  [x] libboost_wave.so")
endif()

# Create imported target Boost::wave

if(NOT TARGET Boost::wave)
  add_library(Boost::wave SHARED IMPORTED)

  set_target_properties(Boost::wave PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "${_BOOST_INCLUDEDIR}"
    INTERFACE_COMPILE_DEFINITIONS "BOOST_WAVE_NO_LIB"
  )
endif()

# Target file name: libboost_wave.so

get_target_property(__boost_imploc Boost::wave IMPORTED_LOCATION_RELEASE)
if(__boost_imploc)
  message(SEND_ERROR "Target Boost::wave already has an imported location '${__boost_imploc}', which is being overwritten with '${_BOOST_LIBDIR}/libboost_wave.so'")
endif()
unset(__boost_imploc)

set_property(TARGET Boost::wave APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)

set_target_properties(Boost::wave PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE CXX
  IMPORTED_LOCATION_RELEASE "${_BOOST_LIBDIR}/libboost_wave.so"
  )

set_target_properties(Boost::wave PROPERTIES
  MAP_IMPORTED_CONFIG_MINSIZEREL Release
  MAP_IMPORTED_CONFIG_RELWITHDEBINFO Release
  )

set_property(TARGET Boost::wave APPEND
  PROPERTY INTERFACE_COMPILE_DEFINITIONS "BOOST_WAVE_DYN_LINK"
  )

list(APPEND _BOOST_WAVE_DEPS atomic chrono filesystem thread headers)
