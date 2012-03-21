cmake_minimum_required (VERSION 2.6)

# a configuration has to be specified
if ((NOT DEFINED RELEASE) AND (NOT DEFINED DEBUG) AND (NOT DEFINED ARTINTERFACEDEBUG))
	message(FATAL_ERROR
		"Please specify one of the following configurations:\n\tRELEASE\n\tDEBUG\n\tARTINTERFACEDEBUG\ne.g. \"cmake -DRELEASE=ON .\""
	)
endif()

project(art)

include_directories(../include ../include/ga ../include/art ../include/fortran ../include/muParser ../source)

set(ART_SOURCE_DIR ../source/art)
set(GA_SOURCE_DIR ../source/ga)
set(MUPARSER_SOURCE_DIR ../source/muParser)
set(FORTRAN_SOURCE_DIR ../source/fortran)

if (DEFINED RELEASE)
	set(LIBRARY_OUTPUT_PATH ../release)
	set(EXECUTABLE_OUTPUT_PATH ../release)
elseif (DEFINED DEBUG)
	set(LIBRARY_OUTPUT_PATH ../debug)
	set(EXECUTABLE_OUTPUT_PATH ../debug)
elseif (DEFINED ARTINTERFACEDEBUG)
	set(LIBRARY_OUTPUT_PATH ../debug_artinterface)
	set(EXECUTABLE_OUTPUT_PATH ../debug_artinterface)
endif()

set(FORTRAN_SOURCES 
	${FORTRAN_SOURCE_DIR}/cbabk2.c
	${FORTRAN_SOURCE_DIR}/cbal.c
	${FORTRAN_SOURCE_DIR}/cdiv.c
	${FORTRAN_SOURCE_DIR}/cg.c
	${FORTRAN_SOURCE_DIR}/comqr2.c
	${FORTRAN_SOURCE_DIR}/comqr.c
	${FORTRAN_SOURCE_DIR}/corth.c
	${FORTRAN_SOURCE_DIR}/csroot.c
	${FORTRAN_SOURCE_DIR}/pythag.c
)

source_group(fortran_src FILES
	${FORTRAN_SOURCES}
)

set(GA_SOURCES
	${GA_SOURCE_DIR}/GA1DArrayGenome.cpp
	${GA_SOURCE_DIR}/GA1DBinStrGenome.cpp
	${GA_SOURCE_DIR}/GA2DArrayGenome.cpp
	${GA_SOURCE_DIR}/GA2DBinStrGenome.cpp
	${GA_SOURCE_DIR}/GA3DArrayGenome.cpp
	${GA_SOURCE_DIR}/GA3DBinStrGenome.cpp
	${GA_SOURCE_DIR}/GAAllele.cpp
	${GA_SOURCE_DIR}/GABaseGA.cpp
	${GA_SOURCE_DIR}/GABin2DecGenome.cpp
	${GA_SOURCE_DIR}/gabincvt.cpp
	${GA_SOURCE_DIR}/GABinStr.cpp
	${GA_SOURCE_DIR}/GADCrowdingGA.cpp
	${GA_SOURCE_DIR}/GADemeGA.cpp
	${GA_SOURCE_DIR}/gaerror.cpp
	${GA_SOURCE_DIR}/GAGenome.cpp
	${GA_SOURCE_DIR}/GAIncGA.cpp
	${GA_SOURCE_DIR}/GAListBASE.cpp
	${GA_SOURCE_DIR}/GAList.cpp
	${GA_SOURCE_DIR}/GAListGenome.cpp
	${GA_SOURCE_DIR}/GAParameter.cpp
	${GA_SOURCE_DIR}/GAPopulation.cpp
	${GA_SOURCE_DIR}/garandom.cpp
	${GA_SOURCE_DIR}/GARealGenome.cpp
	${GA_SOURCE_DIR}/GAScaling.cpp
	${GA_SOURCE_DIR}/GASelector.cpp
	${GA_SOURCE_DIR}/GASimpleGA.cpp
	${GA_SOURCE_DIR}/GASStateGA.cpp
	${GA_SOURCE_DIR}/GAStatistics.cpp
	${GA_SOURCE_DIR}/GAStringGenome.cpp
	${GA_SOURCE_DIR}/GATreeBASE.cpp
	${GA_SOURCE_DIR}/GATree.cpp
	${GA_SOURCE_DIR}/GATreeGenome.cpp
)

source_group(ga_src FILES
	${GA_SOURCES}
)

