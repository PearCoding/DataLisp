cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(DataLisp
	VERSION 2.2
	DESCRIPTION "Lisp based configuration and settings language")

#SETS
SET(VENDOR "DataLisp project 2013-2020")

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
SET(CMAKE_DEBUG_POSTFIX  "_d")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#CHECKS
MESSAGE(STATUS "Building DataLisp ${DataLisp_VERSION}")

IF(MINGW)
	MESSAGE(WARNING "Please don't use MinGW with DataLisp, use the MSVC version instead")
ENDIF()

IF(MSYS)
	MESSAGE(WARNING "Please don't use MSYS with DataLisp, use the MSVC version instead")
ENDIF()

IF(CYGWIN)
	MESSAGE(WARNING "Please don't use CYGWIN with DataLisp, use the MSVC version instead")
ENDIF()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#PACKAGES
option(DL_WITH_TOOLS          "Compile additional tools" ON)
option(DL_WITH_PYTHON         "Compile additional python interface module" ON)
option(DL_BUILD_TESTS         "Build tests." ON)
option(DL_BUILD_DOCUMENTATION "Build documentation with doxygen." ON)
option(BUILD_SHARED_LIBS      "Build shared library" ON)

IF(DL_WITH_PYTHON)
	find_package(Boost 1.45 COMPONENTS python)
	IF(Boost_FOUND)
    find_package(PythonLibs)
		IF(PYTHONLIBS_FOUND)
			SET(DL_HAS_PYTHON True)
		ENDIF()
	ENDIF()
ENDIF()

#DEFINITIONS AND FLAGS
#CONFIGURE
configure_file(src/DataLispConfig.h.in ${CMAKE_CURRENT_BINARY_DIR}/DataLispConfig.h)
include_directories(src/ ${CMAKE_CURRENT_BINARY_DIR})

SET(DL_Src
  src/Data.cpp
  src/DataContainer.cpp
  src/DataGroup.cpp
  src/DataLisp.cpp
  src/SourceLogger.cpp
  src/VM.cpp
  src/internal/Expressions.cpp
  src/internal/Lexer.cpp
  src/internal/Parser.cpp
  src/internal/expressions/cast.cpp
  src/internal/expressions/conditional.cpp
  src/internal/expressions/entries.cpp
  src/internal/expressions/io.cpp)

SET(DL_Hdr
  src/DataLispConfig.h.in
  src/Data.h
  src/Data.inl
  src/DataContainer.h
  src/DataGroup.h
  src/DataLisp.h
  src/DataType.h
  src/SourceLogger.h
  src/VM.h
  src/internal/Expressions.h
  src/internal/Lexer.h
  src/internal/Parser.h
  src/internal/SyntaxTree.h
  src/internal/Token.h)

SET(DL_Python
  python/datalisp/__init__.py
  python/datalisp/datalisp2json.py
  python/datalisp/datalisp2python.py
  python/datalisp/json2datalisp.py
  python/datalisp/python2datalisp.py)

#DEPENDIES, LIBARIES AND EXECUTABLES
add_library(datalisp ${DL_Src} ${DL_Hdr})
target_compile_definitions(datalisp PRIVATE "DL_LIB_BUILD" "$<$<CONFIG:Build>:DL_DEBUG>")
if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(datalisp PUBLIC "DL_LIB_STATIC")
  set_target_properties(datalisp PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
endif()

IF(DL_HAS_PYTHON)
	add_library(pydatalisp SHARED src/python/module.cpp)
  target_link_libraries(pydatalisp PRIVATE datalisp Boost::python ${PYTHON_LIBRARIES})
  set_target_properties(pydatalisp PROPERTIES DEBUG_POSTFIX "")# Disable suffix for this target
  set_target_properties(pydatalisp PROPERTIES PREFIX "${PYTHON_MODULE_PREFIX}"
                                              SUFFIX "${PYTHON_MODULE_EXTENSION}")
  target_include_directories(pydatalisp PRIVATE ${PYTHON_INCLUDE_DIRS})
ENDIF()

IF(DL_WITH_TOOLS)
	add_executable(dl_dump src/dump/main.cpp)
	target_link_libraries(dl_dump datalisp)
ENDIF(DL_WITH_TOOLS)

IF(DL_BUILD_TESTS)
  MESSAGE(STATUS "Building DataLisp tests")
  enable_testing()

  FUNCTION(PUSH_TEST name files)
    add_executable(dl_test_${name} ${files})
    target_link_libraries(dl_test_${name} datalisp)
    add_test(NAME ${name} COMMAND dl_test_${name})
  ENDFUNCTION(PUSH_TEST)

  PUSH_TEST(expr src/tests/expr_test.cpp)
  PUSH_TEST(unicode src/tests/unicode_test.cpp)
  PUSH_TEST(float src/tests/float_test.cpp)
ENDIF()

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

IF(DL_WITH_TOOLS)
install(TARGETS dl_dump
        RUNTIME DESTINATION bin)
ENDIF()

SET(DL_Hdr_INSTALL
  ${CMAKE_CURRENT_BINARY_DIR}/DataLispConfig.h
  src/Data.h
  src/Data.inl
  src/DataContainer.h
  src/DataGroup.h
  src/DataLisp.h
  src/DataType.h
  src/SourceLogger.h
  src/VM.h)

install(FILES ${DL_Hdr_INSTALL} DESTINATION include/DL)
install(EXPORT datalisp NAMESPACE dl_ DESTINATION lib/DL)

IF(DL_BUILD_DOCUMENTATION)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/doc DESTINATION share/DL)
ENDIF()