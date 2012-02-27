
#ifndef ARTMODEL_H
#define ARTMODEL_H

#include "ARTwaveObject.h"
#include "ARTlink.h"
#include "ART.h"
#include "ARTmodelinterface.h"

/** \addtogroup ARTmodelInterfaceGroup ARTmodelInterface
 * @{
 */

/**
 * A template model class using HornElements to calculate impedance. 
 */ 
template <class HE> class HornElementPrototype_FD : public ARTmodelInterface
{
private:
	//functionoid as wrapper for multimodeRadiationImpedance
	/**
	 * This class has its own functionoid to calculate radiation impedance.
	 */   	
	class RadiationImpedanceFunc : public ARTfunctionoid
	{
		public:
		  /**
		   * The constructor preparing the calculation
		   * @param host The object of class HornElementPrototype_FD using this functionoid.
		   * @param frequencies A  pointer to a data container holding a list of angular frequencies.
		   * @param modes The number of modes
		   * @param hasBends true if the circuit this model is used in has bent elements. The HornElement-s will 
		   * then use a different layout in the multimode impedance matrix.                      		   
		   */       		  
			RadiationImpedanceFunc(HornElementPrototype_FD<HE>* host, ARTdataContainer* frequencies, ARTdataContainer* modes, bool hasBends)
				: host_(host), frequencies_(frequencies), hasBends_(hasBends), modes_(modes)
				{
					if (!host) throw ARTerror("RadiationImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","host");
					if (!frequencies) throw ARTerror("RadiationImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","host");
					if (!modes) throw ARTerror("RadiationImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","host");
					in_.push_back(frequencies);
					in_.push_back(modes);
				}
				
			/**
			 * Returns true if functionoid f is the same as this functionoid.
			 */       			
			virtual bool IsSameFunctionoid(ARTfunctionoid* f)
				{
					RadiationImpedanceFunc* mf = dynamic_cast<RadiationImpedanceFunc*>(f);
					if (!mf) return false;
					if (mf->frequencies_ != frequencies_) return false;
					if (mf->modes_ != modes_) return false;
					if (mf->host_ != host_) return false;
					return true;
				}
			/**
			 * Called by the data container to register all dependencies.
			 */       			
			virtual void SetDependencies()
				{
					out_->AddDependency(frequencies_);
					out_->AddDependency(modes_);
					out_->AddPropertiesAsDependency(host_);
				}
				
			/// This is the function where the calculation takes place. see ARTfunctionoid::ApplyFunction() for details	
			virtual void ApplyFunction()
				{
					int modes = modes_->GetValueAsInt();
					//prepare hornelement
					host_->PrepareCalculation(modes);

					out_->SetType(C_ART_nmatx);
					out_->SetArrayLength(frequencies_->len);

					HornElement *hostElement = host_->GetImplementation();

					int i;
					for (i = 0; i < frequencies_->len; i++)
					{
						ARTdataContainer::progressIndicator.Continue(out_->GetComplexity(),out_->GetVarName());
						delete out_->val->nmx[i]; //Actually the datacontainer object should do work like this...
						out_->val->nmx[i] = new Matrix(modes, modes);
						hostElement->multimodeRadiationImpedance(frequencies_->val->nd[i], hasBends_, i , out_->val->nmx[i]);
						//std::cout << "RadImp(" << i <<")=" << *(out_->val->nmx[i]) << "\n";
					}
					//std::cout << "geschrieben in DC " << out_ << "\n";
				}
			/// see ARTfunctionoid::GetIterationNumber() for details	
			virtual int GetIterationNumber() {	return frequencies_->GetIterationNumber();	}

		private:
			HornElementPrototype_FD<HE>* host_;
			bool hasBends_;
			ARTdataContainer* frequencies_;
			ARTdataContainer* modes_;
	};

	//functionoid as wrapper for multimodeInputImpedance
	/**
	 * This class has its own functionoid to calculate input impedance.
	 */   	
	class InputImpedanceFunc : public ARTfunctionoid
	{
		public:
		  /**
		   * The constructor preparing the calculation
		   * @param host The object of class HornElementPrototype_FD using this functionoid.
		   * @param wfrequencies A  pointer to a data container holding a list of angular frequencies.
		   * @param modes The number of modes
		   * @param prop_z A pointer to a data container containing the impedance to be propagated through the model using this functionoid. 
		   * @param hasBends true if the circuit this model is used in has bent elements. The HornElement-s will 
		   * then use a different layout in the multimode impedance matrix.                      		   
		   */       		  
			InputImpedanceFunc(HornElementPrototype_FD<HE>* host, ARTdataContainer* wfrequencies, ARTdataContainer* modes, ARTdataContainer* prop_z, bool hasBends)
				: host_(host), wfrequencies_(wfrequencies), prop_z_(prop_z), hasBends_(hasBends), modes_(modes)
				{
					if (!host) throw ARTerror("InputImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","host");
					if (!wfrequencies) throw ARTerror("InputImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","wfrequencies");
					if (!modes) throw ARTerror("InputImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","modes");
					if (!prop_z) throw ARTerror("InputImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","prop_z");
					in_.push_back(wfrequencies);
					in_.push_back(modes);
					in_.push_back(prop_z);
				}
			/**
			 * Returns true if functionoid f is the same as this functionoid.
			 */       			
			virtual bool IsSameFunctionoid(ARTfunctionoid* f)
				{
					InputImpedanceFunc* mf = dynamic_cast<InputImpedanceFunc*>(f);
					if (!mf) return false;
					if (mf->wfrequencies_ != wfrequencies_) return false;
					if (mf->prop_z_ != prop_z_) return false;
					if (mf->host_ != host_) return false;
					if (mf->modes_ != modes_) return false;
					return true;
				}
			/**
			 * Called by the data container to register all dependencies.
			 */       			
			virtual void SetDependencies()  
				{
					out_->AddDependency(wfrequencies_);
					out_->AddDependency(prop_z_);
					out_->AddPropertiesAsDependency(host_);
					out_->AddDependency(modes_);
				}
			/// This is the function where the calculation takes place. see ARTfunctionoid::ApplyFunction() for details	
			virtual void ApplyFunction()
				{
					int modes = modes_->GetValueAsInt();
					//prepare hornelement
					//std::cout << "lesen von DC " << prop_z_ << "\n";
					host_->PrepareCalculation(modes); //HIER
					out_->SetType(C_ART_nmatx);
					out_->SetArrayLength(wfrequencies_->len);
					Matrix z_t = Matrix(modes,modes);
					HornElement *hostElement = host_->GetImplementation(); //HIER
					int i;
					for (i = 0; i < wfrequencies_->len; i++)
					{
						ARTdataContainer::progressIndicator.Continue(out_->GetComplexity(),out_->GetVarName());
						z_t = *prop_z_->val->nmx[i];
						hostElement->multimodeInputImpedance(wfrequencies_->val->nd[i], hasBends_, &z_t);
						delete out_->val->nmx[i];
						out_->val->nmx[i] = new Matrix(z_t);
					}
					/*
					std::cout << *out_->val->nmx[0] << "\n";
					std::cout << "\n";
					std::cout << " ************************************************** \n";
					std::cout << "\n";
					*/
				}
			/// see ARTfunctionoid::GetIterationNumber() for details	
			virtual int GetIterationNumber()	{	return wfrequencies_->GetIterationNumber();	}

		private:
			HornElementPrototype_FD<HE>* host_;
			bool hasBends_;
			ARTdataContainer* wfrequencies_;
			ARTdataContainer* prop_z_;
			ARTdataContainer* modes_;
	};


    public:

  /**
   * The constructor of the model. Usually properties, methods and data properties are appended in the constructor.
   * But since this class is a template used for all HornElement-s, properties and such are appended after
   * the models's construction as prototype in the root objects constructor (AcousticResearchTool::AcousticResearchTool).      
   */     
	HornElementPrototype_FD(const string name, const string sds="", const string lds="", const string htm="")
	: ARTmodelInterface(name,sds,lds,htm)
	{
		implementation = new HE();
	}
	
	/**
	 * A copy constructor. Used to copy the model when elements are created that use this model. 
	 */   	
	HornElementPrototype_FD(const HornElementPrototype_FD& orig) : ARTmodelInterface(orig)
	{
		//std::cout << "implementation = " << implementation << "\n";
		//std::cout << "this = " << &orig << "\n";
		this->implementation = (HE*)(orig.implementation->clone());
		//other stuff should be copied by ancestors' copy constructors
	}

  /**
   * Returns a copy of the object by calling the copy constructor. Used to copy the model when elements are created that use this model.
   */     
	virtual ARTmodelInterface* CloneModel() {return new HornElementPrototype_FD<HE>(*this);}

  /**
   * Prepares the calculation of the radiation impedance in the waveobject passed as argument. Uses the 
   * functionoids of this object for calculation.
   * @param oSignal The waveobject the radiation impedance should be written to. If the pointer is null, a
   * new waveobject will be created and the address written to oSignal.     
   */
	virtual void RadiationImpedance(WaveObjectInterface*& oSignal) 
	{
		if (!circuit) throw ARTerror("HornElementPrototype_FD::RadiationImpedance", "circuit is NULL. Did you set it using SetCircuit()?");
		if (!simulator) throw ARTerror("HornElementPrototype_FD::RadiationImpedance", "simulator is NULL. Did you set it using SetSimulator()?");
		//Funktor vorbereiten  /  prepare functionoid
		RadiationImpedanceFunc* formel =
			  new RadiationImpedanceFunc(this, simulator->GetAngularFrequencyGrid(), simulator->GetNumberOfModes(),circuit->HasBends()); //hasbends
		//Ausgangssignal  / output signal
		if (!oSignal)
			oSignal = new WaveObjectMMImpedance( "ZR_" + name_, formel, simulator->GetFrequencyGrid() );
		else
		{
			((WaveObjectMMImpedance*)oSignal)->SetCalculation( formel, simulator->GetFrequencyGrid() );
			((WaveObjectMMImpedance*)oSignal)->name_ = "ZR_" + name_;
		}
	}

  /**
   * Prepares the calculation of the input impedance in the waveobject passed as argument. Uses the 
   * functionoids of this object for calculation.
   * @param oSignal The waveobject the input impedance should be written to. If the pointer is null, a
   * new waveobject will be created and the address written to oSignal.
   * @param waveFront A waveobject of the impedance to be propagated through this model.       
   */
	virtual void InputImpedance(WaveObjectInterface* waveFront, WaveObjectInterface*& oSignal)
	{
		if (!circuit) throw ARTerror("HornElementPrototype_FD::InputImpedance", "circuit is NULL. Did you set it using SetCircuit()?");
		if (!simulator) throw ARTerror("HornElementPrototype_FD::InputImpedance", "simulator is NULL. Did you set it using SetSimulator()?");

		//Dieses Modell braucht die Impedanzmatrix im Frequenzbereich / this model needs the imp. in the frq domain
		ARTdataContainer* Impedanz = NULL;
		waveFront->MultiModeImpedanceMatrix(Impedanz);
		
		//Funktor vorbereiten / prepare functionoid
		InputImpedanceFunc* formel =
			  new InputImpedanceFunc(this, simulator->GetAngularFrequencyGrid(), simulator->GetNumberOfModes(), Impedanz, circuit->HasBends()); //hasbends

		//Ausgangssignal / output signal
		if (!oSignal)
			oSignal = new WaveObjectMMImpedance( "Z_" + name_, formel, simulator->GetFrequencyGrid() );
		else
		{
			((WaveObjectMMImpedance*)oSignal)->SetCalculation( formel, simulator->GetFrequencyGrid() );
			//((WaveObjectMMImpedance*)oSignal)->name_ = "Z_" + name_;
		}
	} 

  /**
   * This function must be implemented because it is declared as pure virtual in the ARTmodelInterface. So 
   * far it is not implemented nor used and will just throw an exception of type ARTerror.          
   */
	virtual void Pressure(WaveObjectInterface*, WaveObjectInterface*&) {throw ARTerror("HornElementPrototype_FD::Pressure","The function is not implemented.");};
	virtual void MultimodePressure(WaveObjectInterface*, WaveObjectInterface*&) {throw ARTerror("HornElementPrototype_FD::Pressure","The function is not implemented.");};

  /**
   * This prepares the calculation in the attached HornElement-object, by setting all parameters. The function
   * is called by the functionoids of this object.
   * @param modes The number of modes.      
   */     
	void PrepareCalculation(int modes) 
	{
		if (implementation == NULL) throw ARTerror("HornElementPrototype_FD::PrepareCalculation", "implementation is NULL");
		//set some calculation parameters...
		implementation->setnModes(modes);
		implementation->setlossy(true);
		implementation->setsimplified(false);
		implementation->setboreresolution(0.5); //was ist das?
		//set hornelement's parameters: get pointers to the parameters and write values of ARTelement's properties there.
		ARTproperty* prop = GetProperties(NULL);
		while (prop)
		{
			ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(prop);
			if (dprop) 
			{
				//...get corresponding pointer to variable in implementation
				ARTvariant* p = (implementation)->getPropertyPointer(dprop->GetName().c_str() );
				if (p->typ == C_ART_undef) throw ARTerror("ARTelement::PrepareCalculation", "The hornelement attached to element '%s1' does not have the property '%s2', which is specified in the ARTelement.", name_ , dprop->GetName().c_str());
				switch (p->typ)
				{
					case C_ART_ndbl:
						*p->val->nd = dprop->GetValueAsDouble();
						break;
					case C_ART_nflo:
						*p->val->nf = dprop->GetValueAsDouble();
						break;
					default:
						throw ARTerror("ARTelement::PrepareCalculation", "The hornelement attached to element '%s1' returned the data property '%s2' with an unknown type.", name_ , dprop->GetName().c_str());
						break;
				}
			}
			prop = GetProperties(prop);
		}
	}

	HE* GetImplementation() {return implementation;}

	private:
	  /// An object of a derivative class of HornElement which is used for calculation. 
		HE* implementation; 
};     






