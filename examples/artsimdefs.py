
from ctypes import *
from sys import platform

# load dynamic library
if platform.startswith("win"):
	artsim = cdll.LoadLibrary("../release/artsim.dll")
elif platform.startswith("linux"):
	artsim = cdll.LoadLibrary("../release/libartsim.so")

# set function handles

ARTRootObject = artsim.ARTRootObject
ARTRootObject.restype = c_void_p
ARTRootObject.argtypes = []

ARTCreateSimulator = artsim.ARTCreateSimulator
ARTCreateSimulator.restype = c_void_p
ARTCreateSimulator.argtypes = [c_char_p, c_char_p, c_char_p]

ARTCreateElement = artsim.ARTCreateElement
ARTCreateElement.restype = c_void_p
ARTCreateElement.argtypes = [c_void_p, c_char_p, c_char_p]

ARTSetParameter = artsim.ARTSetParameter
ARTSetParameter.restype = c_void_p
ARTSetParameter.argtypes = [c_void_p, c_char_p]

ARTCreateCircuit = artsim.ARTCreateCircuit
ARTCreateCircuit.restype = c_void_p
ARTCreateCircuit.argtypes = [c_void_p, c_char_p]

ARTCreateTModule = artsim.ARTCreateTModule
ARTCreateTModule.restype = c_void_p
ARTCreateTModule.argtypes = [c_void_p, c_char_p, c_char_p]

ARTAddOPortToTModule = artsim.ARTAddOPortToTModule
ARTAddOPortToTModule.restype = c_int
ARTAddOPortToTModule.argtypes = [c_void_p, c_char_p, c_char_p]

ARTSetOPortOfFModule = artsim.ARTSetOPortOfFModule
ARTSetOPortOfFModule.restype = c_int
ARTSetOPortOfFModule.argtypes = [c_void_p, c_int, c_char_p]

ARTAddLocalParamToTModule = artsim.ARTAddLocalParamToTModule
ARTAddLocalParamToTModule.restype = c_int
ARTAddLocalParamToTModule.argtypes = [c_void_p, c_char_p, c_char_p]

ARTAddGlobalParamToTSimulator = artsim.ARTAddGlobalParamToTSimulator
ARTAddGlobalParamToTSimulator.restype = c_int
ARTAddGlobalParamToTSimulator.argtypes = [c_void_p, c_char_p, c_char_p]

ARTConnectPorts = artsim.ARTConnectPorts
ARTConnectPorts.restype = c_int
ARTConnectPorts.argtypes = [c_void_p, c_char_p]

ARTGetPortFromTModule = artsim.ARTGetPortFromTModule
ARTGetPortFromTModule.restype = c_void_p
ARTGetPortFromTModule.argtypes = [c_void_p, c_char_p]

ARTAppendReference = artsim.ARTAppendReference
ARTAppendReference.restype = c_void_p
ARTAppendReference.argtypes = [c_void_p, c_void_p]

ARTSetFrequencyRange = artsim.ARTSetFrequencyRange
ARTSetFrequencyRange.restype = c_int
ARTSetFrequencyRange.argtypes = [c_void_p, c_double, c_double, c_double]

ARTSetNModes = artsim.ARTSetNModes
ARTSetNModes.restype = c_int
ARTSetNModes.argtypes = [c_void_p, c_int]

ARTInputImpedance = artsim.ARTInputImpedance
ARTInputImpedance.restype = c_void_p
ARTInputImpedance.argtypes = [c_void_p]

ARTGetValue = artsim.ARTGetValue
ARTGetValue.restype = c_void_p
ARTGetValue.argtypes = [c_void_p]

ARTGetLength = artsim.ARTGetLength
ARTGetLength.restype = c_int
ARTGetLength.argtypes = [c_void_p]

ARTGetTriple = artsim.ARTGetTriple
ARTGetTriple.argtypes = [c_void_p, c_int]

ARTGetComplexFromPort = artsim.ARTGetComplexFromPort
ARTGetComplexFromPort.argtypes = [c_void_p, c_int]

ARTDestroyCircuit = artsim.ARTDestroyCircuit
ARTDestroyCircuit.restype = c_int
ARTDestroyCircuit.argtypes = [c_void_p, c_void_p]

ARTDestroyElement = artsim.ARTDestroyElement
ARTDestroyElement.restype = c_int
ARTDestroyElement.argtypes = [c_void_p, c_void_p]

ARTDestroySimulator = artsim.ARTDestroySimulator
ARTDestroySimulator.restype = c_int
ARTDestroySimulator.argtypes = [c_void_p]

ARTRootDestroy = artsim.ARTRootDestroy
ARTRootDestroy.restype = c_int
ARTRootDestroy.argtypes = []

ARTGetLastErrorMessage = artsim.ARTGetLastErrorMessage
ARTGetLastErrorMessage.restype = c_char_p
ARTGetLastErrorMessage.argtype = []

# define c++ classes
class T_ART_Cmplx(Structure):
	_fields_ = [("re", c_double),("im", c_double)]

class T_ART_Tripl(Structure):
	_fields_ = [("f", c_double), ("re", c_double), ("im", c_double)]

ARTGetTriple.restype = T_ART_Tripl
ARTGetComplexFromPort.restype = T_ART_Cmplx
