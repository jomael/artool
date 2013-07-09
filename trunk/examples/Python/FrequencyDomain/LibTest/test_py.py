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

mySim = ARTCreateSimulator("MySimulator", "FrequencyDomain", "MultiModal")

# Create elements
myCyl = ARTCreateElement(mySim, "Cyl", "Cylinder")
myCon = ARTCreateElement(mySim, "Con", "Cone")
myBes = ARTCreateElement(mySim, "Bes", "Besselhorn")

# Set parameters
ARTSetParameter(mySim, "Cyl.r = 1; Cyl.length = 100;")

ARTSetParameter(mySim, "Con.length = 50;")
ARTSetParameter(mySim, "Con.r1 = Cyl.r; Con.r2 = 1.5 * Con.r1;")

ARTSetParameter(mySim, "Bes.length = 20; Bes.r1 = Con.r2;")
ARTSetParameter(mySim, "Bes.r2 = 12.5;")
ARTSetParameter(mySim, "Bes.flare = 0.4;")

# Create circuit and add elements to circuit
myIns = ARTCreateCircuit(mySim, "MyInstrument")

ARTAppendReference(myIns, myCyl)
ARTAppendReference(myIns, myCon)
ARTAppendReference(myIns, myBes)

# Set simulation properties
ARTSetFrequencyRange(mySim, 10, 1800, 10)
ARTSetNModes(mySim, 1)

print "Calculating...",
myImpCurve = ARTGetValue(ARTInputImpedance(myIns))
print "Done.\n"
print "Frequency     Impedance"

for i in range(0, ARTGetLength(myImpCurve)) :
	# get data structure
	tri = ARTGetTriple(myImpCurve, i)
	# compute magnitude
	mag = sqrt(tri.re*tri.re + tri.im*tri.im)
	print "{0:5.0f} Hz  ".format(tri.f),
	print "{0:12.2f} Ohm".format(mag)

raw_input("Please press ENTER to continue...\n")

ARTDestroyCircuit(mySim, myIns)

ARTDestroyElement(mySim, myCyl)
ARTDestroyElement(mySim, myCon)
ARTDestroyElement(mySim, myBes)

ARTDestroySimulator(mySim)

ARTRootDestroy()


