cmake_minimum_required(VERSION 3.15)

project(NanoTag VERSION 0.1.0)
#set(VCPKG_VERBOSE true)

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/")
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake" ${CMAKE_MODULE_PATH})

set(CMAKE_FIND_ROOT 
    "/home/geobrown/Software;/Users/geobrown/Software;c:/Program Files/fmpp/bin;C:/Program Files/STMicroelectronics" )

#set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")

add_subdirectory(software)
if (NOT WIN32)
add_subdirectory(hardware)
add_subdirectory(docs)
endif()

set(CPACK_PACKAGE_NAME "UltralightTags")
set(CPACK_PACKAGE_VENDOR "cs.iu.edu")
set(CPACK_PACKAGE_VERSION "2.0.0")
set(CPACK_PACKAGE_VERSION_MAJOR 2)
set(CPACK_PACKAGE_VERSION_MINOR 1)
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Ultralight Tags Host Tools")

if(FALSE)
set(CPACK_BUNDLE_APPLE_CERT_APP "Developer ID Application: Indiana University (5J69S77A7G)")
set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Apple Development")
set(CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "5J69S77A7G")
endif()



if(WIN32)
  set(CPACK_GENERATOR ${CPACK_BINARY_WIX})
endif()

if(APPLE)
  set(CPACK_GENERATOR ${CPACK_BINARY_DRAGNDROP})
endif()

include(CPack)



