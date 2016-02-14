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
configure_file(${DL_Root}/dl_Config.h.in ${CMAKE_CURRENT_BINARY_DIR}/dl_Config.h)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(DL_Src
  ${DL_Root}/Data.cpp
  ${DL_Root}/DataArray.cpp
  ${DL_Root}/DataContainer.cpp
  ${DL_Root}/DataGroup.cpp
  ${DL_Root}/DataLisp.cpp
  ${DL_Root}/Expressions.cpp
  ${DL_Root}/Lexer.cpp
  ${DL_Root}/Parser.cpp
  ${DL_Root}/SourceLogger.cpp)

SET(DL_Hdr
  ${DL_Root}/dl_Config.h.in
  ${DL_Root}/Data.h
  ${DL_Root}/DataArray.h
  ${DL_Root}/DataContainer.h
  ${DL_Root}/DataGroup.h
  ${DL_Root}/DataLisp.h
  ${DL_Root}/Expressions.h
  ${DL_Root}/Lexer.h
  ${DL_Root}/Parser.h
  ${DL_Root}/SourceLogger.h
  ${DL_Root}/SyntaxTree.h
  ${DL_Root}/Token.h)

SET(DL_DUMP_Src
  ${DL_Root}/dump/main.cpp)

SET(DL_TEST_EXPR_Src
  ${DL_Root}/tests/expr_test.cpp)

#DEPENDIES, LIBARIES AND EXECUTABLES
add_library(${DL_TARGET} SHARED ${DL_Src} ${DL_Hdr})
set_target_properties(${DL_TARGET} PROPERTIES VERSION ${DL_Version})
set_target_properties(${DL_TARGET} PROPERTIES COMPILE_DEFINITIONS "${DEFS};DL_LIB_BUILD" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")

add_executable(dl_dump ${DL_DUMP_Src})
target_link_libraries(dl_dump ${DL_TARGET})

add_executable(dl_test_expr ${DL_TEST_EXPR_Src})
target_link_libraries(dl_test_expr ${DL_TARGET})