CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(DataLisp)

#SETS
SET(VENDOR "DataLisp project 2013-2016")
SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 9)
SET(VERSION_HEX 0x0109)
SET(DL_Version ${VERSION_MAJOR}.${VERSION_MINOR})
if(NOT DEFINED DL_TARGET)
	SET(DL_TARGET "DataLisp")
ENDIF()

SET(DL_Root ${CMAKE_CURRENT_SOURCE_DIR}/src)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
SET(CMAKE_DEBUG_POSTFIX  "_d")

#CHECKS
MESSAGE(STATUS "Building DataLisp ${DL_Version}")
MESSAGE(STATUS "Root: ${DL_Root}")

IF(MINGW)
	MESSAGE(WARNING "Please don't use MinGW with DataLisp, use the MSVC version instead")
ENDIF()

IF(MSYS)
	MESSAGE(WARNING "Please don't use MSYS with DataLisp, use the MSVC version instead")
ENDIF()

IF(CYGWIN)
	MESSAGE(WARNING "Please don't use CYGWIN with DataLisp, use the MSVC version instead")
ENDIF()

#PACKAGES
option(DL_ONLY_LIBRARY "Compile only library" OFF)
option(DL_WITH_PYTHON "Compile additional python interface module" ON)
option(DL_BUILD_TESTS "Build tests." ON)
option(DL_BUILD_DOCUMENTATION "Build documentation with doxygen." ON)

IF(DL_WITH_PYTHON)
	find_package(Boost 1.45 COMPONENTS python)
	IF(Boost_FOUND)
		find_package(PythonLibs)
		find_package(PythonInterp)
		IF(PythonLibs_FOUND AND PythonInterp_FOUND)
  			include_directories(${PYTHON_INCLUDE_DIRS} ${Boost_INCLUDE_DIRS})
			SET(DL_HAS_PYTHON True)
		ENDIF()
	ENDIF()
ENDIF()

