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

#ifndef ARTWAVEOBJECT_H
#define ARTWAVEOBJECT_H

#include "DataContainer.h"
#include "ARTlink.h"
#include "Functionoid.h"

/** \addtogroup waveObjectInterfaceGroup waveObject
 * @{
 */
namespace ART{
/**
 * This is the common interface for all waveobjects passed between elements. 
 * It was introduced to allow models to calculate their acoustic 
 * properties in a variety of different but compatible ways. By passing waves between 
 * elements one element can return its properties in the time domain, while the next 
 * element calculates its properties in the frequency domain. Some elements can return
 * pressure matrices, others impedance matrices. The wave interface is supposed to provide
 * a standardized way of communication between the elements and to convert waves to the 
 * type required by the next element.
 * Wave objects
 * can assume any form, but must specify conversion functions to other forms. If a 
 * conversion is not possible, the waveobject's conversion function should throw an error.
 * @todo: Do we already know all forms a waveobject can assume? If not, maybe we should 
 * not use pure virtual functions but implement functions, which simply throw an exception   
 * here. If some user extends the interface, he would not have to override the new pure 
 * virtual functions in all the old classes as well...       
 */ 
class WaveObjectInterface 
{
	public:
		//Frequenzbereich / frequency domain
		/**
		 * This function must prepare the conversation of the waveobject to a single-mode
		 * impedance matrix.
		 * @param out the data container the output is written to     		 
		 */ 
    //Impedanz, eine eindimensionale Matrix komplexer Zahlen		 
  virtual void ImpedanceMatrix(ART::DataContainer*& out) = 0; 

		/**
		 * This function must prepare the conversion of the waveobject to a multimode 
		 * impedance matrix of dimensions (Number of modes x Number of frequencies).
		 * @param out the data container the output is written to     		 
		 */
    //multimodale Impedanzmatrix, also eine N_m x N_f Matrix komplexer Zahlen 
  virtual void MultiModeImpedanceMatrix(ART::DataContainer*& out) = 0;

		/**
		 * This function must prepare the conversion of the waveobject to a one-
		 * dimensional complex transfer matrix. 
		 * @param out the data container the output is written to     		 
		 */
		//šbertragungsfunktion, eine eindimensionale Matrix komplexer Zahlen
  virtual void TransferMatrix(ART::DataContainer*& out) = 0;

		/**
		 * This function must prepare the conversion of the waveobject to a complex transfer 
		 * matrix  of dimensions (Number of modes x Number of frequencies). 
		 * @param out the data container the output is written to     		 
		 */
		//multimodale _bertragungsfunktion, also eine N_m x N_f Matrix komplexer Zahlen 
  virtual void MultiModeTransferMatrix(ART::DataContainer*& out) = 0; 

		/**
		 * This function must prepare the conversion of the waveobject to a one-dimensional
		 * complex pressure matrix. 
		 * @param out the data container the output is written to     		 
		 */
  virtual void PressureFlowMatrix(ART::DataContainer*& out) = 0;

		/**
		 * This function must prepare the conversion of the waveobject to a 
		 * complex pressure matrix of dimensions modes X modes. 
		 * @param out the data container the output is written to     		 
		 */
  virtual void MultimodePressureFlowMatrix(ART::DataContainer*& out) = 0;

		//Zeitbereich / time domain
		/**
		 * This function must prepare the conversion of the waveobject to an impulse 
		 * representing a pressure wave in the time domain.
		 * @param out the data container the output is written to     		 
		 */
  virtual void PressureImpulse(ART::DataContainer*& out) = 0; 
		//ein Druckimpuls
		
