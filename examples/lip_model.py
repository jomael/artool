#!/usr/bin/env python

"""Test libartsim library with Python."""

from artsimdefs import *;
from math import exp;

# init simulation
pSim = ARTRootObject();

# create simulator
sim = ARTCreateSimulator("TimeSimulator", "TimeDomain", "");

# create function module
impulseResponse = ARTCreateTModule(sim, "InstrumentResponse", "InputFunctionModule");
if (impulseResponse == None):
  print ARTGetLastErrorMessage();

# create time module with convolution
lipModule = ARTCreateTModule(sim, "lipModule", "TimeModule");
if (lipModule == None):
  print ARTGetLastErrorMessage();

# create impulse and gain modules
impulseModule = ARTCreateTModule(sim, "Impulse", "ImpulseModule");
if (impulseModule == None):
  print ARTGetLastErrorMessage();

sineModule = ARTCreateTModule(sim, "Sine", "SinewaveModule");
if (sineModule == None):
  print ARTGetLastErrorMessage();


# create output port of function module
if (ARTSetOPortOfFModule(impulseResponse, 100, "") == 0):
  print ARTGetLastErrorMessage();

# init impulse response
for i in range(0,100):
  value = 0;
  if (i == 46):
    value = 0.013;
  elif (i == 96):
    value = 0.024;
  #if (i == 0):
  #  value = 1;
  if (ARTSetParameter(sim, "InstrumentResponse.out[{0}] = {1}".format(i,value)) == None):
    print ARTGetLastErrorMessage();
  #print "InstrumentResponse.out[{0}] = {1}".format(i,value);


# add output ports to lip module
#if (ARTAddOPortToTModule(lipModule, "x", "x[t] = (-sqrt(k/m) * (1/Q) * ((x[t-1] - x[t-2])/T) + ((b*d)/m)* p_lip[t-2] - (k/m)*x[t-2])*T^2 - x[t-2] + 2*x[t-1]") == 0):
if (ARTAddOPortToTModule(lipModule, "x", "x[t] = (x[t-1]*(2+sqrt(k/m)*(T/Q)) - x[t-2] + ((b*d*(T^2))/m)*p_lip[t-1])/(1+sqrt(k/m)*(T/Q)+(k*T^2)/m)") == 0):
#if (ARTAddOPortToTModule(lipModule, "x", "x[t] = (x[t-1]*(2+0.01*T) - x[t-2])/(1+0.01*T+2*T)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "S_lip", "S_lip[t] = max(b*(2*x[t] + x_0), 0.00001)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U", "U[t] = ((p_0 - p[t-1])*(S_lip[t-1]/rho) + ((U[t - 1])^2) * (-1/(2*S_lip[t-1]) + 1/S_cup + S_lip[t-1]/(S_cup^2)) ) * (T/d) + U[t-1]") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "p_lip", "p_lip[t] = -rho * U[t]^2 * (1/(S_cup * S_lip[t]) - 1/(S_cup^2)) + p[t]") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "p", "p[t] = conv(U, G, t)") == 0):
  print ARTGetLastErrorMessage();

# connect output ports of impulseResponse and sine module
if (ARTConnectPorts(sim, "lipModule.G = InstrumentResponse.out") == 0):
  print ARTGetLastErrorMessage();

#if (ARTConnectPorts(sim, "lipModule.x = Sine.out") == 0):
#  print ARTGetLastErrorMessage();

# add local parameters to lip module
# speed of sound
if (ARTAddLocalParamToTModule(lipModule, "c", "c = 3.4E2") == 0):
  print ARTGetLastErrorMessage();
# average air density
if (ARTAddLocalParamToTModule(lipModule, "rho", "rho = 1.2") == 0):
  print ARTGetLastErrorMessage();
# area of mouthpiece entryway
if (ARTAddLocalParamToTModule(lipModule, "S_cup", "S_cup = 2.3E-4") == 0):
  print ARTGetLastErrorMessage();
# breadth of lip orifice
if (ARTAddLocalParamToTModule(lipModule, "b", "b = 8.0E-3") == 0):
  print ARTGetLastErrorMessage();
# thickness of lips
if (ARTAddLocalParamToTModule(lipModule, "d", "d = 2.0E-3") == 0):
  print ARTGetLastErrorMessage();
# equilibrium lip opening length
if (ARTAddLocalParamToTModule(lipModule, "x_0", "x_0 = 2.4E-3") == 0):
  print ARTGetLastErrorMessage();
# lip quality factor
if (ARTAddLocalParamToTModule(lipModule, "Q", "Q = 5.0") == 0):
  print ARTGetLastErrorMessage();
# lip resonance frequency
if (ARTAddLocalParamToTModule(lipModule, "f_lip", "f_lip = 260") == 0):
  print ARTGetLastErrorMessage();
# lip mass define 1.5/(((2*PI)^2)/f_lip
if (ARTAddLocalParamToTModule(lipModule, "m", "m = 1.5/(((2*pi)^2)*(f_lip))") == 0):
  print ARTGetLastErrorMessage();
# stiffness of lips
if (ARTAddLocalParamToTModule(lipModule, "k", "k = 1.5*f_lip") == 0):
  print ARTGetLastErrorMessage();
# blowing pressure
if (ARTAddLocalParamToTModule(lipModule, "p_0", "p_0 = 6.0") == 0):
  print ARTGetLastErrorMessage();

# set global sampling frequency
if (ARTSetParameter(sim, "T = 1/8000") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "Sine.f = 260") == None):
  print ARTGetLastErrorMessage();

# get output port
outputPort = ARTGetPortFromTModule(lipModule, "U");
if (outputPort == None):
  print ARTGetLastErrorMessage();

# initialize all modules to increase buffer size
if (ARTSetParameter(sim, "lipModule.x[-1] = -0.0002; lipModule.x[-2] = -0.00019") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.p_lip[-1] = 0; lipModule.p_lip[-2] = 0") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.S_lip[-1] = 0.000001;") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.U[-1] = 0;") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.p[-1] = 1;") == None):
  print ARTGetLastErrorMessage();

for i in range(0, 800):
#for i in range(0, 40):
  # get data structure
  outVal = ARTGetComplexFromPort(outputPort, i);
  error = ARTGetLastErrorMessage();
  if (error != ""):
    print error;
    break;
  print "{0:.10f} {1}".format(i/44.1,outVal.re);
  #print "{0} {1}".format(i,outVal.im);

ARTRootDestroy();


