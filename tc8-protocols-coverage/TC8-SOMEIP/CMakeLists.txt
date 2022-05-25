cmake_minimum_required (VERSION 2.8)
project(Testing)
set (CMAKE_CXX_FLAGS "-g -std=c++11 -Wall -DBOOST_LOG_DYN_LINK ")
find_package (vsomeip 2.6.0 REQUIRED)

find_package( Boost 1.58 COMPONENTS system thread log_setup log REQUIRED )

set (PATH_TO_SOMEIP_IMPLEMNTATION "/home/focus/SOME-IP/build-common-api-cpp-native/vsomeip/implementation") # CHANGE IT BEFORE COMPILE AND BUILD
set (PATH_TO_INFRASTRUCTURE "/home/focus/SOME-IP/build-common-api-cpp-native/eth-core-infrastructure")# CHANGE IT BEFORE COMPILE AND BUILD



include_directories (
    ${Boost_INCLUDE_DIR}
    ${VSOMEIP_INCLUDE_DIRS}
    ./inc
    ./pub
    ${PATH_TO_INFRASTRUCTURE}/network-stack-abstraction/inc
    ${PATH_TO_INFRASTRUCTURE}/network-stack-abstraction/pub
    ${PATH_TO_INFRASTRUCTURE}/SOME-IP-abstraction/SOME-IP-API/inc
)



### global files ###
file(GLOB sd_sources
        "${PATH_TO_SOMEIP_IMPLEMNTATION}/service_discovery/src/*entry*.cpp"
        "${PATH_TO_SOMEIP_IMPLEMNTATION}/service_discovery/src/*option*.cpp"
        "${PATH_TO_SOMEIP_IMPLEMNTATION}/service_discovery/src/*message*.cpp"
	"${PATH_TO_INFRASTRUCTURE}/SOME-IP-abstraction/SOME-IP-API/src/SD_API.cpp"
    )
file(GLOB sd_headers
"${PATH_TO_SOMEIP_IMPLEMNTATION}/service_discovery/include/*"
"${PATH_TO_INFRASTRUCTURE}/SOME-IP-abstraction/SOME-IP-API/inc/udp.hpp"
)

file(GLOB udp_sources "${PATH_TO_INFRASTRUCTURE}/SOME-IP-abstraction/SOME-IP-API/src/udp.cpp")
file(GLOB udp_headers "${PATH_TO_INFRASTRUCTURE}/SOME-IP-abstraction/SOME-IP-API/inc/udp.hpp")
### Library sd ###
add_library(SD_API SHARED ${sd_sources})
set_target_properties(SD_API PROPERTIES PUBLIC_HEADER "${sd_headers}")
include(GNUInstallDirs)
install(TARGETS SD_API
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(SD_API.pc.in SD_API.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/SD_API.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
###############
### Library udp ###
add_library(udp_API SHARED ${udp_sources})
set_target_properties(udp_API PROPERTIES PUBLIC_HEADER "${udp_headers}")
include(GNUInstallDirs)
install(TARGETS udp_API
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

configure_file(udp_API.pc.in udp_API.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/udp_API.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)
###############




##################### Tests ############################
add_executable(SomeIpTesting ../test/SomeIpTesting.cpp)
target_link_libraries(SomeIpTesting vsomeip ${Boost_LIBRARIES})

		## FORMAT ##

add_executable(SOMEIPSRV_FORMAT ../src/SOMEIPSRV_FORMAT.cpp)
target_link_libraries(SOMEIPSRV_FORMAT vsomeip ${Boost_LIBRARIES} SD_API udp_API)

		## OPTIONS ##

add_executable(SOMEIPSRV_OPTIONS ../src/SOMEIPSRV_OPTIONS.cpp)
target_link_libraries(SOMEIPSRV_OPTIONS vsomeip ${Boost_LIBRARIES} SD_API udp_API)

		## SD MESSAGE ##

add_executable(SOMEIPSRV_SD_MESSAGE ../src/SOMEIPSRV_SD_MESSAGE.cpp)
target_link_libraries(SOMEIPSRV_SD_MESSAGE vsomeip ${Boost_LIBRARIES} SD_API udp_API)

		## RPC ##

add_executable(SOMEIPSRV_RPC ../src/SOMEIPSRV_RPC.cpp)
target_link_libraries(SOMEIPSRV_RPC vsomeip ${Boost_LIBRARIES} SD_API udp_API)

		## ONWIRE ##
		
add_executable(SOMEIPSRV_ONWIRE ../src/SOMEIPSRV_ONWIRE.cpp)
target_link_libraries(SOMEIPSRV_ONWIRE vsomeip ${Boost_LIBRARIES} SD_API udp_API )

		## BASIC ##

add_executable(SOMEIPSRV_BASIC ../src/SOMEIPSRV_BASIC.cpp )
target_link_libraries(SOMEIPSRV_BASIC vsomeip ${Boost_LIBRARIES} SD_API udp_API )

		## SD BEHAVIOR ##

add_executable(SOMEIPSRV_SD_BEHAVIOR ../src/SOMEIPSRV_SD_BEHAVIOR.cpp )
target_link_libraries(SOMEIPSRV_SD_BEHAVIOR vsomeip ${Boost_LIBRARIES} SD_API udp_API )