/**
 * Represents a tone hole in a circuit. This class uses the HornElement classes CylinderSection and Branch to
 * model the ton hole. 
 */
class ToneHole : public ARTmodelInterface
{
private:
	//functionoid as wrapper for multimodeInputImpedance
	class InputImpedanceFunc : public ARTfunctionoid
	{
		public:
		  /**
		   * The constructor preparing the calculation
		   * @param host The object of class HornElementPrototype_FD using this functionoid.
		   * @param wfrequencies A  pointer to a data container holding a list of angular frequencies.
		   * @param modes The number of modes
		   * @param prop_z A pointer to a data container containing the impedance to be propagated through the model using this functionoid. 
		   * @param hasBends true if the circuit this model is used in has bent elements. The HornElement-s will 
		   * then use a different layout in the multimode impedance matrix.                      		   
		   */    
       	InputImpedanceFunc(ToneHole* host, ARTdataContainer* wfrequencies, ARTdataContainer* modes, ARTdataContainer* prop_z, bool hasBends)
				: host_(host), wfrequencies_(wfrequencies), prop_z_(prop_z), hasBends_(hasBends), modes_(modes)
				{
					if (!host) throw ARTerror("InputImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","host");
					if (!wfrequencies) throw ARTerror("InputImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","wfrequencies");
					if (!modes) throw ARTerror("InputImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","modes");
					if (!prop_z) throw ARTerror("InputImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","prop_z");
					in_.push_back(wfrequencies);
					in_.push_back(modes);
					in_.push_back(prop_z);
				}
			/**
			 * Returns true if functionoid f is the same as this functionoid.
			 */       			
      virtual bool IsSameFunctionoid(ARTfunctionoid* f)
				{
					InputImpedanceFunc* mf = dynamic_cast<InputImpedanceFunc*>(f);
					if (!mf) return false;
					if (mf->wfrequencies_ != wfrequencies_) return false;
					if (mf->prop_z_ != prop_z_) return false;
					if (mf->host_ != host_) return false;
					if (mf->modes_ != modes_) return false;
					return true;
				}

