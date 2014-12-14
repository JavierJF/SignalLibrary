# add a target to generate API documentation with Doxygen
IF(DOXYGEN_FOUND)
   CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/cmake/Doxyfile.in
		  ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
   ADD_CUSTOM_TARGET(doc
   ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
   WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
   COMMENT "Generating API documentation with Doxygen" VERBATIM)
ENDIF(DOXYGEN_FOUND)
