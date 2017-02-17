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

#ifndef ACOUSTICRESEARCHTOOL_H
#define ACOUSTICRESEARCHTOOL_H

#include <list>

#include "ARTobject.h"
#include "DataContainer.h"
#include "ListProp.h"
#include "HornElement.h"
#include "ImpedanceCurve.h"

/**
 * \defgroup ARTimpl ART implementation
 * \addtogroup ARTimpl ART implementation
 * The ART interface uses the classes in this module to simulate instruments. If you wish
 * to write your own application for the simulation of wind instruments, refer to \ref
 * sp_API. If you wish to extend the functionality of the interface, have a look at
 * the following classes.
 *
 * @{
 */

/**
 * The acoustic research tool contains all simulators, prototypes and elements, as well as
 * menu commands.
 */
class AcousticResearchTool : public ART::ARTobject {
 protected:
 public:
  ART::ListProp* menuGroups; ///<shortcut pointers inside list
  ART::ListProp* prototypeModels;///<prototype models
  ART::ListProp* simulators;

  AcousticResearchTool();
  virtual ~AcousticResearchTool();

  //      virtual Property* FindProperty(const string nam);

  void ReplaceProgressFunction(TprogressFunction f)
  {
    ART::DataContainer::progressIndicator.SetProgressFunction(f);
  }

  list<ART::HornElement*>::iterator iter;
  ImpedanceCurve impedanceCurve;
  int *paramRefs_;
};

/** @}
 * End of documentation group ART implementation
 */

#endif /* ACOUSTICRESEARCHTOOL_H */
