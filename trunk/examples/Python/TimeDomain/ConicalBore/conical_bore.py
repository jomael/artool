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
sim = ARTCreateSimulator("TimeSimulator", "TimeDomain", "")
if (sim == None):
  print ARTGetLastErrorMessage()

# adding speed of sound as global simulation parameter
ARTAddGlobalParamToTSimulator(sim, "c", "c = 331")

# create time modules
impulseModule = ARTCreateTModule(sim, "Impulse", "ImpulseModule")
if (impulseModule == None):
  print ARTGetLastErrorMessage()

sineModule = ARTCreateTModule(sim, "Sine", "SinewaveModule")
if (sineModule == None):
  print ARTGetLastErrorMessage()

addModule = ARTCreateTModule(sim, "Add", "AddModule")
if (addModule == None):
  print ARTGetLastErrorMessage()

leftCylinder = ARTCreateTModule(sim, "LeftCylinder", "DWGCylinderModule")
if (leftCylinder == None):
  print ARTGetLastErrorMessage()

leftConeJunction = ARTCreateTModule(sim, "LeftConeJunction", "DWGConeJunctionModule")
if (leftConeJunction == None):
  print ARTGetLastErrorMessage()

cone = ARTCreateTModule(sim, "Cone", "DWGConeModule")
if (cone == None):
  print ARTGetLastErrorMessage()

rightCylinder = ARTCreateTModule(sim, "RightCylinder", "DWGCylinderModule")
if (rightCylinder == None):
  print ARTGetLastErrorMessage()

rightConeJunction = ARTCreateTModule(sim, "RightConeJunction", "DWGConeJunctionModule")
if (rightConeJunction == None):
  print ARTGetLastErrorMessage()

gain1 = ARTCreateTModule(sim, "Gain1", "AmplificationModule")
if (gain1 == None):
  print ARTGetLastErrorMessage()

gain2 = ARTCreateTModule(sim, "Gain2", "AmplificationModule")
if (gain2 == None):
  print ARTGetLastErrorMessage()

# connect output ports of modules
if (ARTConnectPorts(sim, "LeftCylinder.p2m = LeftConeJunction.p1m; LeftConeJunction.p1p = LeftCylinder.p2p") == 0):
  print ARTGetLastErrorMessage()

if (ARTConnectPorts(sim, "LeftConeJunction.p2m = Cone.p1m; Cone.p1p = LeftConeJunction.p2p") == 0):
  print ARTGetLastErrorMessage()

if (ARTConnectPorts(sim, "Cone.p2m = RightConeJunction.p1m; RightConeJunction.p1p = Cone.p2p") == 0):
  print ARTGetLastErrorMessage()

if (ARTConnectPorts(sim, "RightConeJunction.p2m = RightCylinder.p1m; RightCylinder.p1p = RightConeJunction.p2p") == 0):
  print ARTGetLastErrorMessage()

if (ARTConnectPorts(sim, "Gain2.in = RightCylinder.p2p; RightCylinder.p2m = Gain2.out") == 0):
  print ARTGetLastErrorMessage()

if (ARTConnectPorts(sim, "Gain1.in = LeftCylinder.p1m; Add.in1 = Gain1.out; Add.in2 = Impulse.out; LeftCylinder.p1p = Add.out") == 0):
  print ARTGetLastErrorMessage()

# use this line if you want to see the system response to a sine wave signal
#if (ARTConnectPorts(sim, "Gain1.in = LeftCylinder.p1m; Add.in1 = Gain1.out; Add.in2 = Sine.out; LeftCylinder.p1p = Add.out") == 0):
#  print ARTGetLastErrorMessage()


# set local parameters of each module
if (ARTSetParameter(sim, "LeftCylinder.length = 0.171; LeftCylinder.type = 'lagrange'") == None):
  print ARTGetLastErrorMessage()

