#
# setup
#
set( MAP_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}  CACHE INTERNAL "" )


#
# map server
#
if( BUILD_SERVERS )
message( STATUS "Creating target map-server" )
set( MAP_HEADERS
  "${MAP_SOURCE_DIR}/achievement.h"
	"${MAP_SOURCE_DIR}/atcommand.h"
	"${MAP_SOURCE_DIR}/battle.h"
	"${MAP_SOURCE_DIR}/battleground.h"
	"${MAP_SOURCE_DIR}/buyingstore.h"
	"${MAP_SOURCE_DIR}/chat.h"
	"${MAP_SOURCE_DIR}/chrif.h"
	"${MAP_SOURCE_DIR}/clan.h"
	"${MAP_SOURCE_DIR}/clif.h"
	"${MAP_SOURCE_DIR}/date.h"
	"${MAP_SOURCE_DIR}/duel.h"
	"${MAP_SOURCE_DIR}/elemental.h"
	"${MAP_SOURCE_DIR}/guild.h"
	"${MAP_SOURCE_DIR}/homunculus.h"
	"${MAP_SOURCE_DIR}/instance.h"
	"${MAP_SOURCE_DIR}/intif.h"
	"${MAP_SOURCE_DIR}/itemdb.h"
	"${MAP_SOURCE_DIR}/log.h"
	"${MAP_SOURCE_DIR}/mail.h"
	"${MAP_SOURCE_DIR}/map.h"
	"${MAP_SOURCE_DIR}/mapreg.h"
	"${MAP_SOURCE_DIR}/mercenary.h"
	"${MAP_SOURCE_DIR}/mob.h"
	"${MAP_SOURCE_DIR}/npc.h"
	"${MAP_SOURCE_DIR}/party.h"
	"${MAP_SOURCE_DIR}/path.h"
	"${MAP_SOURCE_DIR}/pc.h"
	"${MAP_SOURCE_DIR}/pc_groups.h"
	"${MAP_SOURCE_DIR}/pet.h"
	"${MAP_SOURCE_DIR}/quest.h"
	"${MAP_SOURCE_DIR}/script.h"
	"${MAP_SOURCE_DIR}/script_constants.h"
	"${MAP_SOURCE_DIR}/searchstore.h"
	"${MAP_SOURCE_DIR}/skill.h"
	"${MAP_SOURCE_DIR}/status.h"
	"${MAP_SOURCE_DIR}/storage.h"
	"${MAP_SOURCE_DIR}/trade.h"
	"${MAP_SOURCE_DIR}/unit.h"
	"${MAP_SOURCE_DIR}/vending.h"
	"${MAP_SOURCE_DIR}/cashshop.h"
	"${MAP_SOURCE_DIR}/channel.h"
	)
