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

# create step module
stepModule = ARTCreateTModule(sim, "Step", "HeavisideModule");
if (stepModule == None):
  print ARTGetLastErrorMessage();
ARTSetParameter(sim, "Step.A = 6E3");

# init impulse response of instrument by reading external admittance file
#with open('admittance.flo', 'r') as f:
#  flen = int(f.readline())

# create and resize output port of function module
  #if (ARTSetOPortOfFModule(impulseResponse, flen, "") == 0):
if (ARTSetOPortOfFModule(impulseResponse, 2, "out[t] = (t == 0) ? 1 : 0") == 0):
  print ARTGetLastErrorMessage();

#  for i in range(0,flen-1):
#    value = float(f.readline());
#    if (ARTSetParameter(sim, "InstrumentResponse.out[{0}] = {1}".format(i,value)) == None):
#      print ARTGetLastErrorMessage();
#    print "InstrumentResponse.out[{0}] = {1}".format(i,value);

# add output ports to lip module
if (ARTAddOPortToTModule(lipModule, "y", "y[t] = a1*y[t-1] + a2*y[t-2]+b1*delta_p[t-1]+a3*(max(y[t-1] - y_c,0))^2") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "h", "h[t] = y_m - y[t]") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "u_r", "u_r[t] = (S/T)*(y[t] - y[t-1])") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "pm", "pm[t] = conv(pp, r_f, t-1)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "Lambda", "Lambda[t] = ((lambda*h[t])^2)*Z_0/rho") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "Gamma", "Gamma[t] = (2*((lambda*h[t])^2)/rho)*(2*pm[t] + Z_0*u_r[t] - p_m[t])") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "u_f", "u_f[t] = (Gamma[t] <= 0) ? (-(Lambda[t]) + sqrt(Lambda[t] - Gamma[t])) : (Lambda[t] - sqrt(Lambda[t] + Gamma[t]))") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "u", "u[t] = u_r[t] + u_f[t]") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "pp", "pp[t] = Z_0*u[t] + pm[t]") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "p", "p[t] = pp[t] + pm[t]") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "delta_p", "delta_p[t] = p_m[t] - p[t]") == 0):
  print ARTGetLastErrorMessage();


# connect output ports of impulseResponse and sine module
if (ARTConnectPorts(sim, "lipModule.r_f = InstrumentResponse.out") == 0):
  print ARTGetLastErrorMessage();
if (ARTConnectPorts(sim, "lipModule.p_m = Step.out") == 0):
  print ARTGetLastErrorMessage();

# add local parameters to lip module
# speed of sound
if (ARTAddLocalParamToTModule(lipModule, "c", "c = 3.4E2") == 0):
  print ARTGetLastErrorMessage();
# average air density
if (ARTAddLocalParamToTModule(lipModule, "rho", "rho = 1.2") == 0):
  print ARTGetLastErrorMessage();
# please specify
if (ARTAddLocalParamToTModule(lipModule, "a1", "a1 = 0.0001") == 0):
  print ARTGetLastErrorMessage();
# please specify
if (ARTAddLocalParamToTModule(lipModule, "a2", "a2 = -0.0001") == 0):
  print ARTGetLastErrorMessage();
# please specify
if (ARTAddLocalParamToTModule(lipModule, "a3", "a3 = -0.0001") == 0):
  print ARTGetLastErrorMessage();
# please specify
if (ARTAddLocalParamToTModule(lipModule, "b1", "b1 = 0") == 0):
  print ARTGetLastErrorMessage();
# initial impedance, please specify
if (ARTAddLocalParamToTModule(lipModule, "Z_0", "Z_0 = 1E4") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddLocalParamToTModule(lipModule, "S", "S = 7.62E-5") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddLocalParamToTModule(lipModule, "y_c", "y_c = 2.4E-4") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddLocalParamToTModule(lipModule, "y_m", "y_c = 4E-4") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddLocalParamToTModule(lipModule, "lambda", "lambda = 0.013") == 0):
  print ARTGetLastErrorMessage();


# set global sampling frequency
#if (ARTSetParameter(sim, "T = 1/12000") == None):
#  print ARTGetLastErrorMessage();


# get output port, change to whatever you want from the lip module
outPort_p = ARTGetPortFromTModule(lipModule, "y");

# initialize all modules to increase buffer size
if (ARTSetParameter(sim, "lipModule.y[-1] = 0.0002; lipModule.y[-2] = 0.0002") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.p[-1] = 0") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.delta_p[-1] = 0") == None):
  print ARTGetLastErrorMessage();


for i in range(0, 50):
  outVal = ARTGetComplexFromPort(outPort_p, i);
  error = ARTGetLastErrorMessage();
  if (error != ""):
    print error;
    break;
  print "{0} {1}".format(i, outVal.re, outVal.im);

ARTRootDestroy();