		//Was noch??? / What else?
};

//to be implemented:
//class WaveObjectMMPressureFlow : p

class WaveObjectMMImpedance : public WaveObjectInterface
{
	public:
	/// name of the waveobject (useful for debugging)
  string name_;
	/**
	 * A functionoid to convert a multimode impedance matrix into a single mode impedance 
	 * matrix by dropping all modes except the first mode and to write the modes it into a 
	 * C_ART_ntri data container.
	 */	 
	class ConvertMMImpToImp : public Functionoid //similar to ARTmmInpImpComplexFunc, but so far w/o frequencies in Hz
	{
		public:
		  /**
		   * The constructor to prepare the functionoid's calculation.
		   * @param impMatrix The input multimode impedance matrix.
		   * @param frq The frequency grid giving the frequency in Hz for every index.              		  
		   */		  
	ConvertMMImpToImp(ART::DataContainer* impMatrix, ART::DataContainer* frq)
			: z_matrix_(impMatrix), frequencies(frq)
			{
				if (!frq) throw ARTerror("ConvertMMImpToImp(Constructor)", "Argument '%s1' is NULL.","frq");
				if (!impMatrix) throw ARTerror("ConvertMMImpToImp(Constructor)", "Argument '%s1' is NULL.","impMatrix");
				in_.push_back(impMatrix);
			}

			virtual void ApplyFunction()
			{
				out_->SetType(C_ART_ntri);
				//std::cout << "ConvertMMImpToImp::ApplyFunction()\n";
				out_->SetArrayLength(z_matrix_->len);

				for (int i=0; i<z_matrix_->len; i++) 
				{
					//std::cout << "Umrechnung in DC " << this << "\n";
				  ART::DataContainer::progressIndicator.Continue(out_->GetComplexity(),out_->GetVarName());
					out_->val->nt[i].f = frequencies->val->nd[i]; 
					out_->val->nt[i].re = (*z_matrix_->val->nmx[i])(0,0).real() / 1.0E-5; //convert to Ohms (?)
					out_->val->nt[i].im = (*z_matrix_->val->nmx[i])(0,0).imag() / 1.0E-5;
				}
			}

			virtual bool IsSameFunctionoid(Functionoid* f)
			{
				ConvertMMImpToImp* mf = dynamic_cast<ConvertMMImpToImp*>(f);
				if (!mf) return false;
				if (mf->z_matrix_ != z_matrix_) return false;
				return true;
			}			
			
			virtual int GetIterationNumber() {return frequencies->len;} //z_matrix_->len;

		private:
			ART::DataContainer* z_matrix_;
			ART::DataContainer* frequencies;
	};
	//End conversion functionoid class
	
	/**
	 * A constructor of the multimode impedance waveobject.
	 * @param name The name given to the internal data container.
	 * @param MMimpedanceFunc The functionoid used for calculation the values of this 
	 *    waveobject.     
	 * @param frq The frequency grid giving the frequency for every index.     	
	 */	
 WaveObjectMMImpedance( string name , Functionoid* MMimpedanceFunc = NULL, ART::DataContainer* frq = NULL)
	: name_(name), frequencies(frq)
	{
	  MMimpedance = new ART::DataContainer(name_,MMimpedanceFunc);
	}

	/**
	 * A constructor of the multimode impedance waveobject, where the user can specify a 
	 * data container which will be used as internal data container to store the wave.
	 * @param out The data container that should be used to store the wave.
	 * @param MMimpedanceFunc The functionoid used for calculation the values of this 
	 *        waveobject.
	 * @param frq The frequency grid giving the frequency for every index.
	 */	
 WaveObjectMMImpedance( ART::DataContainer* out, Functionoid* MMimpedanceFunc = NULL, ART::DataContainer* frq = NULL )
	: frequencies(frq)
	{
		if (!out) throw ARTerror("WaveObjectMMImpedance(Constructor)", "Argument '%s1' is NULL.","out");
		MMimpedance = out;
		if (MMimpedanceFunc) MMimpedance->SetFunction(MMimpedanceFunc);
	}

	/**
	 * Sets the calculating functionoid of the waveobject. This function is used to change
	 * the way a waveobject's values are calculated after creation of the waveobject.	 
	 * @param MMimpedanceFunc The functionoid used for calculation the values of this 
	 *    waveobject.
   * @param frq The frequency grid giving the frequency for every index.
	 */
	virtual void SetCalculation(Functionoid* MMimpedanceFunc, ART::DataContainer* frq)
	{
		if (!frq) throw ARTerror("SetCalculation", "Argument '%s1' is NULL.","frq");
		frequencies = frq;
		MMimpedance->SetFunction(MMimpedanceFunc);
	}

