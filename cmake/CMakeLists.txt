cmake_minimum_required (VERSION 2.6)

option(RELEASE "Create project for the release version of ARTool. Default is set to \"OFF\"" OFF)
option(DEBUG "Create project for the interactive debug version of ARTool. Default is set to \"OFF\"" OFF)
option(ARTINTERFACEDEBUG "Create project for the non-interactive debug version of ARTool. It includes unit and integration tests which should all pass successfully. Default is set to \"OFF\"" OFF)

set(NUMBER_OF_OPTIONS 0)

if (RELEASE)
	math(EXPR NUMBER_OF_OPTIONS ${NUMBER_OF_OPTIONS}+1)
endif()

if (DEBUG)
	math(EXPR NUMBER_OF_OPTIONS ${NUMBER_OF_OPTIONS}+1)
endif()

if (ARTINTERFACEDEBUG)
	math(EXPR NUMBER_OF_OPTIONS ${NUMBER_OF_OPTIONS}+1)
endif()

# a configuration has to be specified
#if ((NOT DEFINED RELEASE) AND (NOT DEFINED DEBUG) AND (NOT DEFINED ARTINTERFACEDEBUG))
if (NOT (NUMBER_OF_OPTIONS EQUAL 1))
	message(FATAL_ERROR
		"Please specify one of the following options:\n\tRELEASE\n\tDEBUG\n\tARTINTERFACEDEBUG\ne.g. \"cmake -DRELEASE=ON .\""
	)
# MS Visual Studio is causing some issues when the project file is created by CMake.
#elseif (DEFINED UNIX)
elseif (DEFINED MSVC)
	message(FATAL_ERROR
		"This CMakeFile is only compatible with GCC. Please use any of the available MS Visual Studio Solution files for building ARTool on Windows with a Microsoft Compiler."
	)	
endif()

project(art)

include_directories(../ART/include ../ARTTest/include ../ART/include/ga ../ART/include/art ../ART/include/fortran ../ART/include/muParser ../ARTTest/include/gtest ../ARTTest/include/gmock ../ART/source ../ART/source/art ../ARTTest/source)

set(ART_SOURCE_DIR ../ART/source/art)
set(GA_SOURCE_DIR ../ART/source/ga)
set(MUPARSER_SOURCE_DIR ../ART/source/muParser)
set(FORTRAN_SOURCE_DIR ../ART/source/fortran)

set(GTEST_SOURCE_DIR ../ARTTest/source/gtest)
set(GMOCK_SOURCE_DIR ../ARTTest/source/gmock)
set(ARTTEST_SOURCE_DIR ../ARTTest/source/ARTTest)

if (RELEASE)
	set(LIBRARY_OUTPUT_PATH ../out/release)
	set(EXECUTABLE_OUTPUT_PATH ../out/release)
elseif (DEBUG)
	set(LIBRARY_OUTPUT_PATH ../out/debug)
	set(EXECUTABLE_OUTPUT_PATH ../out/debug)
elseif (ARTINTERFACEDEBUG)
	set(LIBRARY_OUTPUT_PATH ../out/debug_artinterface)
	set(EXECUTABLE_OUTPUT_PATH ../out/debug_artinterface)
endif()

aux_source_directory(${FORTRAN_SOURCE_DIR} FORTRAN_SOURCES)

source_group(fortran_src FILES
	${FORTRAN_SOURCES}
)

aux_source_directory(${GA_SOURCE_DIR} GA_SOURCES)

source_group(ga_src FILES
	${GA_SOURCES}
)

aux_source_directory(${MUPARSER_SOURCE_DIR} MUPARSER_SOURCES)

source_group(muparser_src FILES
	${MUPARSER_SOURCES}
)

aux_source_directory(${ART_SOURCE_DIR} ART_SOURCES)

source_group(art_src FILES
	${ART_SOURCES}
)

add_library(fortran STATIC
	${FORTRAN_SOURCES}
)

add_library(ga STATIC
	${GA_SOURCES}
)

add_library(muparser STATIC
	${MUPARSER_SOURCES}
)

if (RELEASE OR DEBUG)
	add_executable(art 
		${ART_SOURCES}
	)
endif()

