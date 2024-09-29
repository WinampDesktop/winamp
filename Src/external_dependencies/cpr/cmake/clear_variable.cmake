	macro(clear_variable)
cmake_parse_arguments(CLEAR_VAR "" "DESTINATION;BACKUP;REPLACE" "" ${ARGN})
set(${CLEAR_VAR_BACKUP} ${${CLEAR_VAR_DESTINATION}})
set(${CLEAR_VAR_DESTINATION} ${CLEAR_VAR_REPLACE})
	endmacro()
	
	macro(restore_variable)
cmake_parse_arguments(CLEAR_VAR "" "DESTINATION;BACKUP" "" ${ARGN})
set(${CLEAR_VAR_DESTINATION} ${${CLEAR_VAR_BACKUP}})
unset(${CLEAR_VAR_BACKUP})
	endmacro()
	