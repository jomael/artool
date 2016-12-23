#!/usr/bin/env python

"""Test the ARTool library with Python."""

from sys import path,platform
import logging
if platform.startswith("win"):
  path.append("..\..")
elif platform.startswith("linux"):
  path.append("../..")

from artsimdefs import *
from math import sqrt

# create logger with 'spam_application'
logger = logging.getLogger('ARToolLogger')
logger.setLevel(logging.DEBUG)
# create console handler with a higher log level
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
# create formatter and add it to the handlers
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
ch.setFormatter(formatter)
# add the handlers to the logger
logger.addHandler(ch)

# init simulation
logger.info('Creating root object...')
pSim = ARTRootObject()
logger.info('Root object created. %s',ARTGetLastErrorMessage())
logger.info('Creating simulator...')
mySim = ARTCreateSimulator("APITestSimulator", "FrequencyDomain", "MultiModal")
logger.info('TestSimulator created. %s',ARTGetLastErrorMessage())
logger.info('Getting properties...')
prop = ARTGetDataProperties(pSim,None)
propVal = ARTGetValue(prop)
logger.error('Error?: %s', ARTGetLastErrorMessage())
logger.info('Initial prop: %s, Value: %s', ARTGetName(prop), ARTGetString(propVal,0))

while prop:
  prop = ARTGetDataProperties(pSim,prop)
  propVal = ARTGetValue(prop)
  if prop and propVal:
    logger.info('Next property: %s, Value %s',
                ARTGetName(prop), ARTGetString(propVal,0))
  logger.info('\tErrors? %s',ARTGetLastErrorMessage())
