cmake_minimum_required(VERSION 3.15)

find_program(kibot_EXECUTABLE kibot 
             HINTS /Applications/KiCad/KiCad.app/Contents/Frameworks/Python.framework/Versions/3.8/bin/
             NO_CACHE)

function (add_pcb_outputs name)

  # see if there's an extra argument (builddir)

  set(ExtraArgs ${ARGN})

  # Get the length of the args list

  list(LENGTH ExtraArgs NumExtraArgs)

  # Set the build directory

  if(NumExtraArgs GREATER 0)
    list(GET ExtraArgs 0 builddir)
  else()
    set(builddir ${CMAKE_CURRENT_BINARY_DIR})
  endif()
    set(stampfile ${builddir}/pcb.stamp)

  add_custom_command(OUTPUT ${stampfile}
             COMMAND ${kibot_EXECUTABLE} -d ${builddir}
             COMMAND ${CMAKE_COMMAND} -E touch ${stampfile}
             WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
             DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${name}.kibot.yaml" "${CMAKE_CURRENT_SOURCE_DIR}/${name}.kicad_sch"  "${CMAKE_CURRENT_SOURCE_DIR}/${name}.kicad_pcb"
             COMMENT "Building ${name}")

    add_custom_target(${name}-pcb DEPENDS ${stampfile} )

endfunction()

if (DEFINED kibot_EXECUTABLE)
    message("Configure PCBs")
    # add_subdirectory(AccelTag)
    add_subdirectory(BitTagv5)
    add_subdirectory(BitTagv7)
    add_subdirectory(MultiCharger)
    add_subdirectory(PresTagv5)
    add_subdirectory(steval-daughter-v2)
    add_subdirectory(tag-breakout-v2)
    add_subdirectory(tagbase-jlcpcb-v3)
else()
    message("Cannot configure pcb boards")
    message("kibot exec was not found")
endif()

add_subdirectory(Mechanical)