			/**
			 * Called by the data container to register all dependencies.
			 */       			
			virtual void SetDependencies()  
				{
					out_->AddDependency(wfrequencies_);
					out_->AddDependency(prop_z_);
					out_->AddPropertiesAsDependency(host_);
					out_->AddDependency(modes_);
				}

			/// This is the function where the calculation takes place. see ARTfunctionoid::ApplyFunction() for details	
			virtual void ApplyFunction()
				{
					int modes = modes_->GetValueAsInt();
					//prepare hornelement
					host_->PrepareCalculation(modes); 
					out_->SetType(C_ART_nmatx);
					out_->SetArrayLength(wfrequencies_->len);
					Matrix z_hole = Matrix(modes,modes);
					Matrix z_inst = Matrix(modes,modes);
					Matrix one = Matrix(modes,modes);
					Matrix radiation = Matrix(modes,modes);
					Matrix z_in1 = Matrix(1,1);
					Matrix z_in2 = Matrix(1,1);
					one(0,0) = dcomp(1,0);
					//The radiation type as a complex number... grotesque!
					radiation(0,0) = dcomp(host_->GetRadiationType(),0);
					/*Matrix zero = Matrix(modes,modes);
					zero(0,0) = dcomp(0,0);*/
					CylinderSection* cyl = host_->GetTunnel(); 
					Branch* bra = host_->GetBranch(); 
					TerminationElement* te = host_->GetTermination(); 
					int i;
					for (i = 0; i < wfrequencies_->len; i++)
					{
						delete out_->val->nmx[i];
						out_->val->nmx[i] = new Matrix(modes,modes);
						//inform about progress and check if user wants to coninue
						ARTdataContainer::progressIndicator.Continue(out_->GetComplexity(),out_->GetVarName());
						//get the radiation impedance of the tone hole from the termination element
						te->multimodeRadiationImpedance(wfrequencies_->val->nd[i], hasBends_, i , &z_hole);
						//get the impedance at the entrance of the tone hole
						cyl->multimodeInputImpedance(wfrequencies_->val->nd[i], hasBends_, &z_hole);
						//combine the impedance propagated through the instrument with the tone hole's impedance
						z_inst = *prop_z_->val->nmx[i];
						list<Matrix> ImpedanceList;
						ImpedanceList.push_back(one); //I don't know what this (1,0) is, it does not seem to be the same as below. windinstrument.cpp says "Number of hole for this branch" What is that supposed to mean???
						z_in1(0,0) = z_hole(0,0);  // The current branch implementation can only handle 1 mode... so as in the windinstrument, we will only pass the first mode.
						ImpedanceList.push_back(z_in1); 
						ImpedanceList.push_back(radiation); //This (1,0) is presumably the type of radiation. (1,0) must be ZORUMSKI_RADIATION -> no, radiation seems to be the number above... oh dear...
						z_in2(0,0) = z_inst(0,0);
						ImpedanceList.push_back(z_in2);
						bra->BranchImpedanceList(&ImpedanceList);
						(*out_->val->nmx[i]) = bra->GetParallelImpedance( wfrequencies_->val->nd[i] ,-1);
						ImpedanceList.clear();
					}


				}
			/// see ARTfunctionoid::GetIterationNumber() for details	
			virtual int GetIterationNumber()	{	return wfrequencies_->GetIterationNumber();	}

