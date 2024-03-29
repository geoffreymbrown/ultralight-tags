cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

# Tools for building embedded code

find_program(GCCARM arm-none-eabi-gcc REQUIRED)
if(MSVC)
  find_program(FMPP fmpp fmpp.bat PATHS "c:/Program Files/fmpp/bin" REQUIRED)
else()
  find_program(FMPP fmpp REQUIRED)
endif()

if((NOT DEFINED GCCARM) OR (NOT DEFINED FMPP))
  message("Couldn't find GCCARM and FMPP")
  message("Cannot configure embedded")
  return()
endif()

IF(DEFINED ENV{CHIBIOS_DIR})
    MESSAGE("CHIBIOS_DIR IS $ENV{CHIBIOS_DIR}")
    set(CHIBIOS_DIR $ENV{CHIBIOS_DIR})
ELSE()
    set(CHIBIOS_DIR "${CMAKE_SOURCE_DIR}/ChibiOS")
ENDIF()
MESSAGE("CHIBIOS_DIR is ${CMAKE_SOURCE_DIR}/ChibiOS")



# Tools for programming hardware

if(MSVC)
  find_program(
    STM32_PROGRAMMER
    NAMES STM32_Programmer_CLI STM32_Programmer_CLI.exe
    PATHS
      "C:/Program Files/STMicroelectronics/STM32Cube/STM32CubeProgrammer/bin")
else()
  find_program(
    STM32_PROGRAMMER
    NAMES STM32_Programmer_CLI
    HINTS
      "/Users/geobrown/Software/stm32cubeprogrammer/STM32CubeProgrammer.app/Contents/MacOs/bin"
    PATHS "/home/geobrown/Software/STM32CubeProgrammer/bin"
  )
endif()

if(NOT DEFINED STM32_PROGRAMMER)
  message(
    "To program embedded boards install https://www.st.com/en/development-tools/stm32cubeprog.html"
  )
endif()

#
# Function to build embedded ChibiOS targets using make
#

function(add_embedded_target name proto_target)

  # get some paths

  message("proto target ${proto_target}")

  set(tag_source_dir
      ${CMAKE_CURRENT_LIST_DIR}/..
      CACHE INTERNAL "")

  get_property(
    monitor_includes
    TARGET tag_monitor_interface
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES)

  # create variables for gnu make

  set(boarddir ${CMAKE_CURRENT_BINARY_DIR}/../../boards)
  set(builddir ${CMAKE_CURRENT_BINARY_DIR}/build)
  set(depdir ${CMAKE_CURRENT_BINARY_DIR}/dep)

  # these just affect tags with nanopb

  if(NOT ${proto_target} EQUAL 0)
    get_target_property(protopaths ${proto_target}
                        INTERFACE_INCLUDE_DIRECTORIES)
  endif()
  # get_target_property(configdir default_config BINARY_DIR)

  # patch the source directory to get relative path to current source this is
  # used inside the tags for information about the relative path to the tag
  # source within the git repo.

  string(REPLACE ${CMAKE_SOURCE_DIR} "" src_path ${CMAKE_CURRENT_SOURCE_DIR})

  # the following only in recent versions of cmake cmake_path(RELATIVE_PATH
  # src_path ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})

  message("Current Source Dir ${src_path}")

  # create options for make

  set(default_opt
      BUILDDIR=${builddir}
      DEPDIR=${depdir}
      NANOPBDIR="${protopaths}"
      BOARDDIR=${boarddir}
      CHIBIOS=${CHIBIOS_DIR}
      SOURCEDIR=${src_path}
      PROJECT=${name}
      MONITORINCDIR=\"${monitor_includes}\")

  # create build target -- depends upon binary file

  set(target_elf ${CMAKE_CURRENT_BINARY_DIR}/build/${name}.elf)

  #add_custom_target(${name} ALL DEPENDS ${target_elf})

  add_custom_target(${name} ALL
    BYPRODUCTS ${target_elf}
    #COMMAND echo " ${default_opt} ${ARGN}"
    COMMAND make -C ${name} ${default_opt} ${ARGN}
    WORKING_DIRECTORY ${tag_source_dir}
    COMMENT "Building ${name}")

  # build version.h file which includes git variables

  add_custom_target(${name}_version_h ${CMAKE_COMMAND} -P
                                      ${CMAKE_SOURCE_DIR}/cmake/version.cmake
                    BYPRODUCTS version.h)

  add_dependencies(${name} ${name}_version_h)

  set_property(TARGET ${name} PROPERTY ADDITIONAL_CLEAN_FILES
                                       ${depdir};${builddir})

  #
  # add proto files as dependency for tags
  #

  if(NOT ${proto_target} EQUAL 0)
    add_dependencies(${name} ${proto_target}_sources)
  endif()

  # create download target

  add_custom_target(
    ${name}-download
    DEPENDS ${name}
    COMMAND ${STM32_PROGRAMMER} -c port=SWD mode=UR -d ${target_elf} -g
            0x08000000)

  add_custom_target(${name}-erase COMMAND ${STM32_PROGRAMMER} -c port=SWD
                                          mode=UR -e all)

  add_custom_target(
    ${name}-dfu
    DEPENDS ${name} ${STM32_PROGRAMMER}
    COMMAND ${STM32_PROGRAMMER} -c port=usb1 -d ${target_elf})

  install(FILES ${target_elf} TYPE BIN OPTIONAL)

endfunction()

# Make the programmer bases and tags

add_subdirectory(boards)
add_subdirectory(bases EXCLUDE_FROM_ALL)
add_subdirectory(proto-c)
add_subdirectory(tags EXCLUDE_FROM_ALL)
