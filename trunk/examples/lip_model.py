#!/usr/bin/env python

"""Test libartsim library with Python."""

from artsimdefs import *;
from math import exp;
from pylab import *
from time import clock;

startTime = clock();

# init simulation
pSim = ARTRootObject();

# create simulator
sim = ARTCreateSimulator("TimeSimulator", "TimeDomain", "");

# create function module
impulseResponse = ARTCreateTModule(sim, "InstrumentResponse", "InputFunctionModule");
if (impulseResponse == None):
  print ARTGetLastErrorMessage();

sweepSignal = ARTCreateTModule(sim, "Sweep", "InputFunctionModule");
if (sweepSignal == None):
  print ARTGetLastErrorMessage();

# create time module with convolution
lipModule = ARTCreateTModule(sim, "lipModule", "TimeModule");
if (lipModule == None):
  print ARTGetLastErrorMessage();

# create impulse and gain modules
impulseModule = ARTCreateTModule(sim, "Impulse", "ImpulseModule");
if (impulseModule == None):
  print ARTGetLastErrorMessage();

# init impulse response of instrument by reading external reflection function
# normalized s.t. sum is -1
# normalized to Adachi's amplitude
with open('trumpet_rf.txt', 'r') as f:
  flen = int(f.readline())

# create and resize output port of function module
  if (ARTSetOPortOfFModule(impulseResponse, flen, "") == 0):
    print ARTGetLastErrorMessage();

  for i in range(0,flen):
    value = float(f.readline());
    if (ARTSetParameter(sim, "InstrumentResponse.out[{0}] = {1}".format(i,value)) == None):
      print ARTGetLastErrorMessage();
#    print "InstrumentResponse.out[{0}] = {1}".format(i,value);

# Visileios' method:
if (ARTAddOPortToTModule(lipModule, "x", "x[t] = x[t-1]*(2-sqrt(k/m)*(T/Q)-(k/m)*T^2) + x[t-2]*(sqrt(k/m)*(T/Q)-1) + (b*d*T^2/m)*p_lip[t-1]") == 0):
# Clemens' method (not stable)
#if (ARTAddOPortToTModule(lipModule, "x", "x[t] = (x[t-1]*(2+sqrt(k/m)*(T/Q)) - x[t-2] + ((b*d*(T^2))/m)*p_lip[t-1])/(1+sqrt(k/m)*(T/Q)+(k*T^2)/m)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "S_lip", "S_lip[t] = max(b*(2*x[t] + x_0),1E-5)") == 0):
  print ARTGetLastErrorMessage();

