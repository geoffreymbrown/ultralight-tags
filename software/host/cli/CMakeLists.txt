cmake_minimum_required(VERSION 3.15)

set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

function(host_cli_tool name)

  add_executable(${name} ${ARGN})
  if(WIN32)
    target_link_libraries(${name} tag)
  else()
    target_link_libraries(${name} tag pthread)
  endif()

  set_target_properties(
    ${name}
    PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
               RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin)
  if(WIN32)
    set_target_properties(${name} PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
  endif()

  # bundle things up properly

  if(APPLE)
    #include(DeployQt)
    #macdeployqt(${name})
  endif()
endfunction()

# host_cli_tool(tag-cli MACOSX_BUNDLE WIN32 cli.cc options.cc commands.cc)
host_cli_tool(tag-dwnld MACOSX_BUNDLE WIN32 dwnld.cc options.cc)
host_cli_tool(tag-test MACOSX_BUNDLE WIN32 test.cc options.cc)
host_cli_tool(tag-test-example MACOSX_BUNDLE WIN32 test-example.cc options.cc)
host_cli_tool(tag-info MACOSX_BUNDLE WIN32 tag-info.cc options.cc)
host_cli_tool(tag-reset MACOSX_BUNDLE WIN32 tag-reset.cc options.cc)
host_cli_tool(tag-start MACOSX_BUNDLE WIN32 tag-start.cc options.cc)
