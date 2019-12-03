# Install script for directory: X:/Swift/Source/Engine/lib/zlib-1.2.8

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/zlib")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Debug")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/zlib/lib/zlibd.lib")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/zlib/lib" TYPE STATIC_LIBRARY OPTIONAL FILES "X:/Swift/Source/Engine/lib/zlib-1.2.8/build-debug/zlibd.lib")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/zlib/bin/zlibd.dll")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/zlib/bin" TYPE SHARED_LIBRARY FILES "X:/Swift/Source/Engine/lib/zlib-1.2.8/build-debug/zlibd.dll")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/zlib/lib/zlibstaticd.lib")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/zlib/lib" TYPE STATIC_LIBRARY FILES "X:/Swift/Source/Engine/lib/zlib-1.2.8/build-debug/zlibstaticd.lib")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/zlib/include/zconf.h;C:/Program Files (x86)/zlib/include/zlib.h")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/zlib/include" TYPE FILE FILES
    "X:/Swift/Source/Engine/lib/zlib-1.2.8/build-debug/zconf.h"
    "X:/Swift/Source/Engine/lib/zlib-1.2.8/zlib.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/zlib/share/man/man3/zlib.3")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/zlib/share/man/man3" TYPE FILE FILES "X:/Swift/Source/Engine/lib/zlib-1.2.8/zlib.3")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "C:/Program Files (x86)/zlib/share/pkgconfig/zlib.pc")
FILE(INSTALL DESTINATION "C:/Program Files (x86)/zlib/share/pkgconfig" TYPE FILE FILES "X:/Swift/Source/Engine/lib/zlib-1.2.8/build-debug/zlib.pc")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
ELSE(CMAKE_INSTALL_COMPONENT)
  SET(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
ENDIF(CMAKE_INSTALL_COMPONENT)

FILE(WRITE "X:/Swift/Source/Engine/lib/zlib-1.2.8/build-debug/${CMAKE_INSTALL_MANIFEST}" "")
FOREACH(file ${CMAKE_INSTALL_MANIFEST_FILES})
  FILE(APPEND "X:/Swift/Source/Engine/lib/zlib-1.2.8/build-debug/${CMAKE_INSTALL_MANIFEST}" "${file}\n")
ENDFOREACH(file)