# as defined by equation (6) in Adachi
if (ARTAddOPortToTModule(lipModule, "p", "p[t] = pp[t] + pm[t]") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "pm", "pm[t] = conv(pp,G,t-1)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "pp", "pp[t] = Zc*U[t] + pm[t]") == 0):
  print ARTGetLastErrorMessage();

# as defined by equation (1) in Adachi
if (ARTAddOPortToTModule(lipModule, "p_lip", "p_lip[t] = p_0 - 0.5*rho*(U[t]/S_lip[t])^2-(rho*d/S_lip[t])*(U[t]-U[t-1])/T") == 0):
  print ARTGetLastErrorMessage();

# loop unrolling for newton approximation...
if (ARTAddOPortToTModule(lipModule, "par", "par[t] = 1/(S_cup^2) + 1/(2*(S_lip[t])^2) - 1/(S_cup*S_lip[t])") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U0", "U0[t] = U[t-1] - (rho*par[t]*(U[t-1])^2 + Zc*U[t-1] + 2*pm[t] - p_0)/(2*rho*par[t]*U[t-1] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U1", "U1[t] = U0[t] - (rho*par[t]*(U0[t])^2 + Zc*U0[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U0[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U2", "U2[t] = U1[t] - (rho*par[t]*(U1[t])^2 + Zc*U1[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U1[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U3", "U3[t] = U2[t] - (rho*par[t]*(U2[t])^2 + Zc*U2[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U2[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U4", "U4[t] = U3[t] - (rho*par[t]*(U3[t])^2 + Zc*U3[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U3[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U5", "U5[t] = U4[t] - (rho*par[t]*(U4[t])^2 + Zc*U4[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U4[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U6", "U6[t] = U5[t] - (rho*par[t]*(U5[t])^2 + Zc*U5[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U5[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U7", "U7[t] = U6[t] - (rho*par[t]*(U6[t])^2 + Zc*U6[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U6[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U8", "U8[t] = U7[t] - (rho*par[t]*(U7[t])^2 + Zc*U7[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U7[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U9", "U9[t] = U8[t] - (rho*par[t]*(U8[t])^2 + Zc*U8[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U8[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();

if (ARTAddOPortToTModule(lipModule, "U", "U[t] = U9[t] - (rho*par[t]*(U9[t])^2 + Zc*U9[t] + 2*pm[t] - p_0)/(2*rho*par[t]*U9[t] + Zc)") == 0):
  print ARTGetLastErrorMessage();


# connect output ports of impulseResponse and lip module
if (ARTConnectPorts(sim, "lipModule.G = InstrumentResponse.out") == 0):
  print ARTGetLastErrorMessage();

# add local parameters to lip module
# speed of sound
if (ARTAddLocalParamToTModule(lipModule, "c", "c = 3.4E2") == 0):
  print ARTGetLastErrorMessage();
# average air density
if (ARTAddLocalParamToTModule(lipModule, "rho", "rho = 1.2") == 0):
  print ARTGetLastErrorMessage();
# area of mouthpiece entryway
if (ARTAddLocalParamToTModule(lipModule, "S_cup", "S_cup = 2.17992E-4") == 0):
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
if (ARTAddLocalParamToTModule(lipModule, "f_lip", "f_lip = 353") == 0):
#if (ARTAddLocalParamToTModule(lipModule, "f_lip", "f_lip = 231") == 0):
  print ARTGetLastErrorMessage();
# lip mass define 1.5/(((2*PI)^2)/f_lip
if (ARTAddLocalParamToTModule(lipModule, "m", "m = 1.5/(((2*pi)^2)*(f_lip))") == 0):
  print ARTGetLastErrorMessage();
# stiffness of lips
if (ARTAddLocalParamToTModule(lipModule, "k", "k = 1.5*f_lip") == 0):
  print ARTGetLastErrorMessage();
# blowing pressure
if (ARTAddLocalParamToTModule(lipModule, "p_0", "p_0 = 8E3") == 0):
  print ARTGetLastErrorMessage();
# initial impedance
if (ARTAddLocalParamToTModule(lipModule, "Zc", "Zc = rho*c/S_cup") == 0):
  print ARTGetLastErrorMessage();

# set global sampling frequency
if (ARTSetParameter(sim, "T = 1/35970") == None):
  print ARTGetLastErrorMessage();


# get output port, change to whatever you want from the lip module
outPort_x = ARTGetPortFromTModule(lipModule, "x");
if (outPort_x == None):
  print ARTGetLastErrorMessage();
outPort_slip = ARTGetPortFromTModule(lipModule, "S_lip");
if (outPort_slip == None):
  print ARTGetLastErrorMessage();
outPort_p = ARTGetPortFromTModule(lipModule, "p");
if (outPort_p == None):
  print ARTGetLastErrorMessage();
outPort_plip = ARTGetPortFromTModule(lipModule, "p_lip");
if (outPort_plip == None):
  print ARTGetLastErrorMessage();
outPort_U = ARTGetPortFromTModule(lipModule, "U");
if (outPort_U == None):
  print ARTGetLastErrorMessage();
#outPort_tmp = ARTGetPortFromTModule(lipModule, "tmp");
#if (outPort_tmp == None):
#  print ARTGetLastErrorMessage();

# initialize all modules to increase buffer size
if (ARTSetParameter(sim, "lipModule.x[-1] = 0; lipModule.x[-2] = 0") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.p_lip[-1] = 0; lipModule.p_lip[-2] = 0") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.U[-1] = 0; lipModule.U[-2] = 0") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.p[-1] = 0") == None):
  print ARTGetLastErrorMessage();
if (ARTSetParameter(sim, "lipModule.pp[-1] = 0; lipModule.pm[-1] = 0") == None):
  print ARTGetLastErrorMessage();

#N = 2400
N = 5200

pres = [0 for x in range(N)]
flow = [0 for x in range(N)]

#counter = 0;
#fLip = 231;

simStartTime = clock();

for i in range(0, N):
  pres[i] = ARTGetComplexFromPort(outPort_p, i).re;
  error = ARTGetLastErrorMessage();
  flow[i] = ARTGetComplexFromPort(outPort_U, i).re;
  if (error != ""):
    print error;
    break;
  #print "{0}\r".format(i);
  #if (i >= 35970*0.06 and i <= 35970*0.075):
  #  print "{0} {1}".format(i/35.970, flow[i]);
  #counter = counter + 1;
  #if (counter == 800):
  #  fLip = fLip + 24.4;
  #  ARTSetParameter(sim, "lipModule.f_lip = {0}".format(fLip));
  #  ARTSetParameter(sim, "lipModule.m = 1.5/(((2*pi)^2)*({0}))".format(fLip));
  #  ARTSetParameter(sim, "lipModule.k = 1.5*{0}".format(fLip));
  #  counter = 0;

t = arange(0.0,N/35.970,1/35.970);
subplot(2,1,1);
plot(t,pres);
xlabel('$time [ms]$');
ylabel('$pressure [N/m^2]$');
subplot(2,1,2);
plot(t,flow);
xlabel('$time [ms]$');
ylabel('$flow [m^3/s]$');

show();

endTime = clock();

# comment out if you are not interested in the performance
#print "Total simulation duration: {0} s.\nNet simulation duration: {1} s.".format(endTime - startTime, endTime - simStartTime);

ARTRootDestroy();