set(MUPARSER_SOURCES
	${MUPARSER_SOURCE_DIR}/mpARTValue.cpp # y (w)
	${MUPARSER_SOURCE_DIR}/mpError.cpp
	${MUPARSER_SOURCE_DIR}/mpFuncCmplx.cpp
	${MUPARSER_SOURCE_DIR}/mpFuncCommon.cpp
	${MUPARSER_SOURCE_DIR}/mpFuncNonCmplx.cpp
	${MUPARSER_SOURCE_DIR}/mpFuncStr.cpp
	${MUPARSER_SOURCE_DIR}/mpICallback.cpp
	${MUPARSER_SOURCE_DIR}/mpIfThenElse.cpp
	${MUPARSER_SOURCE_DIR}/mpIOprt.cpp
	${MUPARSER_SOURCE_DIR}/mpIPackage.cpp
	${MUPARSER_SOURCE_DIR}/mpIToken.cpp
	${MUPARSER_SOURCE_DIR}/mpIValReader.cpp
	${MUPARSER_SOURCE_DIR}/mpIValue.cpp
	${MUPARSER_SOURCE_DIR}/mpOprtBinAssign.cpp
	${MUPARSER_SOURCE_DIR}/mpOprtBinCommon.cpp
	${MUPARSER_SOURCE_DIR}/mpOprtCmplx.cpp
	${MUPARSER_SOURCE_DIR}/mpOprtNonCmplx.cpp
	${MUPARSER_SOURCE_DIR}/mpOverload.cpp
	${MUPARSER_SOURCE_DIR}/mpPackageCmplx.cpp
	${MUPARSER_SOURCE_DIR}/mpPackageCommon.cpp
	${MUPARSER_SOURCE_DIR}/mpPackageNonCmplx.cpp
	${MUPARSER_SOURCE_DIR}/mpPackageStr.cpp
	${MUPARSER_SOURCE_DIR}/mpPackageUnit.cpp
	${MUPARSER_SOURCE_DIR}/mpParserBase.cpp
	${MUPARSER_SOURCE_DIR}/mpParser.cpp
	${MUPARSER_SOURCE_DIR}/mpRPN.cpp
	${MUPARSER_SOURCE_DIR}/mpTest.cpp
	${MUPARSER_SOURCE_DIR}/mpTokenReader.cpp
	${MUPARSER_SOURCE_DIR}/mpValReader.cpp
	${MUPARSER_SOURCE_DIR}/mpValueCache.cpp
	${MUPARSER_SOURCE_DIR}/mpValue.cpp
	${MUPARSER_SOURCE_DIR}/mpVariable.cpp
)

source_group(muparser_src FILES
	${MUPARSER_SOURCES}
)

set(ART_SOURCES
	${ART_SOURCE_DIR}/art.cpp # y (w)
	${ART_SOURCE_DIR}/ARTdataContainer.cpp # y
	${ART_SOURCE_DIR}/ARTerror.cpp # y
	${ART_SOURCE_DIR}/ARTfunctionoid.cpp # y
	${ART_SOURCE_DIR}/ARTlink.cpp # y
	${ART_SOURCE_DIR}/HornElement.cpp # y
	${ART_SOURCE_DIR}/ImpedanceCurve.cpp # y
	${ART_SOURCE_DIR}/Interface.cpp # y (w)
	${ART_SOURCE_DIR}/legquadrature.cpp # y
	${ART_SOURCE_DIR}/rosenbrock.cpp # y
	${ART_SOURCE_DIR}/runga.cpp # y
	${ART_SOURCE_DIR}/runrb.cpp # y
	${ART_SOURCE_DIR}/SimpleMatrix.cpp # y
	${ART_SOURCE_DIR}/strparsing.cpp # y
	${ART_SOURCE_DIR}/test_main.cpp # y
	${ART_SOURCE_DIR}/WindInstrument.cpp # y 
)

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

add_executable(art 
	${ART_SOURCES}
)

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



# add standard definitions for release build
if (DEFINED ARTINTERFACEDEBUG)
	add_definitions(-D_DEBUG -DARTinterfaceTest)
elseif ((DEFINED RELEASE) OR (DEFINED DEBUG))
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
	if (DEFINED CMAKE_COMPILER_IS_GNUCXX AND (NOT DEFINED WIN32))
		set_target_properties(artsim
			PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings"
			COMPILE_DEFINITIONS "DLL"
			LINK_FLAGS "-Xlinker tokens.lds"
		)
	elseif (DEFINED MSVC)
		set_target_properties(artsim
			PROPERTIES COMPILE_FLAGS "/W0"
			COMPILE_DEFINITIONS "DLL"
		)
	endif()
	if (DEFINED RELEASE)
		add_definitions(-D_CONSOLE)
	else()
		add_definitions(-D_DEBUG -D_CONSOLE)
	endif()
endif()

if (DEFINED CMAKE_COMPILER_IS_GNUCXX AND (NOT DEFINED WIN32))
	set_target_properties(art
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings"
		LINK_FLAGS "-Xlinker tokens.lds"
	)
	set_target_properties(fortran
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings"
	)
	set_target_properties(ga
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings"
	)
	set_target_properties(muparser
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings"
	)
elseif (DEFINED MSVC)
	set_target_properties(art
		PROPERTIES COMPILE_FLAGS "/W0"
	)
	set_target_properties(fortran
		PROPERTIES COMPILE_FLAGS "/W0"
	)
	set_target_properties(ga
		PROPERTIES COMPILE_FLAGS "/W0"
	)
	set_target_properties(muparser
		PROPERTIES COMPILE_FLAGS "/W0"
	)
endif()

# enable packaging for release build
if ((DEFINED RELEASE) AND (DEFINED UNIX))

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