	/**
	 * Ensures that the waveobject's multimode impedance matrix is written to data container
	 * out. Since this wave format is the same as the format used internally by this 
	 * waveobject, no conversation needs to take place. If the user specifies a custom data
	 * container as an argument in this function, this request could only be fulfilled if 
	 * the data of the internal data container were copied. This is possible, but it is more
	 * efficient to specify the custom data container when creating the object, so that it 
	 * is used as an internal data container at the same time. So far this was always 
	 * possible.               	 
	 * @param out The data container that should be used to write the multimode impedance 
	 * matrix wave to. If this is NULL the address of the internal data container is written
	 * to the pointer.	 
	 */
	//Frequenzbereich
	virtual void MultiModeImpedanceMatrix(ART::DataContainer*& out )
	{
		if (!MMimpedance) throw ARTerror("MultiModeImpedanceMatrix::ImpedanceMatrix", "MMimpedance is NULL.");
		if (out) //if the user wants to use a custom data container
		{	//This would be straightforward to implement (just copy the contents of the data container), but it would be
			//a waste of ressources. So for the time being an error message will be returned...
			throw ARTerror("WaveObjectMMImpedance::MultiModeImpedanceMatrix", "If the output type of the wave object is the same as the input type, custom output is only supported when specified in the constructor.");
		}
		else //if not, return the data container we are using in this wave object
		{
			out = MMimpedance; //keine Umrechnung n”tig
		}
	}

  /**
   * Converts the multimode impedance matrix stored internally in this waveobject to a
   * single mode impedance by dropping the higher modes. The output format will be 
   * C_ART_ntri and therefore also contain the frequencies.
	 * @param out The data container that should be used to write the multimode impedance 
	 * matrix wave to. If this is NULL a data container will be created.
   */     
	virtual void ImpedanceMatrix(ART::DataContainer*& out )
	{
		if (!frequencies) throw ARTerror("MultiModeImpedanceMatrix::ImpedanceMatrix", "frequencies is NULL.");
		if (!MMimpedance) throw ARTerror("MultiModeImpedanceMatrix::ImpedanceMatrix", "MMimpedance is NULL.");
		//Umrechnung multimodaler Imp. in "einfachmodale" 
		//Auch dies geschieht mittels Funktoren, siehe unten
		//if an output data container is specified, use it -> just set the function
		if (out)
		{
			out->SetFunction(new ConvertMMImpToImp(MMimpedance, frequencies));
		}
		else //if not, create one 
		  out = new ART::DataContainer("ImpedanceMatrix", new ConvertMMImpToImp(MMimpedance, frequencies));
	}

  ///not implemented -> throw ARTerror
	virtual void TransferMatrix(ART::DataContainer*& out ) {throw ARTerror("WaveObjectMMImpedance::TransferMatrix", "The method is not implemented");}
	virtual void MultiModeTransferMatrix(ART::DataContainer*& out ) {throw ARTerror("WaveObjectMMImpedance::MultiModeTransferMatrix", "The method is not implemented");} 
	virtual void PressureFlowMatrix(ART::DataContainer*& out ) {throw ARTerror("WaveObjectMMImpedance::PressureFlowMatrix", "The method is not implemented");}
	virtual void MultimodePressureFlowMatrix(ART::DataContainer*& out)  {throw ARTerror("WaveObjectMMImpedance::MultimodePressureFlowMatrix", "The method is not implemented");}
	virtual void PressureImpulse(ART::DataContainer*& out ) {throw ARTerror("WaveObjectMMImpedance::PressureImpulse", "The method is not implemented");}
	
	//...und andere Umrechenfunktionen / and other conversion functions
	private:
	  /// pointer to the data container used to store the multimode impedance of this waveobject
	ART::DataContainer* MMimpedance;
		/// pointer to the data container specifying the frequency in Hz for every index
	ART::DataContainer* frequencies;

};
}
/** @}
 * End of documentation group waveobject
 */
#endif
