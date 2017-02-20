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


#ifndef ARTFREQSIMULATOR_H_
#define ARTFREQSIMULATOR_H_

//#include <list>
#include <map>
#include "mpParser.h"
#include "ARTobject.h"
#include "Property.h"
#include "DataProp.h"
#include "Simulator.h"

using namespace mup;
//using std::list;

/**
 * @brief Implementation of a simulator for the frequency domain.
 */
namespace ART{
  // forward declaration
  class ITimeModule;
  class FreqSimulator : public Simulator
{
protected:
  /**
   * Defines the wave type of the simulation - plain, spherical or multimodal.
   */
  ART::Property wavetype_;
  ART::DataContainer* frqGrid;
  ART::DataContainer* wfrqGrid;
  ART::DataContainer* modes;
public:

  FreqSimulator(const string name, const string wavetype="MultiModal",
      const string sds="", const string lds="", const string htm="");

  //void SetMultimodeParameters(ARTdataProp* fmin, ARTdataProp* fmax, ARTdataProp* fstep, ARTdataProp* modes);

  virtual ART::DataContainer* GetFrequencyGrid() {return frqGrid;}
  virtual ART::DataContainer* GetAngularFrequencyGrid() {return wfrqGrid;}
  virtual ART::DataContainer* GetNumberOfModes() {return modes;}

  virtual ART::Property* GetWavetype() {return &wavetype_;}

  virtual ~FreqSimulator() {}
};
}

#endif /* ARTFREQSIMULATOR_H_ */