set( MAP_SOURCES
  "${MAP_SOURCE_DIR}/achievement.cpp"
	"${MAP_SOURCE_DIR}/atcommand.cpp"
	"${MAP_SOURCE_DIR}/battle.cpp"
	"${MAP_SOURCE_DIR}/battleground.cpp"
	"${MAP_SOURCE_DIR}/buyingstore.cpp"
	"${MAP_SOURCE_DIR}/chat.cpp"
	"${MAP_SOURCE_DIR}/chrif.cpp"
	"${MAP_SOURCE_DIR}/clan.cpp"
	"${MAP_SOURCE_DIR}/clif.cpp"
	"${MAP_SOURCE_DIR}/date.cpp"
	"${MAP_SOURCE_DIR}/duel.cpp"
	"${MAP_SOURCE_DIR}/elemental.cpp"
	"${MAP_SOURCE_DIR}/guild.cpp"
	"${MAP_SOURCE_DIR}/homunculus.cpp"
	"${MAP_SOURCE_DIR}/instance.cpp"
	"${MAP_SOURCE_DIR}/intif.cpp"
	"${MAP_SOURCE_DIR}/itemdb.cpp"
	"${MAP_SOURCE_DIR}/log.cpp"
	"${MAP_SOURCE_DIR}/mail.cpp"
	"${MAP_SOURCE_DIR}/map.cpp"
	"${MAP_SOURCE_DIR}/mapreg.cpp"
	"${MAP_SOURCE_DIR}/mercenary.cpp"
	"${MAP_SOURCE_DIR}/mob.cpp"
	"${MAP_SOURCE_DIR}/npc.cpp"
	"${MAP_SOURCE_DIR}/npc_chat.cpp"
	"${MAP_SOURCE_DIR}/party.cpp"
	"${MAP_SOURCE_DIR}/path.cpp"
	"${MAP_SOURCE_DIR}/pc.cpp"
	"${MAP_SOURCE_DIR}/pc_groups.cpp"
	"${MAP_SOURCE_DIR}/pet.cpp"
	"${MAP_SOURCE_DIR}/quest.cpp"
	"${MAP_SOURCE_DIR}/script.cpp"
	"${MAP_SOURCE_DIR}/searchstore.cpp"
	"${MAP_SOURCE_DIR}/skill.cpp"
	"${MAP_SOURCE_DIR}/status.cpp"
	"${MAP_SOURCE_DIR}/storage.cpp"
	"${MAP_SOURCE_DIR}/trade.cpp"
	"${MAP_SOURCE_DIR}/unit.cpp"
	"${MAP_SOURCE_DIR}/vending.cpp"
	"${MAP_SOURCE_DIR}/cashshop.cpp"
	"${MAP_SOURCE_DIR}/channel.cpp"
  "${MAP_SOURCE_DIR}/channel.cpp"
	)
set( DEPENDENCIES common common_new yaml-cpp )
set( LIBRARIES ${GLOBAL_LIBRARIES} yaml-cpp )
set( INCLUDE_DIRS ${GLOBAL_INCLUDE_DIRS} ${COMMON_BASE_INCLUDE_DIRS} )
set( DEFINITIONS "${GLOBAL_DEFINITIONS} ${COMMON_BASE_DEFINITIONS}" )
if( WITH_PCRE )
	message( STATUS "Enabled PCRE code" )
	set( LIBRARIES ${LIBRARIES} ${PCRE_LIBRARIES} )
	set( INCLUDE_DIRS ${INCLUDE_DIRS} ${PCRE_INCLUDE_DIRS} )
	set( DEFINITIONS "${DEFINITIONS} -DPCRE_SUPPORT" )
else()
	message( STATUS "Disabled PCRE code" )
endif()
set( SOURCE_FILES ${COMMON_BASE_HEADERS} ${COMMON_HEADERS} ${MAP_HEADERS} ${MAP_SOURCES} )
source_group( common FILES ${COMMON_BASE_HEADERS} ${COMMON_HEADERS} )
source_group( map FILES ${MAP_HEADERS} ${MAP_SOURCES} )
include_directories( ${INCLUDE_DIRS} )

add_executable( map-server ${SOURCE_FILES} )
add_dependencies( map-server ${DEPENDENCIES} )
target_link_libraries( map-server ${LIBRARIES} ${DEPENDENCIES} )
set_target_properties( map-server PROPERTIES COMPILE_FLAGS "${DEFINITIONS}" )
if( INSTALL_COMPONENT_RUNTIME )
	cpack_add_component( Runtime_mapserver DESCRIPTION "map-server" DISPLAY_NAME "map-server" GROUP Runtime )
	install( TARGETS map-server
		DESTINATION "."
		COMPONENT Runtime_mapserver )
endif( INSTALL_COMPONENT_RUNTIME )
set( TARGET_LIST ${TARGET_LIST} map-server  CACHE INTERNAL "" )
message( STATUS "Creating target map-server - done" )
SYMLINK_FOLDER(map-server)
endif( BUILD_SERVERS )
