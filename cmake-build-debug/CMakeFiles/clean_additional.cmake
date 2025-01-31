# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\voicebet_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\voicebet_autogen.dir\\ParseCache.txt"
  "voicebet_autogen"
  )
endif()
