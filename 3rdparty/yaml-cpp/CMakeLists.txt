cmake_minimum_required(VERSION 2.8)

## start setting
SET (this_target yaml-cpp)
PROJECT(${this_target})

find_path( YAML_INCLUDE_DIRS "yaml.h"
	PATHS "${CMAKE_CURRENT_SOURCE_DIR}/include/yaml-cpp"
	NO_DEFAULT_PATH )
find_path( YAML_SOURCE_DIR "regex_yaml.cpp"
	PATHS "${CMAKE_CURRENT_SOURCE_DIR}/src"
	NO_DEFAULT_PATH )
mark_as_advanced( YAML_INCLUDE_DIRS )
mark_as_advanced( YAML_SOURCE_DIR )

set( YAML_HEADERS
    "${YAML_INCLUDE_DIRS}/yaml.h"
    "${YAML_INCLUDE_DIRS}/traits.h"
    "${YAML_INCLUDE_DIRS}/stlemitter.h"
    "${YAML_INCLUDE_DIRS}/parser.h"
    "${YAML_INCLUDE_DIRS}/ostream_wrapper.h"
    "${YAML_INCLUDE_DIRS}/null.h"
    "${YAML_INCLUDE_DIRS}/noncopyable.h"
    "${YAML_INCLUDE_DIRS}/mark.h"
    "${YAML_INCLUDE_DIRS}/exceptions.h"
    "${YAML_INCLUDE_DIRS}/eventhandler.h"
    "${YAML_INCLUDE_DIRS}/emitterstyle.h"
    "${YAML_INCLUDE_DIRS}/emittermanip.h"
    "${YAML_INCLUDE_DIRS}/emitter.h"
    "${YAML_INCLUDE_DIRS}/emitterdef.h"
    "${YAML_INCLUDE_DIRS}/emitfromevents.h"
    "${YAML_INCLUDE_DIRS}/dll.h"
    "${YAML_INCLUDE_DIRS}/binary.h"
    "${YAML_INCLUDE_DIRS}/anchor.h"
    "${YAML_INCLUDE_DIRS}/node/type.h"
    "${YAML_INCLUDE_DIRS}/node/ptr.h"
    "${YAML_INCLUDE_DIRS}/node/parse.h"
    "${YAML_INCLUDE_DIRS}/node/node.h"
    "${YAML_INCLUDE_DIRS}/node/iterator.h"
    "${YAML_INCLUDE_DIRS}/node/impl.h"
    "${YAML_INCLUDE_DIRS}/node/emit.h"
    "${YAML_INCLUDE_DIRS}/node/convert.h"
    "${YAML_INCLUDE_DIRS}/contrib/graphbuilder.h"
    "${YAML_INCLUDE_DIRS}/contrib/anchordict.h"
    CACHE INTERNAL "yaml headers" )
set( YAML_SOURCES
	"${YAML_SOURCE_DIR}/binary.cpp"
  "${YAML_SOURCE_DIR}/convert.cpp"
  "${YAML_SOURCE_DIR}/directives.cpp"
  "${YAML_SOURCE_DIR}/emit.cpp"
  "${YAML_SOURCE_DIR}/emitfromevents.cpp"
  "${YAML_SOURCE_DIR}/emitter.cpp"
  "${YAML_SOURCE_DIR}/emitterstate.cpp"
  "${YAML_SOURCE_DIR}/emitterutils.cpp"
  "${YAML_SOURCE_DIR}/exceptions.cpp"
  "${YAML_SOURCE_DIR}/exp.cpp"
  "${YAML_SOURCE_DIR}/memory.cpp"
  "${YAML_SOURCE_DIR}/nodebuilder.cpp"
  "${YAML_SOURCE_DIR}/node.cpp"
  "${YAML_SOURCE_DIR}/node_data.cpp"
  "${YAML_SOURCE_DIR}/nodeevents.cpp"
  "${YAML_SOURCE_DIR}/null.cpp"
  "${YAML_SOURCE_DIR}/ostream_wrapper.cpp"
  "${YAML_SOURCE_DIR}/parse.cpp"
  "${YAML_SOURCE_DIR}/parser.cpp"
  "${YAML_SOURCE_DIR}/regex_yaml.cpp"
  "${YAML_SOURCE_DIR}/scanner.cpp"
  "${YAML_SOURCE_DIR}/scanscalar.cpp"
  "${YAML_SOURCE_DIR}/scantag.cpp"
  "${YAML_SOURCE_DIR}/scantoken.cpp"
  "${YAML_SOURCE_DIR}/simplekey.cpp"
  "${YAML_SOURCE_DIR}/singledocparser.cpp"
  "${YAML_SOURCE_DIR}/stream.cpp"
  "${YAML_SOURCE_DIR}/tag.cpp"
  "${YAML_SOURCE_DIR}/contrib/graphbuilderadapter.cpp"
  "${YAML_SOURCE_DIR}/contrib/graphbuilder.cpp"
	CACHE INTERNAL "yaml sources" )
set( YAML_DEFINITIONS
	"-std=c++11"
	CACHE INTERNAL "yaml definitions" )
ADD_LIBRARY(${this_target} STATIC ${YAML_SOURCES} )
