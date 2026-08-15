# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\appQtMirrorApp_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\appQtMirrorApp_autogen.dir\\ParseCache.txt"
  "appQtMirrorApp_autogen"
  )
endif()
