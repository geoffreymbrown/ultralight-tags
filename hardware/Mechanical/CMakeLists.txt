cmake_minimum_required(VERSION 3.15)
message("Configure Mechanical")

find_program(openscad openscad)
if (NOT DEFINED openscad)
    message("openscad not found")
    message("Cannot build plastic")
endif()

function (dxf_create scadfile)

    add_custom_command(OUTPUT "${scadfile}.dxf"
        COMMAND ${openscad} "${scadfile}.scad" -o "${CMAKE_CURRENT_BINARY_DIR}/${scadfile}.dxf"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${scadfile}.scad"
        COMMENT "BUILDING ${scadfile}.dxf"
    )

    add_custom_target(${scadfile} DEPENDS "${scadfile}.dxf")
endfunction()

function (stl_create scadfile)

    add_custom_command(OUTPUT "${scadfile}_base.stl" "${scadfile}_lid.stl"
        COMMAND ${openscad} "${scadfile}.scad" -D makelid=false -o "${CMAKE_CURRENT_BINARY_DIR}/${scadfile}_base.stl"
        COMMAND ${openscad} "${scadfile}.scad" -D makebase=false -o "${CMAKE_CURRENT_BINARY_DIR}/${scadfile}_lid.stl"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${scadfile}.scad"
        COMMENT "BUILDING ${scadfile}.stl"
    )

    add_custom_target(${scadfile} DEPENDS "${scadfile}_lid.stl" "${scadfile}_base.stl")
endfunction()

dxf_create(multicharger-acrylic)
stl_create(BitTagvc)