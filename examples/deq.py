#!/usr/bin/env python

"""Test libartsim library with Python."""

from artsimdefs import *;
from math import sqrt;

# init simulation
pSim = ARTRootObject();

# create simulator
mySim = ARTCreateSimulator("MySimulator", "TimeDomain", "");

# create time modules
exactModule = ARTCreateTModule(mySim, "exactSolution", "TimeModule");
numericModule = ARTCreateTModule(mySim, "numericSolution", "TimeModule");

# add output ports to time modules
ARTAddOPortToTModule(exactModule, "out", "out[t] = (1/25)*exp(-t*T)*(27*cos(2*t*T) + 11*sin(2*t*T)) + (1/25)*(5*t*T - 2)");
ARTAddOPortToTModule(numericModule, "out", "out[t] = (2*out[t-1]*(1+T) - out[t-2] + T^3*t)/(1 + 2*T + 5*T^2)");

outPort = ARTGetPortFromTModule(exactModule, "out");
outPort2 = ARTGetPortFromTModule(numericModule, "out");

ARTSetParameter(mySim, "T = 1/1000");
ARTSetParameter(mySim, "numericSolution.out[-1] = 1; numericSolution.out[-2] = 1");

print ARTGetLastErrorMessage();

for i in range(0, 10000) :
	# get data structure
	outVal = ARTGetComplexFromPort(outPort, i);
	outVal2 = ARTGetComplexFromPort(outPort2, i);
	#print "out[{0:2d}] = {1:.10f}".format(i,outVal.re);
	if (i % 10 == 0) :
		print "{0:.10f} {1:.10f}".format(i*0.001,outVal.re - outVal2.re);

print ARTGetLastErrorMessage();
#raw_input("Please press ENTER to continue...\n");

ARTRootDestroy();