		private:
			ToneHole* host_;
			bool hasBends_;
			ARTdataContainer* wfrequencies_;
			ARTdataContainer* prop_z_;
			ARTdataContainer* modes_;
	};


    public:

	/**
	 * The constructor for the tone hole prototype. All properties, data properties and methods are appended in
	 * this constructor.	 
	 */   	
	ToneHole(const string name = "ToneHole", const string sds="a tone hole at the side of the instrument", const string lds="", const string htm="")
	: ARTmodelInterface(name,sds,lds,htm)
	{
		//Add all properties, data properties and methods to the model
		AppendDataProp("length", 50.0,"axial length [cm]");
		AppendDataProp("r", 1.0,"radius [cm]");
		AppendDataProp("temp", 21.0,"local temperature [degC]");
		AppendDataProp("loss", 1.0,"loss factor (defaults to 1)");
		ARTdataProp* dp = AppendDataProp("radiation", "Zorumski","The radiation impedance. Can be either 'Reflecting' or 'Zorumski'."); 
			dp->SetRange(new ARTvariant("Reflecting", "Zorumski"));
		AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
		AppendDataProp("xc", 0.000382,"CO2 molar fraction");

		AppendProperty("canPropagate","this element can propagate impedance"); 
		//AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); tone holes always have to propagate impedance!

		AppendMethod("SphericalWave", "MapesRiordan Model");
		AppendMethod("PlaneWave", "first mode only");
		AppendMethod("MultiModal", "modal decomposition up to n modes (Braden06)");
		AppendMethod("FrequencyDomain", "simulation in frq domain");
		//Create the element representing the cylindrical duct
		tunnel = new CylinderSection();
		branch = new Branch(false, false, 1, 1.0, 1.0, 0); //NOT the simple model (the last integer is 0, other values are default)
		term = new TerminationElement(); 
	}

	/**
	 * A copy constructor. Used to copy the model when elements are created that use this model. 
	 */   	
	ToneHole(const ToneHole& orig) : ARTmodelInterface(orig)
	{
		//Assume the element lies in the same simulator ...
		this->simulator = orig.simulator;
		//...but will use its own CylinderSection and Branch objects.
		this->tunnel = (CylinderSection*)(orig.tunnel->clone());
		this->branch = (Branch*)(orig.branch->clone());
		this->term = (TerminationElement*)(orig.term->clone());
		//other stuff will be copied by ancestors' copy constructors
	}

  /**
   * Returns a copy of the object by calling the copy constructor. Used to copy the model when elements are created that use this model.
   */     
	virtual ARTmodelInterface* CloneModel() {return new ToneHole(*this);}

  /**
   * This function must be implemented because it is declared as pure virtual in the ARTmodelInterface. 
   * Tone holes can not be the last (outermost) element in a circuit, so this method will throw an exception 
   * of type ARTerror.          
   */
	virtual void RadiationImpedance(WaveObjectInterface*& out)
	{
		throw ARTerror("ToneHole::RadiationImpedance","Can not compute radiation impedance of a tone hole. Tone holes can not be at the outer end of the instrument.");
	}

  /**
   * Prepares the calculation of the input impedance in the waveobject passed as argument. Uses the 
   * functionoids of this object for calculation.
   * @param oSignal The waveobject the input impedance should be written to. If the pointer is null, a
   * new waveobject will be created and the address written to oSignal.
   * @param waveFront A waveobject of the impedance to be propagated through this model.       
   */
	virtual void InputImpedance(WaveObjectInterface* waveFront, WaveObjectInterface*& oSignal )
	{
		if (!circuit) throw ARTerror("ToneHole::InputImpedance", "circuit is NULL. Did you set it using SetCircuit()?");
		if (!simulator) throw ARTerror("ToneHole::InputImpedance", "simulator is NULL. Did you set it using SetSimulator()?");
		//This model needs the multimode impedance matrix in the frequency domain: get it from the wave object
		ARTdataContainer* impedance = NULL;
		waveFront->MultiModeImpedanceMatrix(impedance);
		
		//now prepare the functionoid which will calculate the impedance at the output of this element
		InputImpedanceFunc* func =
			  new InputImpedanceFunc(this, simulator->GetAngularFrequencyGrid(), simulator->GetNumberOfModes(), impedance, circuit->HasBends());

		//Create a multimode frequency domain wave object which will use the functionoid when its value is queried
		if (!oSignal)
			oSignal = new WaveObjectMMImpedance("Z_" + name_,func, simulator->GetFrequencyGrid() );
		else 
		{
			((WaveObjectMMImpedance*)oSignal)->SetCalculation( func, simulator->GetFrequencyGrid() );
			//((WaveObjectMMImpedance*)oSignal)->name_ = "Z_" + name_;
		}
	} 

  /**
   * This function must be implemented because it is declared as pure virtual in the ARTmodelInterface. So 
   * far it is not implemented nor used and will just throw an exception of type ARTerror.          
   */
	virtual void Pressure(WaveObjectInterface*, WaveObjectInterface*&) {throw ARTerror("ToneHole::Pressure","The function is not implemented.");};

