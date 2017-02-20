/**************************************************************************
*                                                                         *
*                   Acoustic Research Tool (ART)                          *
*                                                                         *
*   A Simulation Framework with Modelling Library for Acoustic Systems    *
*                                                                         *
*         Project of the Workgroup 2 of the Technical Committee           *
*      Musical Acoustics of the European Acoustics Association EAA)       *
*                                                                         *
*   http://www.eaa-fenestra.org/technical-committees/ma/workgroups/wg2    *
*                                                                         *
*  Copyright (C) 2013 by the authors and their organisations:             *
*    Alistair Braden            1)                                        *
*    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
*    Delphine Cadefaux          2)                                        *
*    Vasileios Chatziioannou    2)                                        *
*    Sadjad Siddiq              2)                                        *
*    Clemens Geyer              2)                                        *
*                                                                         *
*    1) School of Physics and Astronomy, Univ. of Edinburgh, GB           *
*       (http://www.ph.ed.ac.uk/)                                         *
*    2) Inst. of Music Acoustics, Univ. of Music, Vienna, AT              *
*       (http://iwk.mdw.ac.at)                                            *
*                                                                         *
*  This program is free software: you can redistribute it and/or modify   *
*  it under the terms of the GNU General Public License as published by   *
*  the Free Software Foundation, either version 3 of the License, or      *
*  any later version.                                                     *
*                                                                         *
*  This program is distributed in the hope that it will be useful,        *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of         *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
*  GNU General Public License for more details.                           *
*                                                                         *
*  You should have received a copy of the GNU General Public License      *
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
*                                                                         *
***************************************************************************/

#include "AcousticResearchTool.h"

