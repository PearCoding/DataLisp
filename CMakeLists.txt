CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(DataLisp)

#SETS
SET(VENDOR "DataLisp project 2013-2015")
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

IF(DL_WITH_PYTHON)
	find_package(Boost 1.45 COMPONENTS python)
	IF(Boost_FOUND)
		find_package(PythonLibs)
		IF(PythonLibs_FOUND)
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
ELSE()
    message(WARNING "Unknown Compiler. C++11 needed. Build can fail.")   
ENDIF()

SET(DEFS "${DEFS};")
SET(DEFS_DEBUG "${DEFS_DEBUG};DL_DEBUG")

#CONFIGURE
configure_file(src/dl_Config.h.in ${CMAKE_CURRENT_BINARY_DIR}/dl_Config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(DL_Src
  src/Data.cpp
  src/DataArray.cpp
  src/DataContainer.cpp
  src/DataGroup.cpp
  src/DataLisp.cpp
  src/Expressions.cpp
  src/Lexer.cpp
  src/Parser.cpp
  src/SourceLogger.cpp)

SET(DL_Hdr
  src/dl_Config.h.in
  src/Data.h
  src/DataArray.h
  src/DataContainer.h
  src/DataGroup.h
  src/DataLisp.h
  src/Expressions.h
  src/Lexer.h
  src/Parser.h
  src/SourceLogger.h
  src/SyntaxTree.h
  src/Token.h)

SET(DL_PY_Src
  src/python/module.cpp)

SET(DL_PY_Hdr
  src/dl_Config.h)

SET(DL_DUMP_Src
  src/dump/main.cpp)

SET(DL_TEST_EXPR_Src
  src/tests/expr_test.cpp)

SET(DL_TEST_UNICODE_Src
  src/tests/unicode_test.cpp)

#DEPENDIES, LIBARIES AND EXECUTABLES
add_library(${DL_TARGET} SHARED ${DL_Src} ${DL_Hdr})
set_target_properties(${DL_TARGET} PROPERTIES VERSION ${DL_Version})
target_compile_definitions(${DL_TARGET} PRIVATE "${DEFS};DL_LIB_BUILD" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")

IF(DL_HAS_PYTHON)
	add_library(pydatalisp SHARED ${DL_PY_Src} ${DL_PY_Hdr})
	target_link_libraries(pydatalisp ${DL_TARGET} ${Boost_LIBRARIES})
	set_target_properties(pydatalisp PROPERTIES VERSION ${DL_Version})
	set_target_properties(pydatalisp PROPERTIES PREFIX "" )
	target_compile_definitions(pydatalisp PRIVATE "${DEFS}" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")
ENDIF()

IF(NOT DL_ONLY_LIBRARY)
	add_executable(dl_dump ${DL_DUMP_Src})
	target_link_libraries(dl_dump ${DL_TARGET})
	set_target_properties(dl_dump PROPERTIES VERSION ${DL_Version})
	target_compile_definitions(dl_dump PRIVATE "${DEFS}" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")

	IF(DL_BUILD_TESTS)
		enable_testing()

		FUNCTION(PUSH_TEST name files)
			add_executable(dl_test_${name} ${files})
			target_link_libraries(dl_test_${name} ${DL_TARGET})
			set_target_properties(dl_test_${name} PROPERTIES VERSION ${DL_Version})
			target_compile_definitions(dl_test_${name} PRIVATE "${DEFS}" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")
			add_test(NAME ${name} COMMAND dl_test_${name})
		ENDFUNCTION(PUSH_TEST)

		PUSH_TEST(expr ${DL_TEST_EXPR_Src})
		PUSH_TEST(unicode ${DL_TEST_UNICODE_Src})
	ENDIF()
ENDIF(NOT DL_ONLY_LIBRARY)