  /**
   * This prepares the calculation in the attached CylinderSection and Branch objects, by setting all parameters. The function
   * is called by the functionoid of this object.
   * @param modes The number of modes.      
   */     
	void PrepareCalculation(int modes) //Kopie von ARTelement::PrepareCalculation
	{
		if (tunnel == NULL) throw ARTerror("ToneHole::PrepareCalculation", "The object 'tunnel' is NULL");
		if (term == NULL) throw ARTerror("ToneHole::PrepareCalculation", "The object 'term' is NULL");

		//set some calculation parameters...
		tunnel->setnModes(modes);
		tunnel->setlossy(true);
		tunnel->setsimplified(false);
		tunnel->setboreresolution(0.5); 

		branch->setnModes(modes);
		branch->setlossy(true);
		branch->setsimplified(false);
		branch->setboreresolution(0.5); 

		term->setnModes(modes);
		term->setlossy(true);
		term->setsimplified(false);
		term->setboreresolution(0.5); 
		term->RADIATION_TYPE_ = GetRadiationType();
		//Use the data properties of the element to set the cylindersection's values

		double len;
		double rad;

		//Use the data properties of the element to set the cylindersection's values
		ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(FindProperty("length"));
		if (dprop)
			tunnel->length( len = dprop->GetValueAsDouble() ); 
		else 
			throw ARTerror("ToneHole::PrepareCalculation", "The data property '%s1' was not found.", "length"); //Debug

		dprop = dynamic_cast<ARTdataProp*>(FindProperty("r"));
		if (dprop)
		{
			tunnel->radius( rad = dprop->GetValueAsDouble()); 
			branch->radius( rad ); 
			term->radius( rad ); 
		}
		else 
			throw ARTerror("ToneHole::PrepareCalculation", "The data property '%s1' was not found.", "r");

		dprop = dynamic_cast<ARTdataProp*>(FindProperty("temp"));
		if (dprop) 
		{
			tunnel->settempC(dprop->GetValueAsDouble()); 
			branch->settempC(dprop->GetValueAsDouble()); 
			term->settempC(dprop->GetValueAsDouble()); 
		} 
		else 
			throw ARTerror("ToneHole::PrepareCalculation", "The data property '%s1' was not found.", "temp");

		dprop = dynamic_cast<ARTdataProp*>(FindProperty("loss"));
		if (dprop) 
		{
			tunnel->setlossF(dprop->GetValueAsDouble());
			branch->setlossF(dprop->GetValueAsDouble());
			term->setlossF(dprop->GetValueAsDouble());
		} 
		else 
			throw ARTerror("ToneHole::PrepareCalculation", "The data property '%s1' was not found.", "loss");
		
		dprop = dynamic_cast<ARTdataProp*>(FindProperty("xc"));
		if (dprop) 
		{
			tunnel->setxc(dprop->GetValueAsDouble());
			branch->setxc(dprop->GetValueAsDouble());
			term->setxc(dprop->GetValueAsDouble());
		} 
		else 
			throw ARTerror("TerminationModel::PrepareCalculation", "The data property '%s1' was not found.", "xc");

		dprop = dynamic_cast<ARTdataProp*>(FindProperty("humidity"));
		if (dprop) 
		{
			tunnel->sethumidity(dprop->GetValueAsDouble());
			branch->sethumidity(dprop->GetValueAsDouble());
			term->sethumidity(dprop->GetValueAsDouble());
		} 
		else 
			throw ARTerror("TerminationModel::PrepareCalculation", "The data property '%s1' was not found.", "humidity");

		branch->PutHoleParam(len, rad);
	}

	int GetRadiationType()
	{
		ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(FindProperty("radiation"));
		if (dprop) 
		{
			if (dprop->IsEqual(&ARTvariant("Reflecting"))) return REFLECTING_RADIATION;
			if (dprop->IsEqual(&ARTvariant("Zorumski"))) return ZORUMSKI_RADIATION;
			if (dprop->IsEqual(&ARTvariant("Levine"))) return LEVINE_RADIATION;
			throw ARTerror("ToneHole::GetRadiationType", "The value '%s1' of the property '%s2' is invalid. Please check the description for valid values.", dprop->GetValue()->s, "radiation");
		} 
		else 
			throw ARTerror("ToneHole::GetRadiationType", "The data property '%s1' was not found.", "radiation");
	}
  ///Returns the CylinderSection object representing the tunnel. Used by the functionoid of this object. 
	CylinderSection* GetTunnel() {return tunnel;}
	///Returns the branch object. Used by the functionoid of this object.
	Branch* GetBranch() {return branch;}
	TerminationElement* GetTermination() {return term;}

	private:
		CylinderSection* tunnel; ///The cylindrical duct to the tonhole is implemented as other cylinders
		Branch* branch; ///The cylindrical duct is branched to the circuit by this object		
		TerminationElement* term; ///The cylindrical duct is branched to the circuit by this object		
};     






/**
 * Represents a branch in a circuit. This class combines to parallel impedances. 
 */
class BranchModel : public ARTmodelInterface
{
private:
	//functionoid as wrapper for multimodeInputImpedance
	class CombineParallelImpedanceFunc : public ARTfunctionoid
	{
		public:
		  /**
		   * The constructor preparing the calculation
		   * @param host The object of class HornElementPrototype_FD using this functionoid.
		   * @param wfrequencies A  pointer to a data container holding a list of angular frequencies.
		   * @param modes The number of modes
		   * @param prop_z A pointer to a data container containing the impedance to be propagated through the model using this functionoid. 
		   * @param hasBends true if the circuit this model is used in has bent elements. The HornElement-s will 
		   * then use a different layout in the multimode impedance matrix.                      		   
		   */    
       	CombineParallelImpedanceFunc(ARTdataContainer* wfrequencies, ARTdataContainer* modes, ARTdataContainer* prop_z1, ARTdataContainer* prop_z2)
				: wfrequencies_(wfrequencies), modes_(modes), prop_z1_(prop_z1), prop_z2_(prop_z2)
				{
					if (!prop_z1) throw ARTerror("CombineParallelImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","prop_z1");
					if (!prop_z2) throw ARTerror("CombineParallelImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","prop_z2");
					if (!wfrequencies) throw ARTerror("CombineParallelImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","wfrequencies");
					if (!modes) throw ARTerror("CombineParallelImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","modes");
					in_.push_back(modes);
					in_.push_back(wfrequencies);
					in_.push_back(prop_z1);
					in_.push_back(prop_z2);
				}
			/**
			 * Returns true if functionoid f is the same as this functionoid.
			 */       			
      virtual bool IsSameFunctionoid(ARTfunctionoid* f)
				{
					CombineParallelImpedanceFunc* mf = dynamic_cast<CombineParallelImpedanceFunc*>(f);
					if (!mf) return false;
					if (mf->prop_z1_ != prop_z1_) return false;
					if (mf->prop_z2_ != prop_z2_) return false;
					if (mf->wfrequencies_ != wfrequencies_) return false;
					if (mf->modes_ != modes_) return false;
					return true;
				}