#include "CalculationMethod.h"
#include "DataProp.h"
#include "Model.h"
#include "TimeModule.h"
#include "timePrototypes.h"
#include "ARTlib.h"
using namespace ART;
AcousticResearchTool::AcousticResearchTool() : ARTobject("ART","Acoustic Research Tool","","ART.htm")
{
//!### Developers: Update this text to reflect most recent changes !
        longDesc_  = "ART is a general purpose simulation framework and includes a growing \n"
                     "\tlibrary of acoustic modelling elements. Currently only frequency domain\n"
                     "\tmodelling is supported, work on time domain simulation is in progress.\n"
                     "\tSupported wave types are PlaneWave, SphericalWave and MultiModalWave. \n"
                     "\tAvailable elements are BoreDiscontinuity, CylindricalTube, ConicalTube,\n"
                     "\tBesselHorn, CylindricalTubeBend and ConicalTubeBend";

// -------------------------------------------------------------------------------------------------------------
// Top level properties:
        DataProp* authors = new DataProp(C_ART_nstr, 6, "Authors");
        ARTSetString(authors,0,"Wilfried Kausel");
        ARTSetString(authors,1,"Alistair Braden");
        ARTSetString(authors,2,"Delphine Cadefaux");
        ARTSetString(authors,3,"Vasileios Chatziioannou");
        ARTSetString(authors,4,"Sadjad Siddiq");
        ARTSetString(authors,5,"Clemens Geyer");
        AppendDataProp(authors);

        AppendDataProp("Copyright","This program is licenced according to GPL-Library ()");
        // modified by CBG: support time domain
        AppendDataProp("SimulationDomain", new ARTvariant("FrequencyDomain", "TimeDomain"));
        AppendDataProp("WaveType",new ARTvariant("SphericalWave", "PlaneWave" ,"MultiModal"));
        AppendDataProp("LosslessModel",1);

// -------------------------------------------------------------------------------------------------------------
// Top level methods:

        CalculationMethod*
        met = AppendMethod("ExecuteScript","Read text file with script commands and execute them");
        met->AppendDataProp("FileName", "");

        met = AppendMethod("ReadBoreList","Read standard x,y instrument definition");
        met->AppendDataProp("FileName", "");

// -------------------------------------------------------------------------------------------------------------
// Top level listprops:
        menuGroups = AppendListProp("MenuCommands");

        ARTobject*
        obj =menuGroups->AppendObject("FileMenu");
        met =obj->AppendMethod("SaveImpedanceList");
        met->AppendDataProp("FileName","");
        met =obj->AppendMethod("ExecuteScript");
        met->AppendDataProp("FileName","");
        met =obj->AppendMethod("ReadBoreList");
        met->AppendDataProp("FileName","");

        //this is not used right now...
/*
        obj =menuGroups->AppendObject("Simulation");
        met =obj->AppendMethod("CalculateImpedanceGrid");
        met->AppendDataProp("LowerFrequencyLimit", 50.0); //0.0
        met->AppendDataProp("HigherFrequencyLimit", 90.0); //1800.0
        met->AppendDataProp("FrequencyStep", 5.0);
        met->AppendDataProp("NumberOfModes", 1);
        met =obj->AppendMethod("RefineResonances");
        met->AppendDataProp("AccuracyTarget", 0.001);

        obj =menuGroups->AppendObject("Optimisation");
        met =obj->AppendMethod("EditParameters");
        met->AppendDataProp("Parameter1", 0.0);
        met->AppendDataProp("Parameter2", 0.0);
        met->AppendDataProp("Parameter3", 0.0);
        met->AppendDataProp("Parameter4", 0.0);
        met =obj->AppendMethod("ReadTemplate");
        met->AppendDataProp("FileName", "");
        met =obj->AppendMethod("RunRosenbrock");
        met->AppendDataProp("Iterations", 3);
        met =obj->AppendMethod("RunGenetic");
        met->AppendDataProp("Generations", 99);

        ListProp*
        sublst =obj->AppendListProp("SettingsSubmenu");
        obj = sublst->AppendObject("Entry1");
        met =obj->AppendMethod("Command1");
        met->AppendDataProp("Parm1", 0.0);
        met->AppendDataProp("Parm2", 0.0);
        met =obj->AppendMethod("Command2");
        met->AppendDataProp("Parm3", 0.0);
        met->AppendDataProp("Parm4", 0.0);
        obj = sublst->AppendObject("Entry2");
        met =obj->AppendMethod("Command1");
        met->AppendDataProp("Parm5", 0.0);
        met =obj->AppendMethod("Command2");
        met->AppendDataProp("Parm6", 0.0);
        obj = sublst->AppendObject("Entry3");
        met =obj->AppendMethod("Command1");
        met->AppendDataProp("Parm7", 0.0);
        met =obj->AppendMethod("Command2");
        met->AppendDataProp("Parm8", 0.0);
        */
        //Add a list to the object. Call it "prototypeModels" and keep the pointer to it in prototypeModels
        prototypeModels = AppendListProp("prototypeModels");
        //Add an object to the list and keep its reference obj...
        obj = prototypeModels->AppendObject(new HornElementPrototype_FD<BoreJumpSection>("BoreJump", "sound propagation in a cylindrical duct"));
        //...to add properties of the object
        obj->AppendDataProp("r1", 1.0,"left side radius [cm]");
        obj->AppendDataProp("r2", 2.0,"right side radius [cm]");
        obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
        obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
        obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
        obj->AppendProperty("canPropagate","this element can propagate impedance");
        obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated");
        obj->AppendMethod("SphericalWave", "actually nothing to compute here");
        obj->AppendMethod("PlaneWave", "actually nothing to compute here");
        obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Kemp02)");
        obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

        obj = prototypeModels->AppendObject(new HornElementPrototype_FD<CylinderSection>("Cylinder", "sound propagation in a cylindrical duct"));
        obj->AppendDataProp("length", 100.0,"axial length [cm]");
        obj->AppendDataProp("r", 1.0,"duct radius [cm]");
        obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
        obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
        obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
        obj->AppendDataProp("xc", 0.000382,"CO2 (molar fraction)");
        obj->AppendProperty("canPropagate","this element can propagate impedance");
        obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated");
        obj->AppendMethod("SphericalWave", "MapesRiordan Model");
        obj->AppendMethod("PlaneWave", "first mode only");
        obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Kemp02)");
        obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

        obj = prototypeModels->AppendObject(new HornElementPrototype_FD<CylinderBendSection>("BentCylinder", "sound propagation in a cylindrical duct"));
        obj->AppendDataProp("length", 100.0,"axial length [cm]");
        obj->AppendDataProp("r", 1.0,"duct radius [cm]");
        obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
        obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
        obj->AppendDataProp("bendradius", 50.0,"bend radius");
        obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
        obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
        obj->AppendProperty("bent","marks this object as bent"); //only add this prop if element is bent!!!
        obj->AppendProperty("canPropagate","this element can propagate impedance");
        obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated");
        obj->AppendMethod("SphericalWave", "MapesRiordan Model");
        obj->AppendMethod("PlaneWave", "first mode only");
        obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Kemp02)");
        obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

        obj = prototypeModels->AppendObject(new HornElementPrototype_FD<ConeSection>("Cone", "sound propagation in a cylindrical duct"));
        obj->AppendDataProp("length", 50.0,"axial length [cm]");
        obj->AppendDataProp("r1", 1.0,"left side radius [cm]");
        obj->AppendDataProp("r2", 5.0,"right side radius [cm]");
        obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
        obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
        obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
        obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
        obj->AppendProperty("canPropagate","this element can propagate impedance");
        obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated");
        obj->AppendMethod("SphericalWave", "MapesRiordan Model");
        obj->AppendMethod("PlaneWave", "first mode only");
        obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Braden06)");
        obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

        obj = prototypeModels->AppendObject(new HornElementPrototype_FD<ConeBendSection>("BentCone", "sound propagation in a cylindrical duct"));
        obj->AppendDataProp("length", 50.0,"axial length [cm]");
        obj->AppendDataProp("r1", 1.0,"left side radius [cm]");
        obj->AppendDataProp("r2", 5.0,"right side radius [cm]");
        obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
        obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
        obj->AppendDataProp("bendradius", 10.0,"bend radius");
        obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
        obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
        obj->AppendProperty("canPropagate","this element can propagate impedance");
        obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated");
        obj->AppendProperty("bent","marks this object as bent"); //only add this prop if element is bent!!!
        obj->AppendMethod("SphericalWave", "MapesRiordan Model");
        obj->AppendMethod("PlaneWave", "first mode only");
        obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Braden06)");
        obj->AppendMethod("FrequencyDomain", "simulation in frq domain");


        obj = prototypeModels->AppendObject(new HornElementPrototype_FD<BesselSection>("Besselhorn", "sound propagation in a cylindrical duct"));
        obj->AppendDataProp("length", 50.0,"axial length [cm]");
        obj->AppendDataProp("r1", 1.0,"left side radius [cm]");
        obj->AppendDataProp("r2", 5.0,"right side radius [cm]");
        obj->AppendDataProp("flare", 0.4,"flare parameter");
        obj->AppendDataProp("temp", 21.0,"local temperature [degC]");
        obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
        obj->AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
        obj->AppendDataProp("xc", 0.000382,"CO2 molar fraction");
        obj->AppendProperty("canPropagate","this element can propagate impedance");
        obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated");
        obj->AppendMethod("SphericalWave", "MapesRiordan Model");
        obj->AppendMethod("PlaneWave", "first mode only");
        obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Braden06)");
        obj->AppendMethod("FrequencyDomain", "simulation in frq domain");

        //Append custom prototypes. All properties are set in the constructors
        obj = prototypeModels->AppendObject(new ToneHole());
        obj = prototypeModels->AppendObject(new TerminationModel());

        // The branch is not a model, it is used by Circuit objects, when other Circuits are inserted into the instrument.
        //obj = prototypeModels->AppendObject(new BranchModel());

        // register prototypes of time modules
        obj = prototypeModels->AppendObject(new TimeModule());
        obj = prototypeModels->AppendObject(new impulseModule());
        obj = prototypeModels->AppendObject(new heavisideModule());
        obj = prototypeModels->AppendObject(new rectangularModule());
        obj = prototypeModels->AppendObject(new amplificationModule());
        obj = prototypeModels->AppendObject(new simpleDelayModule());
        obj = prototypeModels->AppendObject(new addModule());
        obj = prototypeModels->AppendObject(new multiplicationModule());
        obj = prototypeModels->AppendObject(new sinewaveModule());
        obj = prototypeModels->AppendObject(new fractionalDelayModule());
        obj = prototypeModels->AppendObject(new inputFunctionModule());
        obj = prototypeModels->AppendObject(new DWGcylinderModule());
        obj = prototypeModels->AppendObject(new DWGcylinderJunctionModule());
        obj = prototypeModels->AppendObject(new DWGconeModule());
        obj = prototypeModels->AppendObject(new DWGconeJunctionModule());

        //the following list has to be filled by the user
        simulators = AppendListProp("Simulators");

}

//--------------------------------------------------------------------------------------------------------------

AcousticResearchTool::~AcousticResearchTool() {}

//Property* AcousticResearchTool::FindProperty(const string nam)
//{
//      Property* prop = NULL;
////    ARTsimulator* sim = dynamic_cast<ARTsimulator*>(simulators->GetObjects(NULL));
//
//      std::cout << "BLABLA" << std::endl;
//      return ARTobject::FindProperty(nam);
//
//      // if we only have 1 simulator
//      if (simulators->Size() == 1)
//      {
//            // special treatment for this case?
//      }
//
//      while (sim)
//      {
//              try
//              {
//                      sim->FindDataPropInSimulator(nam);
//              }
//              catch(...)
//              {
//                      // if any exception happened, we did not find any property with
//                      // the given name
//                      prop = NULL;
//              }
//              sim = dynamic_cast<ARTsimulator*>(simulators->GetObjects(sim));
//      }
//      return prop;
//}