if (ARTSetParameter(sim, "LeftConeJunction.r1 = 1E300; LeftConeJunction.S1 = 0.000113097; LeftConeJunction.r2 = -0.570031578; LeftConeJunction.S2 = 0.0001131; LeftConeJunction.method = 'BT'") == None):
#if (ARTSetParameter(sim, "LeftConeJunction.lr1 = 0.006; LeftConeJunction.lr2 = 0.006; LeftConeJunction.llength = 0.171; LeftConeJunction.rr1 = 0.006; LeftConeJunction.rr2 = 0.004; LeftConeJunction.rlength = 0.19; LeftConeJunction.mode = 'boreprofile'; LeftConeJunction.method = 'BT'") == None):
  print ARTGetLastErrorMessage()

if (ARTSetParameter(sim, "Cone.r1 = 0.570031578; Cone.r2 = 0.380021052; Cone.type = 'lagrange'") == None):
#if (ARTSetParameter(sim, "Cone.r1 = 0.006; Cone.r2 = 0.004; Cone.length = 0.19;  Cone.mode = 'boreprofile'; Cone.type = 'lagrange'") == None):
  print ARTGetLastErrorMessage()

if (ARTSetParameter(sim, "RightConeJunction.r1 = -0.380021052; RightConeJunction.S1 = 5.026675912E-5; RightConeJunction.r2 = 1E300; RightConeJunction.S2 = 5.026548246E-5; RightConeJunction.method = 'BT'") == None):
#if (ARTSetParameter(sim, "RightConeJunction.lr1 = 0.006; RightConeJunction.lr2 = 0.004; RightConeJunction.llength = 0.19; RightConeJunction.rr1 = 0.004; RightConeJunction.rr2 = 0.004; RightConeJunction.rlength = 0.202; RightConeJunction.mode = 'boreprofile'; RightConeJunction.method = 'BT'") == None):
  print ARTGetLastErrorMessage()

if (ARTSetParameter(sim, "RightCylinder.length = 0.202; RightCylinder.type = 'lagrange'") == None):
  print ARTGetLastErrorMessage()

if (ARTSetParameter(sim, "Gain1.A = 0; Gain2.A = -0") == None):
  print ARTGetLastErrorMessage()

if (ARTSetParameter(sim, "Sine.f = 440") == None):
  print ARTGetLastErrorMessage()

# get output port
outputPort = ARTGetPortFromTModule(leftCylinder, "p1m")
if (outputPort == None):
  print ARTGetLastErrorMessage()

# initialize all modules to increase buffer size
for i in range (-1,-56,-1):
  addString = "Add.out[{0}] = 0;".format(i)
  leftCylinderString = "LeftCylinder.p2p[{0}] = 0; LeftCylinder.p1m[{0}] = 0;".format(i)
  leftJunctionString = "LeftConeJunction.p2p[{0}] = 0; LeftConeJunction.p1m[{0}] = 0;".format(i)
  coneString = "Cone.p2p[{0}] = 0; Cone.p1m[{0}] = 0;".format(i)
  rightJunctionString = "RightConeJunction.p2p[{0}] = 0; RightConeJunction.p1m[{0}] = 0;".format(i)
  rightCylinderString = "RightCylinder.p1m[{0}] = 0; RightCylinder.p2p[{0}] = 0;".format(i)
  gain2String = "Gain2.out[{0}] = 0;".format(i)
  if (ARTSetParameter(sim, addString + leftCylinderString + leftJunctionString + coneString + rightJunctionString + rightCylinderString + gain2String) == None):
    print ARTGetLastErrorMessage()

pressure = [0 for x in range(300)]
t = arange(0.0, 300/44.1, 1/44.1)

for i in range(0, 300) :
  # get data structure
  outVal = ARTGetComplexFromPort(outputPort, i)
  pressure[i] = outVal.re
  error = ARTGetLastErrorMessage()
  if (error != ""):
    print error
    break
  #print "{0} {1}".format(i/44.1,outVal.re)

plot(t,pressure)
show()

ARTRootDestroy()