			/**
			 * Called by the data container to register all dependencies.
			 */       			
			virtual void SetDependencies()  
				{
					out_->AddDependency(prop_z1_);
					out_->AddDependency(prop_z2_);
					out_->AddDependency(wfrequencies_);
					out_->AddDependency(modes_);
				}

			/// This is the function where the calculation takes place. see ARTfunctionoid::ApplyFunction() for details	
			virtual void ApplyFunction()
				{
					int modes = modes_->GetValueAsInt();
					//make sure output matrix is big enough to hold all results
					out_->SetType(C_ART_nmatx);
					out_->SetArrayLength(wfrequencies_->len);

					for (int i = 0; i < wfrequencies_->len; i++)
					{
						//inform about progress and check if user wants to coninue
						ARTdataContainer::progressIndicator.Continue(out_->GetComplexity(),out_->GetVarName());
						delete out_->val->nmx[i]; //get rid of old matrix in data container
						out_->val->nmx[i] = new Matrix(modes,modes); //create new one
						//The input impedance Z of a branch with the two parallel impedances Z1 and Z2 is defined as:
						//  1       1       1
						// ---  =  ---  +  ---
						//  Z       Z1      Z2
						//std::cout << (*prop_z1_->val->nmx[i]) << "\n" << (*prop_z2_->val->nmx[i]) << "\n\n*\n";
						for (int m = 0; m < modes; m++)
							for (int n = 0; n < modes; n++)
							{
								//this is the calculation above, solved for Z
								//to avoid divisions by 0:
								dcomp Z1 = dcomp( (*prop_z1_->val->nmx[i])(m,n).real() ? (*prop_z1_->val->nmx[i])(m,n).real() : 1e-023, 
															(*prop_z1_->val->nmx[i])(m,n).imag() ? (*prop_z1_->val->nmx[i])(m,n).imag() : 1e-023 );
								dcomp Z2 = dcomp( (*prop_z2_->val->nmx[i])(m,n).real() ? (*prop_z2_->val->nmx[i])(m,n).real() : 1e-023, 
															(*prop_z2_->val->nmx[i])(m,n).imag() ? (*prop_z2_->val->nmx[i])(m,n).imag() : 1e-023 );
								(*out_->val->nmx[i])(m,n) = dcomp(1,0) / (dcomp(1,0) / Z1 + dcomp(1,0) / Z2);
							}
					}
				
				}
			/// see ARTfunctionoid::GetIterationNumber() for details	
			virtual int GetIterationNumber()	{	return wfrequencies_->GetIterationNumber();	}

		private:
			ARTdataContainer* wfrequencies_;
			ARTdataContainer* prop_z1_;
			ARTdataContainer* prop_z2_;
			ARTdataContainer* modes_;
	};


    public:

	/**
	 * The constructor for the tone hole prototype. All properties, data properties and methods are appended in
	 * this constructor.	 
	 */   	
	BranchModel(const string name = "Branch", const string sds="a branch combining two parallel impedances", const string lds="", const string htm="")
	: ARTmodelInterface(name,sds,lds,htm)
	{
		//There are no data properties.
		//Add properties
		AppendMethod("SphericalWave", "MapesRiordan Model");
		AppendMethod("PlaneWave", "first mode only");
		AppendMethod("MultiModal", "modal decomposition up to n modes (Braden06)");
		AppendMethod("FrequencyDomain", "simulation in frq domain");

		AppendProperty("canPropagate","this element can propagate impedance"); 
		//AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); branches themselves do not radiate sound
	}

	/**
	 * A copy constructor. Used to copy the model when elements are created that use this model. 
	 */   	
	BranchModel(const BranchModel& orig) : ARTmodelInterface(orig)
	{
		//Assume the element lies in the same simulator ...
		this->simulator = orig.simulator;
		//other stuff will be copied by ancestors' copy constructors
	}

  /**
   * Returns a copy of the object by calling the copy constructor. Used to copy the model when elements are created that use this model.
   */     
	virtual ARTmodelInterface* CloneModel() {return new BranchModel(*this);}

  /**
   * This function must be implemented because it is declared as pure virtual in the ARTmodelInterface. 
   * Branches can not be the last (outermost) element in a circuit, so this method will throw an exception 
   * of type ARTerror.          
   */
	virtual void RadiationImpedance(WaveObjectInterface*& out)
	{
		throw ARTerror("BranchModel::RadiationImpedance","Can not compute radiation impedance of a branch model. Branches can not be at the outer end of the instrument.");
	}

  /**
   * This function must be implemented because it is declared as pure virtual in the ARTmodelInterface. 
   * Branches propagate more than one impedance object, so this method will throw an exception 
   * of type ARTerror.          
   */
	virtual void InputImpedance(WaveObjectInterface* waveFront, WaveObjectInterface*& oSignal )
	{
		throw ARTerror("BranchModel::InputImpedance","Branches combine two impedances, please pass a vector containing two impedances to the alternate method InputImpedance(vector<WaveObjectInterface*>& in, WaveObjectInterface*& out).");
	} 

