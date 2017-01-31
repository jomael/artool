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

#ifndef ARTMODELINTERFACE_H
#define ARTMODELINTERFACE_H

//#include "ARTwaveObject.h"
//#include "ARTsimulator.h"
#include <vector>
#include "ARTobject.h"
#include "ARTerror.h"
#include "FreqSimulator.h"
#include "WaveObject.h"
using std::string;
// forward declarations
namespace ART{
  class Circuit;
}
//prototype: a blueprint of a model (same class), but without simulator (NULL).
//model: a copy of a prototype used by an element
//element: a part of an instrument, simulated using a model

//in implementierung sind model und element objekte der gleichen klasse
//objekte in array prototypes sind prototypen, die sich clonen k�nnen, wenn man ein element des
//prototyps haben will. Gibt es bei diesem Ansatz ein Problem??
//--> ja, ein prototyp wird nie wie ein element eingesetzt; wenn benutzer einen prototyp schreibt sollte er sich
//gar nicht darum knmmern mnssen, wie der prototyp eingesetzt wird. Elemente setzten dann ihre kopie vom
//prototyp ein.

/** \addtogroup ModelInterfaceGroup ModelInterface
 * @{
 */
namespace ART{
/**
 * This is the common interface for acoustic models. Models are used by elements of a 
 * circuit to calculate acoustic properties like impedance. You can create custom models 
 * by implementing this interface.
 */ 
class ModelInterface : public ARTobject
{
	public:
	  /**
	   * Use this constructor to fill the fields of super classes and initialize other fields.
	   */     	  
		ModelInterface(const string name, const string sds, const string lds, const string htm)
		 : ARTobject(name,sds,lds,htm), circuit(NULL), simulator(NULL) {}; 

		/**
		 * The copy constructor.
		 */		 
		ModelInterface(const ModelInterface& orig)
		 : ARTobject(orig) 
		{
			this->simulator = orig.simulator;
			this->circuit = orig.circuit;
		};

    /**
     * This function should prepare the calculation for the radiation impedance of the 
     * model. 
     * @param out A reference to a pointer pointing to the waveobject the function should 
     *    use as output. If the value of the pointer is NULL, the function must allocate
     *    a waveobject to use as output and update the pointer.
     */         
  virtual void RadiationImpedance(ART::WaveObjectInterface*& out) = 0;

    /**
     * This function should prepare the calculation of the input impedance of the 
     * model by propagating the wave specified in the waveobject in.
     * @param in The wave to be propagated.      
     * @param out A reference to a pointer pointing to the waveobject the function should 
     *    use as output. If the value of the pointer is NULL, the function must allocate
     *    a waveobject to use as output and update the pointer.
     */         
  virtual void InputImpedance(ART::WaveObjectInterface* in, ART::WaveObjectInterface*& out) = 0;

    /**
     * This function should prepare the calculation of the pressure matrix of the 
     * model by propagating the wave specified in the waveobject in. 
     * @param in The wave to be propagated.      
     * @param out A reference to a pointer pointing to the waveobject the function should 
     *    use as output. If the value of the pointer is NULL, the function must allocate
     *    a waveobject to use as output and update the pointer.
     */         
  virtual void Pressure(ART::WaveObjectInterface* in, ART::WaveObjectInterface*& out) = 0;

		//more than one wave object is passed to branches: These functions are implemented in the branch
  virtual void InputImpedance(vector<ART::WaveObjectInterface*>& in, ART::WaveObjectInterface*& oSignal) 
		{
			throw ARTerror("ModelInterface::getRadiationImpedance(vector<ART::WaveObjectInterface*>&)","This method is not implemented in the object '%s1'.", GetName()); 
		};

  virtual void Pressure(vector<ART::WaveObjectInterface*>& in, ART::WaveObjectInterface*& oSignal)
		{
			throw ARTerror("ModelInterface::getPressure(vector<ART::WaveObjectInterface*>&)","This method is not implemented in the object '%s1'.", GetName()); 
		};

		/**
		 *  A function creating exact copies must be provied.		
		 */
		virtual ModelInterface* CloneModel() = 0;

		//... (add methods for other calculations) ...

		/**
		 *  Sets the simulator of the model to sim. The simulator must be set before evaluation.
		 *  Prototypes need to write formulas for impedance calculation and must therefore have access to some 
		 *  properties of the simulator and circuit (which will in turn give access to some properties of the simulator)
		 */     		
		void SetSimulator(Simulator* sim);

		/**
		 *  Sets the circuit of the model to cir. The circuit must be set to the circuit which should be evaluated before evaluation.
		 *  Prototypes need to write formulas for impedance calculation and must therefore have access to some 
		 *  properties of the simulator and circuit (which will in turn give access to some properties of the simulator)
		 */     		
		void SetCircuit(ART::Circuit* cir) {circuit = cir;};

	protected:
		/**
		 * This field must contain a pointer to the circuit which is being evaluated when.		
		 * Prototypes need to write formulas for impedance calculation and must therefore have access to some 
		 * properties of the simulator and circuit (which will in turn give access to some properties of the simulator)
		 */     
		ART::Circuit* circuit; 

		/**
		 * This field must contain a pointer to the simulator the element was created in.		
		 * Prototypes need to write formulas for impedance calculation and must therefore have access to some 
		 * properties of the simulator and circuit (which will in turn give access to some properties of the simulator)
		 */     
		FreqSimulator* simulator;
};
}
/** @}
 * End of documentation group ModelInterface
 */

#endif /* ARTMODELINTERFACE_H */
