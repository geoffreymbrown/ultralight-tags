cmake_minimum_required(VERSION 3.10)
project(image-testfile)

# find the files

file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/tikz/*.tex)

# create output directory

set(image-out-directory "${CMAKE_CURRENT_SOURCE_DIR}/static/images-svg")
file(MAKE_DIRECTORY ${image-out-directory})

add_custom_target(images-svg)

foreach(f ${SRC_FILES})
  # process names

  get_filename_component(fname ${f} NAME_WLE)
  set(out-files "${fname}.pdf" "${fname}.svg")

  # generate targets

  add_custom_command(
    OUTPUT "${fname}.pdf"
    COMMAND pdflatex -output-directory="${CMAKE_CURRENT_BINARY_DIR}"
            -interaction=batchmode "${f}"
    #COMMAND cmake -E copy_if_different "${fname}.pdf" "${image-out-directory}"
    BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/${fname}.log"
               "${CMAKE_CURRENT_BINARY_DIR}/${fname}.aux"
    DEPENDS ${f})

  add_custom_command(
    OUTPUT "${fname}.svg"
    COMMAND pdf2svg "${fname}.pdf" "${fname}.svg"
    COMMAND cmake -E copy_if_different "${fname}.svg" "${image-out-directory}"
    DEPENDS "${fname}.pdf")

  add_custom_target("${fname}-tikz" DEPENDS ${fname}.svg)
  add_dependencies(images-svg "${fname}-tikz")
endforeach()

add_custom_target(docs 
COMMAND hugo -d "${CMAKE_CURRENT_BINARY_DIR}/public/" -s "${CMAKE_CURRENT_SOURCE_DIR}" -b "https://pages.github.iu.edu/geobrown/NanoTag-paper/"
)

add_custom_command(TARGET docs POST_BUILD COMMAND rsync -rcv --delete --exclude=.git "${CMAKE_CURRENT_BINARY_DIR}/public/" "${CMAKE_BINARY_DIR}/../NanoTag-paper-gh-pages")



