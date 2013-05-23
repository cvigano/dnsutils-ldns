cmake_minimum_required (VERSION 2.8)

project (dnsutils-ldns)

set (DNSUTILS_LDNS_MAJOR 0)

set (DNSUTILS_LDNS_MINOR 1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

add_executable(host src/host.c)

find_package(LDNS REQUIRED)

if(LDNS_FOUND)
  include_directories(${LDNS_INCLUDE_DIRS})
  target_link_libraries(host ${LDNS_LIBRARIES})
endif(LDNS_FOUND)
