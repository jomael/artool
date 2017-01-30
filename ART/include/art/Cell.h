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
 *  Copyright (C) 2011 by the authors and their organisations:             *
 *    Alistair Braden            1)                                        *
 *    Wilfried Kausel            2)         kausel(at)mdw.ac.at            *
 *    Delphine Cadefaux          2)                                        *
 *    Vasileios Chatziioannou    2)                                        *
 *    Sadjad Siddiq              2)                                        *
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

#ifndef ARTCELL_H
#define ARTCELL_H

#include <stdlib.h>
#include <list>
#include <string>

using std::string;
namespace ART{
  /**
   * The base class for all classes containing name, short and long description of an object
   * and HTML explanation.
   */
  class Cell {
  private:
  protected:
    string name_;           /**< Identifier of ARTObject */
    string shortDesc_;      /**< Short Description of ARTObject (single line hint) */
    string longDesc_;       /**< Long Description of ARTObject (multi line text) */
    string helpFile_;       /**< File name of HTML help file for ARTObject */
  public:
  Cell(const string name="", const string sds="", const string lds="", const string htm="") : name_(name), shortDesc_(sds), longDesc_(lds), helpFile_(htm) {}
    Cell(const Cell& orig) /**< copy constructor */
      {
	name_ = orig.name_;
	shortDesc_ = orig.shortDesc_;
	longDesc_ = orig.longDesc_;
	helpFile_ = orig.helpFile_;
      }

    virtual ~Cell() {}
    virtual Cell* clone() {return new Cell(*this);}

    virtual const string& GetName() const {return name_;}
    virtual const string& GetShortDescription() const {return shortDesc_;}
    virtual const string& GetLongDescription() const {return longDesc_;}
    virtual const string& GetHelpFilename() const {return helpFile_;}

    virtual void SetName(const string name) {name_ = name;}

  };
}
#endif /* ARTCELL_H */
