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

#ifndef ARTCIRCUIT_H
#define ARTCIRCUIT_H

#include <vector>

#include "Element.h"
#include "Simulator.h"
#include "WaveObject.h"
namespace ART{
  /**
   * This class represents a wind instrument, that is built from several Element:s
   * which are put together in a certain order. Circuit does not have a direct connection
   * to a hornelement, so prototype_ is NULL, its default value.
   */
  class Circuit : public Element {
  private:
    vector<Element*> references;
    DataContainer* impedanceCurve_; //Input impedance as triple vector (f, re, im)
    ART::Simulator* simulator_;
    WaveObjectInterface* wavefrontRadiation; //Wave object representing the radiation impedance of the circuit. This object is constructed in prepareCalculation

  public:
    Circuit(ART::Simulator* simulator, const string name, const string sds="", const string lds="", const string htm="");

    virtual void Rename(const string newname)
    {
      model->SetName(newname);
      SetName(newname);
    }

    int GetElementPosition(Element* el);
    void AppendElement(Element* element);
    void AppendElementBefore(Element* before, Element* element);
    void AppendElementAfter(Element* after, Element* element);

    ///deletes element from circuits element list, when the element itself is destroyed. This will leave a NULL-pointer in the vector, so the user knows something is missing. Always remove elements before destroying them!
    int DeleteElement(Element* element);

    ///removes element from circuits element list
    int RemoveElement(Element* element);
    int ReplaceElement(Element* search, Element* replace);
    int RemoveAllElements();

    /// propagated impedance using functionoids
    virtual void PrepareCalculation();

    virtual void RadiationImpedance(WaveObjectInterface*& out);
    virtual void InputImpedance(WaveObjectInterface* in, WaveObjectInterface*& out);
    virtual bool HasBends();
    virtual ~Circuit ()
      {
	//destroy branchmodel
	delete model;
	model = NULL;
      }
  };
}
#endif /* ARTCIRCUIT_H */
