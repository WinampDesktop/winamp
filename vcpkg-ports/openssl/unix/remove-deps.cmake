	file(GLOB_RECURSE MAKEFILES ${DIR}/*/Makefile)
	foreach(MAKEFILE ${MAKEFILES})
message("removing deps from ${MAKEFILE}")
file(READ "${MAKEFILE}" _contents)
string(REGEX REPLACE "\n# DO NOT DELETE THIS LINE.*" "" _contents "${_contents}")
file(WRITE "${MAKEFILE}" "${_contents}")
	endforeach()
	