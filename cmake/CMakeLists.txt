cmake_minimum_required (VERSION 2.6)

project(art)

include_directories(../include ../include/ga ../include/art ../include/fortran ../include/muParser ../source)

set(ART_SOURCE_DIR ../source/art)
set(GA_SOURCE_DIR ../source/ga)
set(MUPARSER_SOURCE_DIR ../source/muParser)
set(FORTRAN_SOURCE_DIR ../source/fortran)

add_executable(art 

	${FORTRAN_SOURCE_DIR}/cbabk2.c
	${FORTRAN_SOURCE_DIR}/cbal.c
	${FORTRAN_SOURCE_DIR}/cdiv.c
	${FORTRAN_SOURCE_DIR}/cg.c
	${FORTRAN_SOURCE_DIR}/comqr2.c
	${FORTRAN_SOURCE_DIR}/comqr.c
	${FORTRAN_SOURCE_DIR}/corth.c
	${FORTRAN_SOURCE_DIR}/csroot.c
	${FORTRAN_SOURCE_DIR}/pythag.c

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

	${ART_SOURCE_DIR}/ART.cpp # y (w)
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
	${ART_SOURCE_DIR}/simplematrix.cpp # y
	${ART_SOURCE_DIR}/strparsing.cpp # y
	${ART_SOURCE_DIR}/test_main.cpp # y
	${ART_SOURCE_DIR}/WindInstrument.cpp # y 
)

# add standard definitions for release build
if (DEFINED TEST)
	add_definitions(-D_DEBUG -DARTinterfaceTest)
else()
	add_definitions(-D_DEBUG -D_CONSOLE)
endif()

if (DEFINED GNU)
	set_target_properties(art
		PROPERTIES COMPILE_FLAGS "-Wno-deprecated -Wno-write-strings"
		COMPILE_DEFINITIONS __stdcall=
		LINK_FLAGS "-Xlinker tokens.lds"
	)
endif()