	virtual void InputImpedance(vector<WaveObjectInterface*>& in, WaveObjectInterface*& oSignal) 
	{
		if (!circuit) throw ARTerror("BranchModel::InputImpedance", "circuit is NULL. Did you set it using SetCircuit()?");
		if (!simulator) throw ARTerror("BranchModel::InputImpedance", "simulator is NULL. Did you set it using SetSimulator()?");
		//check that the input vector has 2 elements
		if (in.size() != 2) throw ARTerror("BranchModel::InputImpedance", "The input vector does not contain exatly two waveobjects as expected.");

		//This model needs the multimode impedance matrices in the frequency domain: get them from the wave object
		ARTdataContainer* impedance1 = NULL;
		ARTdataContainer* impedance2 = NULL;
		in[0]->MultiModeImpedanceMatrix(impedance1);
		in[1]->MultiModeImpedanceMatrix(impedance2);
		
		//now prepare the functionoid which will calculate the impedance at the output of this element by combining the two parallel impedances
		CombineParallelImpedanceFunc* func =
			  new CombineParallelImpedanceFunc(simulator->GetAngularFrequencyGrid(), simulator->GetNumberOfModes(), impedance1, impedance2);

		//Prepare a multimode frequency domain wave object which will use the functionoid when its value is queried
		if (!oSignal) //create a new waveobject
			//this is like writing down a formula for the output signal. The formula is the functionoid created above. The result is converted to a
			//multimode impedance wave object.
			oSignal = new WaveObjectMMImpedance("Z_" + name_,func, simulator->GetFrequencyGrid() );
		else 
		{	//or use the waveobject passed as output argument
			// we set the formula to the functionoid created above.
			((WaveObjectMMImpedance*)oSignal)->SetCalculation( func, simulator->GetFrequencyGrid() );
			//((WaveObjectMMImpedance*)oSignal)->name_ = "Z_" + name_;
		}
	};

  /**
   * This function must be implemented because it is declared as pure virtual in the ARTmodelInterface. So 
   * far it is not implemented nor used and will just throw an exception of type ARTerror.          
   */
	virtual void Pressure(WaveObjectInterface*, WaveObjectInterface*&) {throw ARTerror("ToneHole::Pressure","The function is not implemented.");};

};     







/** 
 * Represents a termination element. Use this prototype for terminations that can be open or closed. 
 */
class TerminationModel : public ARTmodelInterface
{
private:
	//functionoid as wrapper for multimodeInputImpedance
	class RadiationImpedanceFunc : public ARTfunctionoid
	{
		public:
		  /**
		   * The constructor preparing the calculation
		   * @param host The object of class HornElementPrototype_FD using this functionoid.
		   * @param wfrequencies A  pointer to a data container holding a list of angular frequencies.
		   * @param modes The number of modes
		   * @param prop_z A pointer to a data container containing the impedance to be propagated through the model using this functionoid. 
		   * @param hasBends true if the circuit this model is used in has bent elements. The HornElement-s will 
		   * then use a different layout in the multimode impedance matrix.                      		   
		   */    
       	RadiationImpedanceFunc(TerminationModel* host, ARTdataContainer* wfrequencies, ARTdataContainer* modes, bool hasBends)
				: host_(host), wfrequencies_(wfrequencies), hasBends_(hasBends), modes_(modes)
				{
					if (!host) throw ARTerror("RadiationImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","host");
					if (!wfrequencies) throw ARTerror("RadiationImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","wfrequencies");
					if (!modes) throw ARTerror("RadiationImpedanceFunc(Constructor)", "Argument '%s1' is NULL.","modes");
					in_.push_back(wfrequencies);
					in_.push_back(modes);
				}
			/**
			 * Returns true if functionoid f is the same as this functionoid.
			 */       			
      virtual bool IsSameFunctionoid(ARTfunctionoid* f)
				{
					RadiationImpedanceFunc* mf = dynamic_cast<RadiationImpedanceFunc*>(f);
					if (!mf) return false;
					if (mf->wfrequencies_ != wfrequencies_) return false;
					if (mf->host_ != host_) return false;
					if (mf->modes_ != modes_) return false;
					return true;
				}

			/**
			 * Called by the data container to register all dependencies.
			 */       			
			virtual void SetDependencies()  
				{
					out_->AddDependency(wfrequencies_);
					out_->AddPropertiesAsDependency(host_);
					out_->AddDependency(modes_);
				}

			/// This is the function where the calculation takes place. see ARTfunctionoid::ApplyFunction() for details	
			virtual void ApplyFunction()
				{
					int modes = modes_->GetValueAsInt();
					//prepare hornelement
					host_->PrepareCalculation(modes); 
					out_->SetType(C_ART_nmatx);
					out_->SetArrayLength(wfrequencies_->len);
					TerminationElement* te = host_->GetModel(); 
					int i;
					for (i = 0; i < wfrequencies_->len; i++)
					{
						delete out_->val->nmx[i];
						out_->val->nmx[i] = new Matrix(modes,modes);
						//inform about progress and check if user wants to coninue
						ARTdataContainer::progressIndicator.Continue(out_->GetComplexity(),out_->GetVarName());
						//get the radiation impedance of the termination hornelement
						te->multimodeRadiationImpedance(wfrequencies_->val->nd[i], hasBends_, i , out_->val->nmx[i]);

					}
				
				}
			/// see ARTfunctionoid::GetIterationNumber() for details	
			virtual int GetIterationNumber()	{	return wfrequencies_->GetIterationNumber();	}

		private:
			TerminationModel* host_;
			bool hasBends_;
			ARTdataContainer* wfrequencies_;
			ARTdataContainer* modes_;
	};


    public:

	/**
	 * The constructor for the termination prototype. All properties, data properties and methods are appended in
	 * this constructor.	 
	 */   	
	TerminationModel(const string name = "TerminationModel", const string sds="a tone hole at the side of the instrument", const string lds="", const string htm="")
	: ARTmodelInterface(name,sds,lds,htm)
	{
		//Add all properties, data properties and methods to the model
		AppendDataProp("radiationradius", 1.0,"The radius of the radiating surface. [cm]");
		AppendDataProp("temp", 21.0,"local temperature [degC]");
		AppendDataProp("loss", 1.0, "loss factor (defaults to 1)");
		ARTdataProp* dp = AppendDataProp("radiation", "Zorumski","The radiation impedance. Can be either 'Reflecting' or 'Zorumski'."); 
			dp->SetRange(new ARTvariant("Reflecting", "Zorumski"));
		AppendDataProp("humidity", 0.8,"air humidity (molar fraction)");
		AppendDataProp("xc", 0.000382,"CO2 molar fraction");
		
		//AppendProperty("canPropagate","this element can propagate impedance"); A termination element can not propagate impedance!
		AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); 

