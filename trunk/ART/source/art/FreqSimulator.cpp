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

#include <sstream>
#include "strparsing.h"
#include "FreqSimulator.h"
#include "ListProp.h"
#include "TimeModule.h"
#include "Functionoid.h"

using namespace mup;

using std::stringstream;
using namespace ART;


//**************************************************************************************************************
// ARTfreqSimulator

FreqSimulator::FreqSimulator(const string name, const string wavetype,
    const string sds, const string lds, const string htm) :
    Simulator(name, "FrequencyDomain", sds, lds, htm), wavetype_(wavetype)
{

  /*
	The frequency grids are not supposed to be edited by the user and therefore no properties!
	frqGrid = AppendDataProp("frqGrid", new ARTvariant(C_ART_ndbl), "The list of frequencies (in Hz) for which this simulator will calculate the impedance.");
	wfrqGrid = AppendDataProp("wfrqGrid", new ARTvariant(C_ART_ndbl), "The list of frequencies (angular frequency) for which this simulator will calcualte the impedance.");
   */
  modes = AppendDataProp("NumberOfModes", 1, "The number of modes for which this simulator will calculate the impedance.");

  DataProp* fmin = AppendDataProp("LowerFrequencyLimit", 50.0, "The lower frequency (in Hz) of the range for which this simulator will calculate the impedance.");
  DataProp* fmax = AppendDataProp("HigherFrequencyLimit", 1800.0, "The higher frequency (in Hz) of the range for which this simulator will calculate the impedance.");
  DataProp* fstep = AppendDataProp("FrequencyStep", 5.0, "The frequency step (in Hz) used to go through the range for which this simulator will calculate the impedance.");

  //add properties to parser
  Property* prop = GetProperties(NULL);
  while (prop)
  {
    //if it is a data property
    DataProp* dprop = dynamic_cast<DataProp*>(prop);
    if (dprop)
    {
      string varname = dprop->GetName();
      dprop->SetParser(parser_);
      dprop->SetParserVar(varname);
      //std::cout << "Created Parser Var: " << varname << "\n";
    }
    prop = GetProperties(prop);
  }

  frqGrid = new DataContainer("frqGrid", new ARTfrqGridFunc(fmin, fmax, fstep));
  wfrqGrid = new DataContainer("wfrqGrid", new ARTwfrqGridFunc(frqGrid));

}
