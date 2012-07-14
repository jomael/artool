#ifndef ARTMODELINTERFACE_H
#define ARTMODELINTERFACE_H

#include "ARTwaveObject.h"
//#include "ARTsimulator.h"

// forward declaration
class ARTfreqSimulator;

//prototype: a blueprint of a model (same class), but without simulator (NULL).
//model: a copy of a prototype used by an element
//element: a part of an instrument, simulated using a model

//in implementierung sind model und element objekte der gleichen klasse
//objekte in array prototypes sind prototypen, die sich clonen können, wenn man ein element des
//prototyps haben will. Gibt es bei diesem Ansatz ein Problem??
//--> ja, ein prototyp wird nie wie ein element eingesetzt; wenn benutzer einen prototyp schreibt sollte er sich
//gar nicht darum knmmern mnssen, wie der prototyp eingesetzt wird. Elemente setzten dann ihre kopie vom
//prototyp ein.

/** \addtogroup ARTmodelInterfaceGroup ARTmodelInterface
 * @{
 */

/**
 * This is the common interface for acoustic models. Models are used by elements of a 
 * circuit to calculate acoustic properties like impedance. You can create custom models 
 * by implementing this interface.
 */ 
class ARTmodelInterface : public ARTobject
{
	public:
	  /**
	   * Use this constructor to fill the fields of super classes and initialize other fields.
	   */     	  
		ARTmodelInterface(const string name, const string sds, const string lds, const string htm)
		 : ARTobject(name,sds,lds,htm), circuit(NULL), simulator(NULL) {}; 

		/**
		 * The copy constructor.
		 */		 
		ARTmodelInterface(const ARTmodelInterface& orig)
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
		virtual void RadiationImpedance(WaveObjectInterface*& out) = 0;

    /**
     * This function should prepare the calculation of the input impedance of the 
     * model by propagating the wave specified in the waveobject in.
     * @param in The wave to be propagated.      
     * @param out A reference to a pointer pointing to the waveobject the function should 
     *    use as output. If the value of the pointer is NULL, the function must allocate
     *    a waveobject to use as output and update the pointer.
     */         
		virtual void InputImpedance(WaveObjectInterface* in, WaveObjectInterface*& out) = 0;

    /**
     * This function should prepare the calculation of the pressure matrix of the 
     * model by propagating the wave specified in the waveobject in. 
     * @param in The wave to be propagated.      
     * @param out A reference to a pointer pointing to the waveobject the function should 
     *    use as output. If the value of the pointer is NULL, the function must allocate
     *    a waveobject to use as output and update the pointer.
     */         
		virtual void Pressure(WaveObjectInterface* in, WaveObjectInterface*& out) = 0;

		//more than one wave object is passed to branches: These functions are implemented in the branch
		virtual void InputImpedance(vector<WaveObjectInterface*>& in, WaveObjectInterface*& oSignal) 
		{
			throw ARTerror("ARTmodelInterface::getRadiationImpedance(vector<WaveObjectInterface*>&)","This method is not implemented in the object '%s1'.", GetName()); 
		};

		virtual void Pressure(vector<WaveObjectInterface*>& in, WaveObjectInterface*& oSignal)
		{
			throw ARTerror("ARTmodelInterface::getPressure(vector<WaveObjectInterface*>&)","This method is not implemented in the object '%s1'.", GetName()); 
		};

		/**
		 *  A function creating exact copies must be provied.		
		 */
		virtual ARTmodelInterface* CloneModel() = 0;

		//... (add methods for other calculations) ...

		/**
		 *  Sets the simulator of the model to sim. The simulator must be set before evaluation.
		 *  Prototypes need to write formulas for impedance calculation and must therefore have access to some 
		 *  properties of the simulator and circuit (which will in turn give access to some properties of the simulator)
		 */     		
		void SetSimulator(ARTsimulator* sim);

		/**
		 *  Sets the circuit of the model to cir. The circuit must be set to the circuit which should be evaluated before evaluation.
		 *  Prototypes need to write formulas for impedance calculation and must therefore have access to some 
		 *  properties of the simulator and circuit (which will in turn give access to some properties of the simulator)
		 */     		
		void SetCircuit(ARTcircuit* cir) {circuit = cir;};

	protected:
		/**
		 * This field must contain a pointer to the circuit which is being evaluated when.		
		 * Prototypes need to write formulas for impedance calculation and must therefore have access to some 
		 * properties of the simulator and circuit (which will in turn give access to some properties of the simulator)
		 */     
		ARTcircuit* circuit; 

		/**
		 * This field must contain a pointer to the simulator the element was created in.		
		 * Prototypes need to write formulas for impedance calculation and must therefore have access to some 
		 * properties of the simulator and circuit (which will in turn give access to some properties of the simulator)
		 */     
		ARTfreqSimulator* simulator;
};
/** @}
 * End of documentation group ARTmodelInterface
 */

#endif