		AppendMethod("SphericalWave", "MapesRiordan Model");
		AppendMethod("PlaneWave", "first mode only");
		AppendMethod("MultiModal", "modal decomposition up to n modes (Braden06)");
		AppendMethod("FrequencyDomain", "simulation in frq domain");

		model = new TerminationElement();
	}

	/**
	 * A copy constructor. Used to copy the model when elements are created that use this model. 
	 */   	
	TerminationModel(const TerminationModel& orig) : ARTmodelInterface(orig)
	{
		//Assume the element lies in the same simulator ...
		this->simulator = orig.simulator;
		this->model = (TerminationElement*)(orig.model->clone());
	}

  /**
   * Returns a copy of the object by calling the copy constructor. Used to copy the model when elements are created that use this model.
   */     
	virtual ARTmodelInterface* CloneModel() {return new TerminationModel(*this);}

  /**
   * Prepares the calculation of the radiation impedance in the waveobject passed as argument. Uses the 
   * functionoids of this object for calculation.
   * @param out The waveobject the radiation impedance should be written to. If the pointer is null, a
   * new waveobject will be created and the address written to out.     
   */
	virtual void RadiationImpedance(WaveObjectInterface*& out)
	{
		if (!circuit) throw ARTerror("TerminationModel::InputImpedance", "circuit is NULL. Did you set it using SetCircuit()?");
		if (!simulator) throw ARTerror("TerminationModel::InputImpedance", "simulator is NULL. Did you set it using SetSimulator()?");
		
		//now prepare the functionoid which will calculate the radiation impedance of this element
		RadiationImpedanceFunc* func =
			  new RadiationImpedanceFunc(this, simulator->GetAngularFrequencyGrid(), simulator->GetNumberOfModes(), circuit->HasBends());

		//Create a multimode frequency domain wave object which will use the functionoid when its value is queried
		if (!out)
			out = new WaveObjectMMImpedance("Z_" + name_,func, simulator->GetFrequencyGrid() );
		else 
		{
			((WaveObjectMMImpedance*)out)->SetCalculation( func, simulator->GetFrequencyGrid() );
			//((WaveObjectMMImpedance*)out)->name_ = "Z_" + name_;
		}
	}

  /**
   * This function must be implemented because it is declared as pure virtual in the ARTmodelInterface. 
   * A termination can not be in the middle of a circuit, so this method will throw an exception 
   * of type ARTerror.          
   */
	virtual void InputImpedance(WaveObjectInterface* waveFront, WaveObjectInterface*& oSignal )
	{
		throw ARTerror("TerminationModel::InputImpedance","Can not propagate impedance through termination element. Termination elements can only lie on outer edges of branches or instruments.");
	} 

  /**
   * This function must be implemented because it is declared as pure virtual in the ARTmodelInterface. So 
   * far it is not implemented nor used and will just throw an exception of type ARTerror.          
   */
	virtual void Pressure(WaveObjectInterface*, WaveObjectInterface*&) {throw ARTerror("TerminationModel::Pressure","The function is not implemented.");};

  /**
   * This prepares the calculation in the attached TerminationElement object, by setting all parameters. The function
   * is called by the functionoid of this object.
   * @param modes The number of modes.      
   */     
	void PrepareCalculation(int modes) 
	{
		if (model == NULL) throw ARTerror("TerminationModel::PrepareCalculation", "The terminationElement object is NULL");

		//set some calculation parameters...
		model->setnModes(modes);
		model->setlossy(true);
		model->setsimplified(false);
		model->setboreresolution(0.5); //was ist das?
		model->RADIATION_TYPE_ = GetRadiationType();
		//Use the data properties of the element to set the cylindersection's values
		ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(FindProperty("radiationradius"));
		if (dprop)
			model->radius( dprop->GetValueAsDouble() ); 
		else 
			throw ARTerror("TerminationModel::PrepareCalculation", "The data property '%s1' was not found.", "radiationradius"); //Debug


		dprop = dynamic_cast<ARTdataProp*>(FindProperty("temp"));
		if (dprop) 
		{
			model->settempC(dprop->GetValueAsDouble()); 
		} 
		else 
			throw ARTerror("TerminationModel::PrepareCalculation", "The data property '%s1' was not found.", "temp");

		dprop = dynamic_cast<ARTdataProp*>(FindProperty("loss"));
		if (dprop) 
		{
			model->setlossF(dprop->GetValueAsDouble());
		} 
		else 
			throw ARTerror("TerminationModel::PrepareCalculation", "The data property '%s1' was not found.", "loss");
		
		dprop = dynamic_cast<ARTdataProp*>(FindProperty("xc"));
		if (dprop) 
		{
			model->setxc(dprop->GetValueAsDouble());
		} 
		else 
			throw ARTerror("TerminationModel::PrepareCalculation", "The data property '%s1' was not found.", "xc");

		dprop = dynamic_cast<ARTdataProp*>(FindProperty("humidity"));
		if (dprop) 
		{
			model->sethumidity(dprop->GetValueAsDouble());
		} 
		else 
			throw ARTerror("TerminationModel::PrepareCalculation", "The data property '%s1' was not found.", "humidity");
	}

	int GetRadiationType()
	{
		ARTdataProp* dprop = dynamic_cast<ARTdataProp*>(FindProperty("radiation"));
		if (dprop) 
		{
			if (dprop->IsEqual(&ARTvariant("Reflecting"))) return REFLECTING_RADIATION;
			if (dprop->IsEqual(&ARTvariant("Zorumski"))) return ZORUMSKI_RADIATION;
			if (dprop->IsEqual(&ARTvariant("Levine"))) return LEVINE_RADIATION;
			throw ARTerror("TerminationModel::GetRadiationType", "The value '%s1' of the property '%s2' is invalid. Please check the description for valid values.", dprop->GetValue()->s, "radiation");
		} 
		else 
			throw ARTerror("TerminationModel::GetRadiationType", "The data property '%s1' was not found.", "radiation");
	}

	TerminationElement* GetModel() {return model;}

	private:
		TerminationElement* model; ///The hornelement implementation of the termination model
};     

/**
 * @}
 */

#endif