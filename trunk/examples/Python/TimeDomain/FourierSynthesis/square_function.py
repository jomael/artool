#!/usr/bin/env python

"""Test libartsim library with Python."""

from sys import path,platform

if platform.startswith("win"):
  path.append("..\..")
elif platform.startswith("linux"):
  path.append("../..")

from artsimdefs import *
from math import sqrt
from pylab import *

# init simulation
pSim = ARTRootObject()

# create simulator
mySim = ARTCreateSimulator("MySimulator", "TimeDomain", "")

# add global parameter base frequency
ARTAddGlobalParamToTSimulator(mySim, "base_f", "base_f = 440")

# create time modules
baseSin = ARTCreateTModule(mySim, "baseSin", "SinewaveModule")
baseSin3 = ARTCreateTModule(mySim, "baseSin3", "SinewaveModule")
baseSin5 = ARTCreateTModule(mySim, "baseSin5", "SinewaveModule")
baseSin7 = ARTCreateTModule(mySim, "baseSin7", "SinewaveModule")
baseSin9 = ARTCreateTModule(mySim, "baseSin9", "SinewaveModule")

sumModule = ARTCreateTModule(mySim, "sumModule", "TimeModule")

# add output ports to sum module
ARTAddOPortToTModule(sumModule, "out", "out[t] = (pi/4)*(in1[t] + in2[t] + in3[t] + in4[t] + in5[t])")

# connect ports of all time modules
ARTConnectPorts(mySim, "sumModule.in1 = baseSin.out; sumModule.in2 = baseSin3.out;"
  + "sumModule.in3 = baseSin5.out; sumModule.in4 = baseSin7.out; sumModule.in5 = baseSin9.out")

# set parameters of sine wave module
ARTSetParameter(mySim, "baseSin.A = 1; baseSin.f = base_f; baseSin.Delta = 0")
ARTSetParameter(mySim, "baseSin3.A = 1/3; baseSin3.f = base_f*3; baseSin3.Delta = 0")
ARTSetParameter(mySim, "baseSin5.A = 1/5; baseSin5.f = base_f*5; baseSin5.Delta = 0")
ARTSetParameter(mySim, "baseSin7.A = 1/7; baseSin7.f = base_f*7; baseSin7.Delta = 0")
ARTSetParameter(mySim, "baseSin9.A = 1/9; baseSin9.f = base_f*9; baseSin9.Delta = 0")

# get the output port of sum module
outPort = ARTGetPortFromTModule(sumModule, "out");

square = [0 for x in range(300)]
t = arange(0.0, 300/44100.0, 1/44100.0)

# simulate about 7 ms ~ 3 periods
for i in range(0, 300):
  # get data structure
  outVal = ARTGetComplexFromPort(outPort, i)
  #print "{0:.10f} {1:.10f}".format(i/44.1,outVal.re)
  square[i] = outVal.re

#plot(t,square)
#show()


# destroy all previously created objects
ARTRootDestroy()

