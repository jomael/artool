#!/usr/bin/env python

"""Test libartsim library with Python."""

from sys import path,platform

if platform.startswith("win"):
  path.append("..\..")
elif platform.startswith("linux"):
  path.append("../..")

from artsimdefs import *
from math import sqrt

# init simulation
pSim = ARTRootObject()

# create simulator
mySim = ARTCreateSimulator("MySimulator", "TimeDomain", "")

# create time modules
timeModule = ARTCreateTModule(mySim, "myModule", "TimeModule")
timeModule2 = ARTCreateTModule(mySim, "myModule2", "TimeModule")

# add output ports to time modules
ARTAddOPortToTModule(timeModule, "fib", "fib[t] = fib[t-1] + fib[t-2]")
ARTAddOPortToTModule(timeModule2, "test", "test[t] = fib[t] * l * T")

# add a local simulation parameter to second time module
ARTAddLocalParamToTModule(timeModule2, "l", "l = 0.0")

# change value of local parameter
ARTSetParameter(mySim, "myModule2.l = 1.0/T")

# connect ports of both time modules
ARTConnectPorts(mySim, "myModule2.fib = myModule.fib")

# initialize fibonacci ports
ARTSetParameter(mySim, "myModule.fib[-1] = 1; myModule.fib[-2] = 0")

testPort = ARTGetPortFromTModule(timeModule2, "test")

for i in range(0, 50) :
	# get data structure
	fibonacci = ARTGetComplexFromPort(testPort, i)
	print "fib[{0:2d}] = {1:.0f}".format(i,fibonacci.re)

raw_input("Please press ENTER to continue...\n")

ARTRootDestroy()