if (ARTINTERFACEDEBUG)
	set(GTEST_SOURCES 
		${GTEST_SOURCE_DIR}/gtest-all.cc
	)

	set(GMOCK_SOURCES 
		${GMOCK_SOURCE_DIR}/gmock-all.cc
	)

	source_group(gtest_src FILES
		${GTEST_SOURCES}
	)

	source_group(gmock_src FILES
		${GMOCK_SOURCES}
	)

	aux_source_directory(${ARTTEST_SOURCE_DIR} ARTTEST_SOURCES)

	source_group(ARTTest_src FILES
		${ARTTEST_SOURCES}
	)	

	add_library(gtest STATIC
		${GTEST_SOURCES}
	)

	add_library(gmock STATIC
		${GMOCK_SOURCES}
	)

	add_library(art STATIC
		${ART_SOURCES}
	)

	add_executable(arttest ${ARTTEST_SOURCES})

	add_dependencies(arttest
		art
		fortran
		ga
		muparser
		gmock
		gtest
	)

	target_link_libraries(arttest
		art
		fortran
		ga
		muparser
		gmock
		gtest
      )

	#add_definitions(-D_DEBUG -DARTinterfaceTest -ggdb)
	add_definitions(-D_DEBUG -DARTinterfaceTest -DGTEST_HAS_PTHREAD=0 -ggdb)

	set_target_properties(gtest
	#	PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings -Wno-unused-parameter -Wall -Wextra -pedantic -std=c++11 -pthread -fPIC"
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings -Wno-unused-parameter -Wall -Wextra -pedantic -std=c++11 -fPIC"
	)

	set_target_properties(gmock
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings -Wno-unused-parameter -Wall -Wextra -pedantic -std=c++11 -fPIC"
	)

	set_target_properties(arttest
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings -Wno-unused-parameter -Wall -Wextra -pedantic -std=c++98"
		#LINK_FLAGS "-lpthread -Xlinker tokens.lds"
		LINK_FLAGS "-Xlinker tokens.lds"
	)


elseif (RELEASE OR DEBUG)

	add_dependencies(art
		fortran
		ga
		muparser
	)

	target_link_libraries(art
		fortran
		ga
		muparser
	)

	add_library(artsim SHARED
		${ART_SOURCES}
	)

	add_dependencies(artsim
		fortran
		ga
		muparser
	)

	target_link_libraries(artsim
		fortran
		ga
		muparser
	)

	set_target_properties(art
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings -Wno-unused-parameter -Wall -Wextra -pedantic -std=c++98"
		LINK_FLAGS "-Xlinker tokens.lds"
	)
	
	set_target_properties(artsim
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings -Wno-unused-parameter -Wall -Wextra -pedantic -std=c++98 -fPIC"
		COMPILE_DEFINITIONS "DLL"
		LINK_FLAGS "-Xlinker tokens.lds"
	)

	if (DEFINED RELEASE)
		add_definitions(-D_CONSOLE)
	else()
		add_definitions(-D_DEBUG -D_CONSOLE -ggdb)
	endif()

endif()

set_target_properties(fortran
	PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings -fPIC"
)
set_target_properties(ga
	PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings -fPIC"
)
set_target_properties(muparser
	PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings -fPIC"
)


# enable packaging for release build
if (RELEASE AND UNIX)

	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Acoustic Research Tool - A Simulation Framework with Modelling Library for Acoustic Systems")
	set(CPACK_PACKAGE_VENDOR "Inst. of Music Acoustics, Univ. of Music, Vienna, AT")
	set(CPACK_PACKAGE_CONTACT "http://iwk.mdw.ac.at/")
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER "http://iwk.mdw.ac.at/")
	set(CPACK_PACKAGE_VERSION_MAJOR "0")
	set(CPACK_PACKAGE_VERSION_MINOR "9")
	set(CPACK_PACKAGE_VERSION_PATCH "1")
	set(CPACK_PACKAGE_ICON "art_logo.jpg")

	include(CPack)
	install_files(/include/art .h ../include/art/ART ../include/art/ARTlink ../include/art/matrix)
	install_targets(/lib/art artsim)
	install_targets(/bin art)
endif()