#DEFINITIONS AND FLAGS
include_directories(${DL_Root})
link_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(MSVC)
	#SET(DEFS "_UNICODE;UNICODE;_MT;_USRDLL")
	#SET(DEFS_DEBUG "_DEBUG")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oi /fp:fast /EHsc")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /Ot /GL")
	#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /MDd")
	#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
	IF(CMAKE_CL_64)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
	ENDIF()
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	IF (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
        message(STATUS "C++11 activated.")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    ELSEIF (GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
        message(WARNING "C++0x activated. If you get any errors update to a compiler which fully supports C++11")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
    ELSE ()
        message(WARNING "C++11 needed. Build can fail.")   
    ENDIF ()
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
	# We do not check version any further.
	if(VERBOSE)
		message(STATUS "C++11 activated.")
	endif()
  
  IF(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qstd=c++11")
  ELSE()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  ENDIF()
ELSE()
    message(WARNING "Unknown Compiler. C++11 needed. Build can fail.")   
ENDIF()

SET(DEFS "${DEFS};")
SET(DEFS_DEBUG "${DEFS_DEBUG};DL_DEBUG")

#CONFIGURE
configure_file(src/DataLispConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/DataLispConfig.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(DL_Src
  src/Data.cpp
  src/DataContainer.cpp
  src/DataGroup.cpp
  src/DataLisp.cpp
  src/Expressions.cpp
  src/Lexer.cpp
  src/Parser.cpp
  src/SourceLogger.cpp
  src/VM.cpp
  src/expressions/cast.cpp
  src/expressions/conditional.cpp
  src/expressions/entries.cpp
  src/expressions/helper.cpp
  src/expressions/io.cpp)

SET(DL_Hdr
  src/DataLispConfig.h.in
  src/Data.h
  src/DataContainer.h
  src/DataGroup.h
  src/DataLisp.h
  src/Expressions.h
  src/Lexer.h
  src/Parser.h
  src/SourceLogger.h
  src/SyntaxTree.h
  src/Token.h
  src/VM.h
  src/expressions/helper.h)

SET(DL_PY_Src
  src/python/module.cpp)

SET(DL_DUMP_Src
  src/dump/main.cpp)

SET(DL_TEST_EXPR_Src
  src/tests/expr_test.cpp)

SET(DL_TEST_UNICODE_Src
  src/tests/unicode_test.cpp)

SET(DL_Python
  python/datalisp/__init__.py
  python/datalisp/datalisp2json.py
  python/datalisp/datalisp2python.py
  python/datalisp/json2datalisp.py
  python/datalisp/python2datalisp.py)

#DEPENDIES, LIBARIES AND EXECUTABLES
add_library(datalisp SHARED ${DL_Src} ${DL_Hdr})
set_target_properties(datalisp PROPERTIES VERSION ${DL_Version})
target_compile_definitions(datalisp PRIVATE "${DEFS};DL_LIB_BUILD" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")

IF(DL_HAS_PYTHON)
	add_library(pydatalisp SHARED ${DL_PY_Src})
	target_link_libraries(pydatalisp datalisp ${Boost_LIBRARIES})
	set_target_properties(pydatalisp PROPERTIES VERSION ${DL_Version})
	set_target_properties(pydatalisp PROPERTIES PREFIX "" )
  set_target_properties(pydatalisp PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/datalisp)
	target_compile_definitions(pydatalisp PRIVATE "${DEFS}" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")

  file(COPY python/datalisp DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

	set(SETUP_PY ${CMAKE_CURRENT_BINARY_DIR}/setup.py)
  set(TIMESTAMP ${CMAKE_CURRENT_BINARY_DIR}/build/timestamp)
  configure_file(python/setup.py.in ${SETUP_PY})

  add_custom_command(OUTPUT ${TIMESTAMP}
                       COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} build
                       COMMAND ${CMAKE_COMMAND} -E touch ${TIMESTAMP}
                       DEPENDS ${DL_Python} pydatalisp)

  add_custom_target(pydatalisp_module ALL DEPENDS ${TIMESTAMP})

  install(CODE "execute_process(COMMAND ${PYTHON_EXECUTABLE} ${SETUP_PY} install --prefix ${CMAKE_INSTALL_PREFIX})")
ENDIF()

IF(NOT DL_ONLY_LIBRARY)
	add_executable(dl_dump ${DL_DUMP_Src})
	target_link_libraries(dl_dump datalisp)
	set_target_properties(dl_dump PROPERTIES VERSION ${DL_Version})
	target_compile_definitions(dl_dump PRIVATE "${DEFS}" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")

	IF(DL_BUILD_TESTS)
		enable_testing()

		FUNCTION(PUSH_TEST name files)
			add_executable(dl_test_${name} ${files})
			target_link_libraries(dl_test_${name} datalisp)
			set_target_properties(dl_test_${name} PROPERTIES VERSION ${DL_Version})
			target_compile_definitions(dl_test_${name} PRIVATE "${DEFS}" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")
			add_test(NAME ${name} COMMAND dl_test_${name})
		ENDFUNCTION(PUSH_TEST)

		PUSH_TEST(expr ${DL_TEST_EXPR_Src})
		PUSH_TEST(unicode ${DL_TEST_UNICODE_Src})
	ENDIF()
ENDIF(NOT DL_ONLY_LIBRARY)

# DOCUMENTATION
IF(DL_BUILD_DOCUMENTATION)
  find_package(Doxygen)
  IF(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tools/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen" VERBATIM
  )
  ENDIF()
ENDIF()

# INSTALL
install(TARGETS datalisp EXPORT datalisp
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib/static)

IF(NOT DL_ONLY_LIBRARY)
	install(TARGETS dl_dump
        RUNTIME DESTINATION bin)
ENDIF()

SET(DL_Hdr_INSTALL
  ${CMAKE_CURRENT_BINARY_DIR}/DataLispConfig.h
  src/Data.h
  src/DataContainer.h
  src/DataGroup.h
  src/DataLisp.h
  src/Expressions.h
  src/SourceLogger.h
  src/VM.h)

install(FILES ${DL_Hdr_INSTALL} DESTINATION include/DL)
install(EXPORT datalisp NAMESPACE dl_ DESTINATION lib/DL)

IF(DL_BUILD_DOCUMENTATION)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc DESTINATION share/DL)
ENDIF()