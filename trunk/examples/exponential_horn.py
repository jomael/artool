#!/usr/bin/env python

"""Test libartsim library with Python."""

from artsimdefs import *;
from math import exp;

# init simulation
pSim = ARTRootObject();

# create simulator
sim = ARTCreateSimulator("TimeSimulator", "TimeDomain", "");

delayModules = list();
junctionModules = list();

# create list of delay modules and cone junctions
for i in range(0, 36) :
  r1 = 2 + exp(0.015*(331.0/44100.0) * 1000 * i);
  r2 = 2 + exp(0.015*(331.0/44100.0) * 1000 * (i + 1));
  delayModules.append(ARTCreateTModule(sim, "DelayModule{0}".format(i), "TimeModule"));
  if (delayModules[i] == None):
    print ARTGetLastErrorMessage();
  if (ARTAddOPortToTModule(delayModules[i], "p2p", "p2p[t] = p1p[t-1]") == 0):
    print ARTGetLastErrorMessage();
  if (ARTAddOPortToTModule(delayModules[i], "p1m", "p1m[t] = p2m[t-1]") == 0):
    print ARTGetLastErrorMessage();
  junctionModules.append(ARTCreateTModule(sim, "CylinderJunction{0}".format(i), "DWGCylinderJunctionModule"));
  if (junctionModules[i] == None):
    print ARTGetLastErrorMessage();
  if (ARTSetParameter(sim, "CylinderJunction{0}.r1 = {1}; CylinderJunction{0}.r2 = {2}".format(i, r1, r2)) == None):
    print ARTGetLastErrorMessage();
  if (ARTConnectPorts(sim, "CylinderJunction{0}.p1p = DelayModule{0}.p2p; DelayModule{0}.p2m = CylinderJunction{0}.p1m".format(i)) == 0):
    print ARTGetLastErrorMessage();
  if (i != 0):
    if (ARTConnectPorts(sim, "CylinderJunction{0}.p2m = DelayModule{1}.p1m; DelayModule{1}.p1p = CylinderJunction{0}.p2p".format(i-1, i)) == 0):
      print ARTGetLastErrorMessage();
  #print "{3}:\tx = {0}, r1 = {1}, r2 = {2}".format(((331.0/44100.0) * i), r1, r2, i);
  #print "{0} {1}".format(((331.0/44100.0) * i * 1000), r1);


# add and connect final delay module
delayModules.append(ARTCreateTModule(sim, "DelayModule36", "TimeModule"));
if (delayModules[36] == None):
  print ARTGetLastErrorMessage();
if (ARTAddOPortToTModule(delayModules[36], "p2p", "p2p[t] = p1p[t-1]") == 0):
  print ARTGetLastErrorMessage();
if (ARTAddOPortToTModule(delayModules[36], "p1m", "p1m[t] = p2m[t-1]") == 0):
  print ARTGetLastErrorMessage();
if (ARTConnectPorts(sim, "CylinderJunction35.p2m = DelayModule36.p1m; DelayModule36.p1p = CylinderJunction35.p2p") == 0):
  print ARTGetLastErrorMessage();

# create impulse and gain modules
impulseModule = ARTCreateTModule(sim, "Impulse", "ImpulseModule");
if (impulseModule == None):
  print ARTGetLastErrorMessage();

sineModule = ARTCreateTModule(sim, "Sine", "SinewaveModule");
if (sineModule == None):
  print ARTGetLastErrorMessage();

gainModule = ARTCreateTModule(sim, "Gain", "AmplificationModule");
if (gainModule == None):
  print ARTGetLastErrorMessage();


# connect output ports of remaining Modules
if (ARTConnectPorts(sim, "DelayModule36.p2m = Gain.out; Gain.in = DelayModule36.p2p") == 0):
  print ARTGetLastErrorMessage();

if (ARTConnectPorts(sim, "DelayModule0.p1p = Impulse.out") == 0):
  print ARTGetLastErrorMessage();


# set local parameters of each module
if (ARTSetParameter(sim, "Gain.A = -0.3") == None):
  print ARTGetLastErrorMessage();

if (ARTSetParameter(sim, "Sine.f = 1000") == None):
  print ARTGetLastErrorMessage();

# get output port
outputPort = ARTGetPortFromTModule(delayModules[0], "p1m");
if (outputPort == None):
  print ARTGetLastErrorMessage();

# initialize all modules to increase buffer size
for i in range (0,36):
  if (ARTSetParameter(sim, "DelayModule{0}.p2p[-1] = 0; DelayModule{0}.p1m[-1] = 0".format(i)) == None):
    print ARTGetLastErrorMessage();
  if (ARTSetParameter(sim, "CylinderJunction{0}.p2p[-1] = 0; CylinderJunction{0}.p1m[-1] = 0".format(i)) == None):
    print ARTGetLastErrorMessage();

if (ARTSetParameter(sim, "DelayModule36.p2p[-1] = 0; DelayModule36.p1m[-1] = 0") == None):
  print ARTGetLastErrorMessage();

for i in range(0, 441):
  # get data structure
  outVal = ARTGetComplexFromPort(outputPort, i);
  error = ARTGetLastErrorMessage();
  if (error != ""):
    print error;
    break;
  print "{0:.10f} {1}".format(i/44.1,outVal.re);

ARTRootDestroy();


