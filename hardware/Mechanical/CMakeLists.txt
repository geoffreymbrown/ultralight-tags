cmake_minimum_required(VERSION 3.15)
message("Configure Mechanical")

find_program(openscad openscad)
if (NOT DEFINED openscad)
    message("openscad not found")
    message("Cannot build plastic")
endif()

function (dxf_create scadfile)

    add_custom_command(OUTPUT "${scadfile}.dxf"
        COMMAND ${openscad} "${scadfile}.scad" -o "${scadfile}.dxf"
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/${scadfile}.scad"
        COMMENT "BUILDING ${scadfile}.dxf"
    )

    add_custom_target(${scadfile} DEPENDS "${scadfile}.dxf")
endfunction()

dxf_create(multicharger-acrylic)