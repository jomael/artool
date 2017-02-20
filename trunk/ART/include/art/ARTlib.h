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


/** 
 * @mainpage Acoustic Research Tool (ART) documentation
 *
 * @section sec_intro Introduction
 *
 * @subsection ss_sum Summary
 * 
 * ART is a flexible simulation framework for wind instruments. It includes a growing 
 * library of modelling elements. So far bore discontinuities, cylindrical and conical 
 * tubes, Bessel horns and bent tubes are available for frequency domain modelling.
 * 
 * This project is in its conceptual stage.
 * 
 * @ref sec_use "Skip the introduction and show me how to use it!"   
 * 
 * @subsection ss_spec Project specifications
 * 
 * The goal of this project is to provide an open, extendible and efficient acoustical 
 * simulation library which can be used by application programmers. This package should 
 * allow to realistically and efficiently model and simulate complicated and fairly 
 * general acoustical systems - especially wind instruments with or without tone holes 
 * and eventually the singing or speaking voice - in the frequency domain as well as in 
 * the time domain.
 * 
 * Special focus should be put on efficiency because computer optimisation may require 
 * many thousands of evaluations of virtual instruments which are usually only slightly 
 * modified between successive analysis steps by an optimiser application program. Time 
 * domain modelling often requires generating real time sound when parameters of the 
 * sound generator are modified.
 * 
 * Nevertheless, modelling accuracy and physical realism should be another major 
 * concern. The package should contain most accurate models for the frequency domain as 
 * well as for the time domain because it is a main goal of this project to provide the 
 * core of new and accurate tools for instrument makers as well as for scientists who 
 * want to study the influence of faint differences in geometry or other acoustical 
 * boundary conditions on sound and playability of musical instruments or on resonance 
 * frequencies, damping conditions and other properties of travelling or standing waves 
 * in general acoustical systems.
 * 
 * A main application which makes use of the library's frequency domain modeling will be 
 * the computer optimisation of general acoustical systems in terms of resonance and 
 * radiation characteristics, damping and excitation properties. Applied to musical 
 * instruments these characteristics are called intonation, sound quality, efficiency 
 * and playability. The library's time domain simulation mode will be used for 
 * synthesizing sounds of more or less simplified up to nearly realistic virtual 
 * instruments depending on the required tradeoff between accuracy and computation 
 * speed. By controlling various model parameters in real time articulation and 
 * phonation can be studied in virtual singers and speakers.
 * 
 * The library will be programmed in ANSI C++ with no assumptions about or dependencies 
 * of operating system or computer architecture. The Class dependencies will be defined 
 * in UML in order to enforce proper interplay and to make it easier to keep 
 * documentation and implementation consistent and extensible. Models have to register 
 * their parameters, usage details and documentary text in order to allow applications 
 * being independent of the implementation status of the library.
 * 
 * Applications written in any programming language should be able to easily access the 
 * library's functionality. All graphical and user interface related issues will have to 
 * be handled by the application program, otherwise complete platform independence would 
 * not be achievable.
 * 
 * The developing process will be a multi-national effort with independent contributions 
 * of several researchers, co-ordinated by a small board of experienced acousticians and 
 * software developers continuously communicating by means of a dedicated online forum 
 * hosted by the TC-MA of the EAA (the workgroup area). Concepts, documentation, sources 
 * and prototype applications will be made available, too. Contributions in terms of 
 * comments, ideas, conceptual proposals as well as actual C-code are highly welcome.
 * 
 * @subsection ssec_status Project Status (Version 0.10)
 *   
 * This project is still in an early stage. Nevertheless a preliminary working release has 
 * already been published. Although the concept does include other simulation domains, 
 * only frequency and a limited number of time domain models have been
 * implemented up to now. The currently existing
 * code allows to calculate input impedances of arbitrary bore profiles consisting of 
 * cylindrical sections, conical sections, bessel horn sections, exponential horn 
 * sections, bent cylindrical and conical tubes, bore discontinuities and branches (tone 
 * holes). All elements (except for the branch) take multi-modal wave propagation of an 
 * arbitrary number of modes into account. The very common case of 1 (plane wave), 2 or 3 
 * modes has been optimised for performance. Lossless as well as lossy wave propagation is 
 * available for all elements. Simulation parameters like air temperature, humidity and 
 * carbon dioxide content as well as a separate boundary layer loss factor can be 
 * specified for each individual section of the bore profile. Termination impedances can 
 * be chosen from a range of predefined radiation models.
 *
 * In the time domain, basic modules for Digital Waveguides (DWGs) have been implemented.
 * The current release includes DWGs for
 * - Cylindrical bores / fractional delays (@ref DWGcylinderModule),
 * - Cylindrical junctions (@ref DWGcylinderJunctionModule),
 * - Conical bores (@ref DWGconeModule) and
 * - Conical bore junctions (@ref DWGconeJunctionModule).
 * Moreover, several standard modules (sine wave, heaviside, amplification, etc.)
 * which are commonly used in any time-domain simulation have been implemented.
 * The most flexible module is the @ref inputFunctionModule which is able to
 * calculate the output based on any given mathematical formula and multiple inputs.
 * As by version 0.10, only the @ref sp_API "ART programmer's interface" implements
 * the time-domain simulation.
 *
 *
 * @section sec_tb Theoretical background 
 *
 * One way to describe the acoustic characteristics of wind instruments is the input 
 * impedance. The input impedance, as a function of frequency, describes which frequency
 * components will be attenuated by the instrument. It depends on the shape of the 
 * instrument and can be calculated by dividing the instrument into pieces of the 
 * following types:
 *
 * A <B>Cylinder</B> is characterised by its radius and length.
 * @image html cyl.png
 * @image latex cyl_l.pdf "Parameters of a cylinder module."
 * A <B>Bore jump</B> must be inserted, where two sections with different perimeter meet. It is
 * only characterised by radius 1 and 2. 
 * @image html bj.png
 * @image latex bj_l.pdf "Parameters of a bore jump module."
 * A <B>Cone</B> is described by its length, radius 1 and 2. The perimeter increase of the
 * perimeter is linear.
 * @image html cone.png
 * @image latex cone_l.pdf "Parameters of a cone module."
 * A <B>Bessel horn</B> is defined by its length, radius 1, radius 2 and the flare 
 * parameter. The increase of the perimeter is non-linear. Use the flare parameter to
 * change the curvature. See Braden 2006, p. 132 for detailed formulae.
 * @image html bessel.png
 * @image latex bessel_l.pdf "Parameters of a bessel horn."
 * A <B>tone hole</B> is a branch to a cylinder with an open end. Tone holes can only
 * be inserted between two other elements.   
 * @image html tonehole.png
 * @image latex tonehole_l.pdf "Parameters of a tone hole module."
 * 
 * A <B>termination element</B> can only be added at the end of the element. Use termination
 * elements to choose the radiation impedance. The radius r defines the radius of the circular
 * radiating surface. 
 * 
 * @image html termination.png
 * @image latex termination_l.pdf "Parameters of a termination module." width=0.5cm
 *
 * The cylinder and the cone are also available as bent elements. An additional property,
 * the bend radius, describes the curvature of the bend.  
 *   
 * In the programme the user can use these models to build an instrument in order to
 * calculate its input impedance.
 *  
 * @section sec_use Using the Acoustic Research Tool
 * 
 * ART comes in two flavours: As a command line tool and as a programmer's library. As a
 * command line tool ART can be embedded in the TAP framework. An executable for Windows 
 * and source code are provided. The programmer's library
 * provides an interface for implementing applications for impedance calculation. For this
 * we provide a Windows DLL and source code, which you can include in your projects. 
 * 
 *  
 * @ref sp_ACT "Using the ART command line tool"
 *	
 * @ref sp_API "Using the ART programmer's interface"	
 *   
 * @section sec_enh Enhancing the Acoustic Research Tool
 *
 * ART was designed to welcome contribitions by fellow researchers. You can implement your 
 * own elements, add different calculation models to existing elements or add simulation 
 * domains to the project. To find out more read the programmer's guide.  
 *
 * 
 * @ref sp_EAP "Acoustic Research Tool programmer's guide"
 * 
 *   
 * @section lit Literature
 *
 * - Alistair Braden: Bore Optimisation and Impedance Modelling of Brass Musical Instruments. PhD thesis, University of Edinburgh, 2006.
 * - Clemens Bernhard Geyer: <a href="http://iwk.mdw.ac.at/lit_db_iwk/download.php?id=16433" target="_blank">Time-domain Simulation of Brass and Woodwind Instruments</a>.
 * Master's thesis, Universit&auml;t f&uuml;r Musik und darstellende Kunst Wien, 2012.
 *
 * @section ss_more Find out more
 * 
 * For more information please visit the project page, where you can download some 
 * documentation, the executables and explore the source code.
 * 
 * Project Web Hosted by SourceForge.net   
 */
//----------------------------------------------------------------------------------------	
/** 
 * @page sp_ACT Using the ART command line tool
 * 
 * The ART command line tool provides a simple and quick interface for calculating the
 * impedance of musical instruments. The programme can receive input from the binary input
 * stream and sends output to the binary output stream. Use the programme in pipes with
 * other executables to provide input (a frequency range) and process output (impedance 
 * curves). Such executables can be found in the Toolbox for Analysis and Prototyping (TAP).
 * The TAP framework is a collection of small programmes that can be used to create stimulus 
 * signals like frequency sweeps over a certain frequency range and process binary output.
 * 
 * @section s_setup Setup of ART and TAP
 *  
 * @subsection ss_inst Installation 
 *  
 *  - Extract zipped archive to a local folder (e.g. "C:\TAP").
 *  - DoubleClick "C:\TAP\command completion (HKCU-NT shell).reg" to turn on filename completion with TAB-key
 *  - RightClick/Install "C:\TAP\DOSHERE.INF" to enter a "DOS Prompt Here" command to your explorer's folder 
 *	context menu
 *  - Use Settings/ControlPanel/System/Extended/Environment Variables/ to prepend "C:\TAP;" to the system 
 *	variable named "Path" (it might now be necessary to logoff and logon again to make your changes effective)
 * 
 * @subsection ss_vinst Verify the Installation
 * 
 *  - Open a command shell using the explorer's context menu of some (empty) folder.
 *  - enter "path" at the command prompt to see whether "C:\TAP" is the first entry in your path system variable.
 *  - enter "stim | envsort | plot" to check whether everything is working. A GNUPlot window with a sinewave 
 *	should open.
 * 
 * @section s_run Running the simulator
 *  
 *  The following command will simulate the input impedance of a straight tube of 100 cm length 
 *  and 0.5 cm radius and send the frequency and magnitude columns of the resulting binary 
 *  stream to the plot program. The programme will simulate the tube at equally spaced 
 *  frequencies between 50 Hz and 1000 Hz with 5 Hz increment and won't read the input stream.
 *
 *  @verbatim
art -freq 50 1000 5 -cyl 100 0.5 | plot -sig 3 -xcol 1 -ycol 2 -nam ImpMag -xu [Hz] -yu [acOhm] -tit "Tube 1m"
    @endverbatim
 *
 *
 *  To do the same and see a numerical table with three columns (f, mag, arg) instead of 
 *  plotting the binary stream, type:
 *
 *  @verbatim
art -freq 50 1000 5 -cyl 100 0.5 | bin2flo -columns 3 -nocount 
    @endverbatim
 *
 *  To do the same but calculate resonant frequencies and magnitudes instead of generating a binary plot stream:
 *
 *  @verbatim
art -freq 50 1000 5 1 -cyl 100 0.5
    @endverbatim
 *
 *  To simulate a complete trumpet described in an @ref s_instfiles "instrument file", type:
 *
 *  @verbatim
art -freq 50 1000 5 -list trumpet.ins | plot -sig 3 -xcol 1 -ycol 2 -nam ImpMag -xu Hz -yu Ohm -tit "Trumpet"
    @endverbatim
 * 
 *  @section s_instfiles Instrument file format
 *
 *  The programme defines its instruments in a top-down, object-oriented approach. The .ins
 *  file-format likewise represents instruments in this manner. An instrument consists of a
 *  list of elements describing its geometry, each of which has parameters specifying the
 *  exact dimensions.
 *
 *  @subsection ss_elements Element types
 *  \par
 *  0 - bore discontinuity (input radius, output radius, T, L, H, CO2) <BR>
 *  1 - cylinder (length, radius, T, L, H, CO2)<BR>
 *  2 - cone section (length, input radius, output radius, T, L, H, CO2) <BR>
 *  3 - Bessel horn section (length, input radius, output radius, flare coefficient, T, L, H, CO2) <BR>
 *  4 - list element (filename)   <BR>
 *  5 - cylindrical bend (length, radius, bend radius, T, L, H, CO2) <BR>
 *  6 - conical bend (length, input radius, output radius, bend radius, T, L, H, CO2) <BR>
 *  7 - termination element (type of termination [can be 0=reflecting or 1=Zorumski], T, L, H, CO2) <BR>
 *  8 - branch element (calculation model, number of holes, T, L, H, CO2) <BR>
 *
 *  where <BR>
 *
 *  T...temperature in degree centigrade (default is 21 C) <BR>
 *  L...loss factor (default is 1) <BR>
 *  H...air humidity in percent<BR>
 *  CO2...CO<sub>2</sub> content of air in ppm  <BR>
 *  The unit of length is cm.<BR>
 *
 *  @subsection ss_instspec Format specification
 *
 *  A .ins file is a list of element types followed by relevant parameters separated by commas.
 *  Whitespace is permitted, but parameters must be on the same line as the element type. Any
 *  lines starting with a semi-colon ; are ignored as comments. Comments may not be placed on
 *  the same line as commands. Lines may not begin (or consist solely of) whitespace. The first
 *  non-commented line of the file defines the name of the instrument.
 *
 *
 *  @subsection ss_instex Example file
 *  @verbatim
Test Instrument
; - this is a comment
1, 10.0, 1.5
; ^ this is a cylinder, length 10cm, radius 1.5cm
;
2, 20.0, 1.5, 2.0
; ^ this is a cone, length 20cm, radius 1.5..2.0cm
;
4, filename.ins 
; ^ this loads a list element and appends it.
    @endverbatim
 *
 *  @section s_learnmore Learn more about ART and TAP:
 *  
 *  - enter <KBD>"art -?"</KBD> for a complete list of options
 *  - type one of the following commands to learn more about other parts of TAP:
 *
 *  @verbatim
bin2flo -?
plot -?
stim -?
    @endverbatim
 *
 *  - Read the TAP reference manual (download on sourceforge project page)
 *   
 */
//----------------------------------------------------------------------------------------	
/** 
 * @page sp_API Using the ART programmer's interface 
 *
 * @section incl Including the ART interface
 *  
 * Windows programmers can use the DLL. C++ programmers can include the ART.h file.
 *	
 * @subsection dll Using the DLL
 * 
 * If you are working on windows and not using C++, you can use a DLL. The DLL can be 
 * downloaded from the sourceforge site. If you prefer to compile the DLL by yourself,
 * use the MakeARTDLL.bat make file script. Check the Delphi examples on source forge to
 * find out how to use the DLL.	
 *
 * @subsection arth Including ART.h
 * 
 * When working in C++, simply include the ART.h file into your project. Don't forget to
 * add all files of the ART package as dependencies. So far only compiling on Microsoft
 * Visual C++ 6.0 has been tested, other compilers could work. We are still working on 
 * making the code portable to other compilers and platforms. Check out the source forge
 * page if you want to get the source code.  
 *   
 * @section use Using the ART interface 
 *
 * @subsection pemh Models - Prototypes - Elements - Circuits
 *
 * It is important to grasp the difference between a model, a prototype and an element.

<B>Models</B> provide functions to prepare the calculation of their own acoustic properties. The data properties of a model are its physical features, like length, width or temperature. Other properties concern implementation details, like for which simulation domain and which wavetypes the model provides calculation functions. A model has to implement the ModelInterface.  

<B>Prototypes</B> (or "prototype models") are models, that are instantiated during the initialization of the ART root object. Each of these prototype models contains meaningful default values. Prototypes serve as templates to create other models of the same class during runtime. Theses clones are attached to elements (see below), which you can insert into circuits and use for calculation. 

<B>Elements</B> are wrappers of models and can be used for simulation. Their properties, like input impedance or radiation impedance, are dependent on their place in a circuit. Elements contain their own model object (which is a clone of a prototype model).

 * <B>Circuits</B> are arrangements of acoustic elements. When simulating a musical instrument, 
 * you will have to build a circuit to represent that instrument closely. A circuit must 
 * contain one or more elements. So far elements can only be arranged in in a one   
 * dimensional way, branches (with the notable excepetion of tone holes) are not (yet) possible. 

When calculation the impedance of a cylindrical tube, you would first create an element. When creating an element of type cylinder, the cylinder prototype is automatically cloned and the clone is attached to the element as its model. Then you would adapt the properties of the element's model, place the element into a circuit within the same simulator and evaluate the circuits impedance. When computing the impedance, the element will use the functions provided by its model.
 
 * @subsection step2a Creating the simulator
 *
 * First the user must create the ART root object. This object will contain all simulators and
 * provides us with all prototypes.  
 * Then the user has to create a simulator for a certain wave type and a certain 
 * simulation domain. So far only frequency domain and multimodal waves are supported, so
 * we will create a simulator for that and save a pointer to it:
 * @code
 * ARTRootObject(); 
 * P_ART_Simulator mySim = ARTCreateSimulator("MySimulator", "FrequencyDomain", "MultiModal"); 
 * @endcode  
 *	
 * @subsection step2 Creating the elements
 * 
 * Elements are created from prototypes and placed into a simulator by the function 
 * ARTCreateElement. The prototypes need to provide functions for simulation in the 
 * simulator's domain and wavetype. We will create a cylinder and call it "Cyl", a cone
 * called "Con" and a Bessel horn named "Bes", all in the simulator created before. 
 * @code
 * P_ART_Element Cyl = ARTCreateElement(mySim, "Cyl", "Cylinder");
 * P_ART_Element Con = ARTCreateElement(mySim, "Con", "Cone");
 * P_ART_Element Bes = ARTCreateElement(mySim, "Bes", "Besselhorn");
 * @endcode
 * 
 * @subsection step3 Customising the elements
 *	
 * To change the measurements and other parameters of the elements use the function
 * ARTSetParameter. We well change the measurements of the cylinder to a 100 cm long tube
 * with a radius of 1 cm. The cone will be 50 cm long, and have the same radius as
 * the cylinder at the beginning and 1.5 times that radius at the end. The Bessel horn 
 * will be 20 cm long, have the same radius as the cone ending and bend to a radius of 
 * 12.5 cm at the end. The parameter flare, controlling the shape 
 * of the Bessel horn is set to 0.4.
 * @code
 * ARTSetParameter(mySim, "Cyl.r = 1; Cyl.length = 100;"); 
 * ARTSetParameter(mySim, "Con.length = 50;"); 
 * ARTSetParameter(mySim, "Con.r1 = Cyl.r; Con.r2 = 1.5 * Con.r1;"); 
 * ARTSetParameter(mySim, "Bes.length = 20; Bes.r1 = Con.r2;");
 * ARTSetParameter(mySim, "Bes.r2 = 12.5; Bes.flare = 0.4;"); 
 * @endcode
 *
 * @subsection step4 Building the instrument
 * 
 * The order in which the elements are added is
 * important. The first element will be the element nearest to the mouth of the player. 
 * Use the functions ARTAppendReferenceBefore and ARTAppendReferenceAfter if you want to
 * insert elements at a certain place. We will first create an instrument and then
 * add our customised elements one by one.
 * 
 * @code
 * P_ART_Circuit myIns = ARTCreateCircuit(mySim, "MyInstrument");
 * ARTAppendReference(myIns, Cyl);
 * ARTAppendReference(myIns, Con);
 * ARTAppendReference(myIns, Bes);
 * @endcode
 *	
 * @subsection step5 Calculating the instrument's impedance
 * 
 * Before calculating the impedance the frequency range and the number of modes have to be
 * defined. The impedance is evaluated for discrete points within the range. The distance
 * between these points is determined by the frequency step. We will set the range of the
 * simulator to be from 50 Hz to 1800 Hz, with a frequency step of 5 Hz. We set the number 
 * of modes to 1.
 * 
 * @code	
 * ARTSetFrequencyRange(mySim, 50, 1800, 5);
 * ARTSetNModes(mySim, 1);
 * @endcode
 *
 * To calculate a circuit's impedance we simply use the function ARTInputImpedance. The 
 * impedance curve returned won't change if the arrangement is changed after the function
 * is called. A new call ensures the impedance curve is up-to-date. The function returns
 * a pointer to an object of class DataContainer, which holds the data.
 *
 * We will have the impedance of our instrument calculated and print it as numbers to the
 * screen.
 * 
 * @code
 * P_ART_Variant myImpCurve = ARTGetValue( ARTInputImpedance(myIns) );
 * for (int i=0; i<ARTGetLength(myImpCurve); i++)
 * {  
 *    T_ART_Tripl tri = ARTGetTriple(myImpCurve, i); 
 *    std::cout << tri.f << "Hz\t" 
 *	            << tri.re << "\t" 
 *	            << tri.im << "i\n";
 * } 
 * @endcode
 * 
 * @subsection step6 Cleaning up
 * 
 * When you are done you must destroy all elements and simulators you create and
 * also the ART root object: 
 *    
 * @code	
 * ARTDestroyCircuit(mySim, myIns);
 * ARTDestroyElement(mySim, Cyl);
 * ARTDestroyElement(mySim, Con);
 * ARTDestroyElement(mySim, Bes);
 * ARTDestroySimulator(mySim);
 * ARTRootDestroy();
 * @endcode
 * 
 * @section sec3 Error handling
 * 
 * When errors are encountered, the interface either throws an exception as an object of    
 * the class ARTerror (when not using the DLL) or returns a bad value through a function 
 * (when using the DLL).
 * 
 * When not using the DLL, include critical operations in try/catch blocks, like this:
 * @code
 * try
 * {
 *	ARTRootObject();
 *	P_ART_Simulator mySim = ARTCreateSimulator("MySim", "FrequencyDomain", "MultiModal");
 *	ARTCreateElement(mySim, "Zylinder1", "Cylinder");
 *	ARTSetParameter(mySim, "Cyl.R = 10;"); //Error! There's no element called "Cyl"!
 * }
 * catch(ARTerror e)
 * {	
 *	string err = e.GetErrorMessage();
 *	std::cout << err; 
 *	//Will print: 
 *	//"Error in function 'ARTSetParameter': An element of the specified name 'Cyl' does not exist."
 * }
 * @endcode  
 * Of course it does not harm to include all operations in try/catch blocks. At this stage
 * of the project that can be useful for debugging. Note that most error messages so far are 
 * indeed rather for debugging and not meant for end-users.
 *
 * @see ARTerror   
 * @see ARTGetLastErrorMessage to find out more about error handling when using the DLL. 
 *   	
 * @section sec2 Technicalities
 *
 * @subsection susec1 The dependency tree
 *   
 * When the impedance is evaluated, first all parameters it depends on are evaluated. If
 * those parameters depend themselves on other parameters, they are evalauted before. All
 * parameters are in a dependency tree, which the programme automatically follows to its
 * principle elements, those that do not depend on any other parameter. In our example the
 * dependency tree looks as follows:
 * 
 * @image html dependencies.png
 * @image latex dependencies_l.pdf
 *   
 * So when the impedance is queried, all other properties are evaluated first. 
 * Therefore the user never has to worry about evaluating other properties before calling
 * ARTInputImpedance.
 * 
 * Properties keep track of their validity. As soon as a dependency of a property is 
 * changed, the property is marked for recalculation the next time it is needed.	
 *   
 * @subsection susec4 Progress information and computation cost estimation
 * 
 * The user can specify a callback function, which will be called at regular intervals 
 * during the calculation process to communicate information about the progress. This 
 * function can also be used to abort the calculation. Before the calculation starts, the
 * interface determines the cost of the computation in computation steps, that is parser or functionoid calls 
 * respectively. Since not all of these operations are of the same complexity the first
 * thousand computation steps can be twice as fast as the following thousand.   
 * 
 * \htmlonly 
 * @see ARTSetProgressFunction
 * \endhtmlonly 	
 *
 * @section sec4 Detailed description
 * 
 * Check the @ref ARTinterface_group "documentation of the ART interface" for a detailed description of its functions.
 */ 
//---------------------------------------------------------------------------------------- 
/** 
 * @page sp_EAP ART Programmer's Guide 
 *
 *  
 * @section pg_intro Introduction
 * 
 * This is the top level page of the source code documentation. 
 * These pages are the documentation of ART's sources. If you want to contribute to the 
 * project you will find useful background information here. Some sections of the user's
 * manual may also bring you to these pages, since some details of ART's functionality are
 * explained here. 
 *  
 * If you only want to use ART without extending its functionality, have a look at these
 * pages: 
 *          
 * @ref sp_ACT "Using the ART command line tool"
 *  
 * @ref sp_API "Using the ART programmer's interface"	
 *
 * @section pg_overview Overview
 * 
 * The source code can be divided into several units:
 * 
     - The @ref ARTinterface_group "ART programmer's interface" provides simple functions which allow you to build your 
       own simulation applications. 
   
     - The @ref ARTimpl "ART objects" are the objects the programmer's interface is built on. You can
       also build your own applications by using these objects.   
   
     - The @ref ModelInterfaceGroup "ART model interface" is being developped to provide an easy way of adding custom
       models to ART.
   
     - The @ref waveObjectInterfaceGroup "wave interface" standardizes a way of converting different types of waves as
       they are required by the models. 
 *  
 *
 * @section enhancing Enhancing the Acoustic Research Tool 
 * 
 * @subsection calc_Imp Behind the scenes of ARTInputImpedance
 * 
 * This section was written to give you an idea about how the calculation 
 * actually works, which is essential if you plan to integrate your own models in ART.
 * 
 * Do you remember the difference between elements, models and prototypes? Then 
 * you might also remember that every element has its own model attached to it 
 * and that every model implements the ModelInterface. Now what happens when 
 * we call ARTinputImpedance?
 * 
 * ARTInputImpedance calls the method PrepareCalculation of the specified 
 * circuit. That function uses the interface functions of the models of each 
 * element to prepare the calculation. The outermost element is queried for its 
 * radiation impedance. The radiation impedance is then propagated through all 
 * elements until the input impedance at the mouthpiece has been found.
 * 
 * It is important to note that calculation does not take place in this step. In 
 * fact, the circuit prepares the calculation by setting up relations between the 
 * elements and laying down how the values of all the propagated impedances have 
 * to be calculated.
 * 
 * Let's assume we want to calculate the input impedance of a simple model, 
 * consisting of a cylinder (the mouth piece) a cone and a Bessel horn at the 
 * outer end (the "radiating" end). In our simple model the circuit tells the Besselhorn element to provide a 
 * waveobject representing its radiation impedance. This waveobject is still 
 * empty when passed to the Besselhorn to get a waveobject representing the 
 * propagated impedance. The element only notes which waveobject to propagate 
 * when calculation is finally carried out. The new waveobject is passed to the 
 * cone, which returns a representation of the propagated impedance, which is 
 * again passed on to the cylinder to get the impedance propagated through the 
 * last element (or the first, from the player's perspective). This waveobject 
 * will contain the input impedance.
 * 
 * @image html instrument_waveobj.png
 * @image latex instrument_waveobj_l.pdf
 * 
 * When these relations are set up, the circuit is ready for calculation. The 
 * input impedance is queried for its value and since it is not up-to-date, it 
 * initiates its own computation. By asking each dependency for its value it 
 * initiates their computation. This sets off a chain reaction during which all 
 * waveobjects are evaluated, starting from the radiation impedance and ending 
 * with the input impedance of the circuit, which was queried in the beginning.
 * 
 * @image html waveobjects_evaluation.png
 * @image latex waveobjects_evaluation_l.pdf
 * 
 * A little but important detail are the waveobjects themselves. Waveobjects 
 * contain the wave in some form (as impedance per frequency, as pressure per 
 * frequency, as an impulse in the time domain, etc.) and implement the 
 * WaveObjectInterface. The interface declares functions which the models can use 
 * to ensure the wave is provided in a certain form, the waveobjects need to 
 * prepare this conversion.
 * 
 * The calculations themselves are carried out by functionoids. If you are not 
 * familiar with the concept of functionoids, imagine them to be deep-frozen 
 * function calls. That is, all necessary arguments are passed to a functionoid's 
 * constructur. The constructor saves references or pointers to those arguments 
 * but does not compute anything. Only when the functionoid's core function 
 * (we've called it applyFunction) is called, the calculation is executed.
 * 
 * The reason for introducing these concepts is efficiency. The ART interface was 
 * written with different types of applications in mind, and in some, like 
 * optimizers, it is common to reevaluate an instrument's input impedance 
 * frequently after doing only minor changes. Impedance calculations of whole 
 * instruments can be very time consuming, so we want to avoid recalculating 
 * values which have no reason to change. If, for example, in our simple example 
 * instrument only the length of the cylinder changed, we would not need to 
 * recompute the Bessel horn's or the cone's impedance, but only propagate the 
 * input impedance at the entrance of the cone through the changed cylinder.
 * 
 * @subsection ownModel How to implement your own model
 * 
 * To implement your own model, derive a class from ModelInterface and 
 * implement the declared functions. If a model is not supposed to support a 
 * certain functionality, throw an exception (of type ARTerror) in the function 
 * to inform users and programmers, that the requested functionality is not 
 * available.
 * 
 * Methods like InputImpedance or RadiationImpedance are supposed to *prepare* 
 * waveobjects for calculation. Therefore, a model must implement the calculation 
 * itself in functionoids. Only the constructors of these custom functionoids 
 * should be called in the methods derived from ModelInterface.
 * 
 * Look at the models implemented so far (HornElementPrototype_FD and ToneHole) 
 * for examples. Both classes are wrappers for other objects of class HornElement 
 * and use the HornElement-object's methods to calculate acoustic impedance. Note 
 * how the functionoids are used in the two classes.
 * 
 * @subsection useOwnModel How to use your own model
 * 
 * To use your own model and make it useable for others, you have to construct a 
 * protoype with your model. Set properties of the prototype in its constructor. 
 * Check the ToneHole constructor to see an example.
 * 
 * That prototype must be added to the prototpeModels list of the ART root object 
 * in the root object's constructor. That is as simple as this:
 * 
 * @code 
 *   obj = prototypeModels->AppendObject(new ToneHole()); 
 * @endcode
 * 
 * The prototype will then be available as a template for elements. When the root 
 * object is detroyed, the prototype will also be destroyed (properties, objects 
 * and methods appended to an ARTobject are always destroyed when the object is 
 * destroyed).
 * 
 * If you can not specify all properties of a prototype in its constructor (for 
 * example when creating a template class to represent a prototype that will have 
 * different properties depending on the template argument) you can add the 
 * properties after instantiation of the prototype object in the root object's 
 * constructor. The cylinder, for example, is created like that:
 * 
 * @code 
 *   obj = prototypeModels->AppendObject(
 *     new HornElementPrototype_FD<CylinderSection>("Cylinder", "sound propagation in a cylindrical duct")
 *     ); 
 *   //physical properties with values are data properties 
 *   obj->AppendDataProp("length", 100.0, "axial length"); 
 *   obj->AppendDataProp("r", 1.0,"duct radius"); 
 *   obj->AppendDataProp("temp", 21.0,"local temperature"); 
 *   obj->AppendDataProp("loss", 1.0,"loss factor (defaults to 1)"); 
 *   //what the cylinder can do is specified by its properties
 *   obj->AppendMethod("SphericalWave", "MapesRiordan Model"); 
 *   obj->AppendMethod("PlaneWave", "first mode only"); 
 *   obj->AppendMethod("MultiModal", "modal decomposition up to n modes (Kemp02)"); 
 *   obj->AppendMethod("FrequencyDomain", "simulation in frq domain"); 
 *   obj->AppendProperty("canPropagate","this element can propagate impedance"); 
 *   obj->AppendProperty("canRadiate","this radiation impedance of this element can be calculated"); 
 * @endcode
 * 
 */
//----------------------------------------------------------------------------------------	
/** 
 * @page sp_about About ART
 * 
 * @section about_proj The Project
 *   
 * <p>Acoustic Research Tool (ART) - A Simulation Framework with Modelling Library for Acoustic Systems</p>	
 *	
 * <p>Project of the <a href="http://www.eaa-fenestra.org/technical-committees/ma/workgroups/wg2">Workgroup 2</a> 
 * of the Technical Committee Musical Acoustics of the <a href="http://www.eaa-fenestra.org/">European Acoustics Association (EAA)</a>
 * </p>	
 *	
 *  <p>	
 *  Copyright (C) 2011 by the authors and their organisations:<br />	
 *	Alistair Braden	<sup>1)</sup><br />	
 *	Wilfried Kausel	<sup>2)</sup><br />	
 *	Delphine Chadefaux	<sup>2)</sup><br />	
 *	Vasileios Chatziioannou	<sup>2)</sup><br />	
 *	Sadjad Siddiq	<sup>2)</sup><br />
 *	Clemens Geyer	<sup>2)</sup><br />
 *	<br />	
 *	
 *	1) <a href="http://www.ph.ed.ac.uk/">School of Physics and Astronomy, Univ. of Edinburgh, GB</a>	
	<br />	
 *	2) <a href="http://iwk.mdw.ac.at">Inst. of Music Acoustics, Univ. of Music, Vienna, AT</a>	
	<br />	
 *  </p>
 *	
 *  <p>This program is free software: you can redistribute it and/or modify   
 *  it under the terms of the <a href="http://www.gnu.org/licenses/">GNU General Public License</a> as published by   
 *  the Free Software Foundation, either version 3 of the License, or	
 *  any later version.</p>	
 *	
 *  <p>This program is distributed in the hope that it will be useful,	
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of	
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the	
 *  GNU General Public License for more details.</p>	
 * 
 * @section about_researchers Researchers contributing to the project
 *
 * In 2002 <b>Jonathan Kemp</b> published his thesis "Theoretical and experimental study of wave
 * propagation in brass musical instruments" (Univ. of Edinburgh) containing a
 * comprehensive review of the theory of multimodal wave propagation and extending it to
 * include ducts with rectangular cross-section (ready to be implemented).
 *
 * The basic concepts for the simulation framework of the ART project have been presented
 * at the ViennaTalk 2005 by <b>Wilfried Kausel</b>, who founded the WG2 at the subsequent TCMA
 * meeting in Budapest in order to trigger an international collaboration for the
 * collection of existing and future physical models with the aim of integrating them into
 * a common framework to make them more generally available and useful.
 *
 * In 2006 <b>Alistair Braden</b> contributed the first C++ code as part of his thesis "Bore
 * Optimisation and Impedance Modelling of Brass Musical Instruments" (Univ. of
 * Edinburgh). He extended the theory of multimodal wave propagation to include bent
 * cylindrical tubes and he implemented straight and bent cylinders and cones, bessel
 * horns, exponential sections and bore discontinuities. His code was ported to Windows by
 * Kausel who also optimised its performance and included local simulation parameters like
 * temperature and boundary loss factor. He also started to create the application
 * interface layer by adding the parameter dependency tree and by implementing the "self-
 * documentation feature" of all available models and parameters.
 *
 * During an internship at the University of Music in Vienna <b>Delphine Chadefaux</b> added
 * branch elements (tone holes) and she implemented the "matrix accumulation concept" as
 * an optional alternative to the "impedance back-propagation concept" which turned out to
 * be numerically more stable and which allows to cache the system matrix of a bigger
 * section of the bore profile (e.g. the bell) when this is not to be modified during
 * series of impedance recalculations. She also improved the model for the speed of sound
 * in air taking relative humidity and carbon dioxide content into account.
 *
 * In 2009 <b>Vasileios Chatziioannou</b> became a member of the acoustic research team at the
 * University of Music in Vienna and he is now in charge of the simulation kernel. He
 * improved the code significantly by fixing many remaining bugs, straightening out and
 * documenting the code. He also added the thermo-viscous loss model of straight tubes to
 * the bent tube and cone sections which were lossless before.
 *
 * In 2011 <b>Sadjad Siddiq</b> was working on the programmers' interface. He
 * implemented the caching algorithm, integrated the expression parser, introduced the wave
 * objects and models interface and compiled the documentation. 
 *
 * In 2012 <b>Clemens Geyer</b>, a master level student at the University of Music in Vienna, was
 * responsible for reestablishing the GCC- and Linux compatibility and
 * added time domain processing to the simulation core.
 *
 *
 *	
 */

#ifndef ART_H
#define ART_H

#include "ARTlink.h"

#if defined(_MSC_VER)
#define __CALLCONV __cdecl
#if defined(DLL)
#define __DECLSPEC __declspec(dllexport)
#else
#define __DECLSPEC
#endif /* DLL */
#else
#define __CALLCONV
#define __DECLSPEC
#endif /* _MSC_VER */


#ifndef __cplusplus
/* C struct declarations */
  typedef struct Cell Cell;
  typedef struct Circuit Circuit;
  typedef struct DataContainer DataContainer;
  typedef struct Element Element;
  typedef struct Simulator Simulator;
  typedef struct Property Property;
  typedef struct ITimeModule ITimeModule;
  typedef struct ARTobject ARTobject;
  typedef struct CalculationMethod CalculationMethod;
  typedef struct ListProp ListProp;
  typedef struct ARTvariant ARTvariant;
  typedef struct DataProp DataProp;
typedef int bool;

#else
/* CPP class declarations */
namespace ART{
  class Cell;
  class Circuit;
  class DataContainer;
  class Element;
  class Simulator;
  class Property;
  class ListProp;
  class ITimeModule;
  class ARTobject;
  class CalculationMethod;
  class ARTvariant;
  class DataProp;
}
/*
#include "Interface.h"1
#include "strparsing.h"
*/
using namespace ART;
#endif /* __cplusplus*/

typedef const char*	pChar;
typedef Cell*	P_ART_Cell;
typedef Circuit*	P_ART_Circuit;
typedef DataContainer*	P_ART_DataContainer;
typedef Element*	P_ART_Element;
typedef Simulator*   P_ART_Simulator;
typedef Property*	P_ART_Property;
typedef ListProp*	P_ART_ListProp;
typedef ITimeModule* P_ART_TModule;
typedef ARTobject*	P_ART_Object;
typedef CalculationMethod*	P_ART_Method;
typedef ARTvariant*	P_ART_Variant;
typedef DataProp*	P_ART_DataProp;


typedef bool (* TprogressFunction)(double, const char*);

//typedef int*	__CALLCONV	TStatus_Indication(char* msg, int completion);


//TStatus_Indication* Status_Indication = NULL;


#ifdef __cplusplus
extern "C" {
#endif

__DECLSPEC int	__CALLCONV	begin_trace	(const char* filename);
__DECLSPEC int	__CALLCONV	end_trace	(int dummy);
//int	__CALLCONV	store_callback	(TStatus_Indication func);

//DLL-Interface methods, must be registered in ART.def!!! 
// (see http://www.functionx.com/visualc/libraries/moduledef.htm)

/** \addtogroup ARTinterface_group ART programmer's interface 
 * The functions and types in this module constitute the top level interface for the Acoustic
 * Research Tool. Use them to write applications of your own.  
 *  @{
 */

// For creating and destroying root object (type: AcousticResearchTool <- ARTObject)
// All elements like cylinders etc. have to be part of a root object like this.
/**
 * Create the ART root object. This object contains all prototypes, simulators, methods 
 * and objects. It is created automatically in most functions and a pointer is stored 
 * internally. Do not call this method yourself.
 * @returns a pointer to the root object created. 
 */ 
__DECLSPEC P_ART_Object	__CALLCONV	ARTRootObject	();

/**
 * Destroy the ART root object. Call this function before exiting your programme to clean
 * up memory. 
 */ 
__DECLSPEC bool	__CALLCONV	ARTRootDestroy	();

/**
 * Define a callback function that is frequently informed about the process 
 * of the calculation.
 * 
 * @param f  A pointer to the callback function. The function must be of type 
 *	TprogressFunction. A floating point number of type double is passed to the callback 
 *	function. The number indicates the progress of the current calculation. 0 means no 
 *	progress, 1 means complete. If the return value of the callback function is true, 
 *	the calculation is continued, otherwise it is aborted by throwing an ARTabort 
 *	exception, which must be catched by the user. 
 * @returns void  
 * @see TprogressFunction	
 *	
 */
__DECLSPEC bool	__CALLCONV	ARTSetProgressFunction	(TprogressFunction f);

/**
 * Check if a property contains a capability.
 * 
 * @param property The property checked for the capability.
 * @param capability A string representing the capability to be checked for.
 * @returns true if the capability is found.
 * @throws ARTerror	
 */ 
__DECLSPEC bool	__CALLCONV	ARTCheckPropertyCapability	(const char* property, const char* capability);

/** 
 *  Use this method to create a simulator. A simulator belongs to a domain of simulation 
 *  in which waves of one type are calculated. It contains models of acoustic elements and 
 *  arrangements of such models.
 *   
 *  @param name The name of the simulator.
 *  @param domain The domain of the simulation. So far only frequency domain simulation is 
 *	possible, so this value must be "frequency".
 *  @param wavetype The type of the simulated waves. Query the root object for possible 
 *	values.
 *  @returns A reference to the created simulator. Using this pointer instead of the 
 *	string "name" to refer to the simulator is faster and should be favoured.
 *  @throws ARTerror 
 */
  __DECLSPEC P_ART_Simulator	__CALLCONV	ARTCreateSimulator	(const char* name, const char* domain, const char* wavetype);


/**
 * Destroys a simulator.
 * @param simulator A pointer to the simulator to be destroyed.
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
  __DECLSPEC bool	__CALLCONV	ARTDestroySimulator	(P_ART_Simulator simulator);


/**
 *  Set the frequency range of the simulator.
 *  @param sim The simulator whose frequency should be set.
 *  @param f_min The lower frequency limit in Hz.
 *  @param f_max The higher frequency limit in Hz.  
 *  @param f_step The frequency resolution used in the frequency range, in Hz.  
 *  @returns true if successful, the DLL interface returns false if an error occurs,
 *    use ARTGetLastErrorMessage to get the error message.
 *  @throws ARTerror if not using the DLL interface.
 */ 
  __DECLSPEC bool	__CALLCONV	ARTSetFrequencyRange	(Simulator* sim, double f_min, double f_max, double f_step);

/**
 * Sets the number of modes of a simulator.
 * @param sim The simulator whose number of modes should be set.
 * @param Nmodes The number of modes.
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
  __DECLSPEC bool	__CALLCONV	ARTSetNModes	(Simulator* sim, int Nmodes);


/**
 * Finds an element with a certain name in the simulator specified.
 */ 
  __DECLSPEC P_ART_Element    __CALLCONV ARTFindElement     (P_ART_Simulator simulator, const char* name);

/**
 * Finds an circuit with a certain name in the simulator specified.
 */ 
  __DECLSPEC P_ART_Circuit    __CALLCONV ARTFindCircuit     (P_ART_Simulator simulator, const char* name);

/**
 * Creates an element of a certain prototype model.
 * @param simulator A pointer to the simulator in which the element should be created.
 * @param name The name of the new element.
 * @param type The name of the prototype model.
 * @returns A pointer to the element. The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC P_ART_Element	__CALLCONV	ARTCreateElement	(P_ART_Simulator simulator, const char* name, const char* type);
//P_ART_Element	__CALLCONV	ARTCreateElement2	(P_ART_Simulator simulator, char* name, char* type);

/**
 * Changes the model of an element.
 * @param simulator A pointer to the simulator in which the element was created.
 * @param element A pointer to the element.
 * @param type The name of the prototype model you want the element to change to.
 * @returns A pointer to the element. The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
  */ 
  __DECLSPEC P_ART_Element	__CALLCONV	ARTChangeElementModel	(P_ART_Simulator simulator, P_ART_Element element, const char* type);
//P_ART_Element	__CALLCONV	ARTChangeElementModel	(P_ART_Simulator simulator, char* name, char* type);

/**
 * Returns the model of an element. Use this function when you want to get information about an element's model to
 * get a pointer to the model which you can pass on to other functions like ARTGetDataProperties.
 * @param element A pointer to the element whose model should be retrieved.
 */
  __DECLSPEC P_ART_Object	__CALLCONV	ARTGetModel	(P_ART_Element  element);

/**
* Changes the name of an element or circuit.
* Note that expressions referring to this element like a parameter definition along the lines of "MyEl.R * 2"
* are not changed. If you changed the name of element "MyEl" to "Cyl", you have to redefine the defenition.
* @param element A pointer to the element or circuit to be renamed.
* @param newName The new name.
* @returns The element renamed. When using the DLL this function will return NULL if an error occurs.
*    Use ARTGetLastErrorMessage to get the error message.
*/
  __DECLSPEC P_ART_Element    __CALLCONV ARTChangeName     (P_ART_Element element, const char* newName);

/**
 * Destroys an element.
 * @param simulator A pointer to the simulator to which the element belongs.
 * @param element A pointer to the element to be destroyed. 
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
  __DECLSPEC bool	__CALLCONV	ARTDestroyElement	(P_ART_Simulator simulator, P_ART_Element element);
//DLL needs unique match, so only one function with this name
//void	__CALLCONV	ARTDestroyElement	(P_ART_Simulator simulator,char* name);

/**
 * Sets the value of parameters.
 * 
 * @param simulator The simulator to which the parameters or their models belong. 
 * @param command A list of commands to set the values of parameters. If more than one 
 *	command, the commands must be seperated by semicolons. 
 *	A data property P of a model of an element E can always be refered to by "E.P". To
 *	change the radius of the model of the element "MyCylinder" to 12 cm, write:
 *	MyCylinder.R = 12
 *	You can also use other properties to define one property, for example:
 *	ThyCylinder.R = MyCylinder.R * 2
 *  Properties of the simulator can be accessed directly. To set the number of modes, write:
 *  NumberOfModes = 32
 *	Some errors like circular references will only be detected when evaluating the property.
 * @returns A pointer to the last parameter defined in the command string. The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */ 
  __DECLSPEC P_ART_DataProp	__CALLCONV	ARTSetParameter	(P_ART_Simulator simulator, const char* command);


/**
 * Creates an acoustic circuit.
 * @param simulator The simulator within which the circuit should be created. 
 * @param name The name of the circuit.
 * @returns A pointer to the created circuit. The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC P_ART_Circuit	__CALLCONV	ARTCreateCircuit	(P_ART_Simulator simulator, const char* name);

/**
 * Destroys a circuit.
 * @param simulator A pointer to the simulator to which the circuit belongs.
 * @param circuit A pointer to the circuit to be destroyed. 
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
  __DECLSPEC bool	__CALLCONV	ARTDestroyCircuit	(P_ART_Simulator simulator,P_ART_Circuit circuit);
//void	__CALLCONV	ARTDestroyCircuit	(char* name);
//DLL needs unique match, so only one function with this name


/**
 * Creates a time simulation module.
 * @param simulator The simulator within which the circuit should be created.
 * @param name The name of the time module.
 * @param type The type of the time module. All available types are registered as prototypes in the root object.
 * @returns A pointer to the created time module. The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC P_ART_TModule __CALLCONV ARTCreateTModule	(P_ART_Simulator simulator, const char* name, const char* type);

/**
 * Destroys a time simulation module.
 * @param simulator A pointer to the simulator to which the circuit belongs.
 * @param module A pointer to the time module to be destroyed.
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC bool __CALLCONV ARTDestroyTModule	(P_ART_Simulator simulator, P_ART_TModule module);


/**
 * Adds a new output port to the given time module.
 * @param module A pointer to the time module to which the port will be added.
 * @param name Name of the new output port.
 * @param expr The calculation expression of the new output port.
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
__DECLSPEC bool __CALLCONV ARTAddOPortToTModule	(P_ART_TModule module, const char* name, const char* expr);

/**
 * Sets the function output port of the given function module.
 * @param module A pointer to the time module of which the port will be defined.
 * @param len Length of the output port.
 * @param expr The calculation expression of the output port, may be empty if values are
 *             initialized manually.
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
__DECLSPEC bool __CALLCONV ARTSetOPortOfFModule	(P_ART_TModule module, int len, const char* expr);

/**
 * Adds a new local parameter to the given time module.
 * @param module A pointer to the time module to which the parameter will be added.
 * @param name Name of the new parameter.
 * @param expr The calculation expression of the new parameter.
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
__DECLSPEC bool __CALLCONV ARTAddLocalParamToTModule	(P_ART_TModule module, const char* name, const char* expr);

/**
 * Adds a new global parameter to the given time simulator.
 * @param simulator A pointer to the simulator to which the parameter will be added.
 * @param name Name of the new parameter.
 * @param expr The calculation expression of the new parameter.
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC bool __CALLCONV ARTAddGlobalParamToTSimulator	(P_ART_Simulator simulator, const char* name, const char* expr);

/**
 * Connects the ports of the given modules.
 * @param simulator A pointer to the simulator containing the related time modules.
 * @param expr The calculation expression of the port connection. Example: "module1.in = module2.out".
 * @returns true if successful, the DLL interface returns false if an error occurs,
 *   use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC bool __CALLCONV ARTConnectPorts	(P_ART_Simulator simulator, const char* expr);

/**
 * Connects the ports of the given modules.
 * @param module A pointer to the time module containing the specified port.
 * @param name The name of the port.
 * @returns The corresponding port object or NULL if an error occurs. Use ARTGetLastErrorMessage to get
 *   the error message.
 * @throws ARTerror if not using the DLL interface.
 */
__DECLSPEC P_ART_DataProp __CALLCONV ARTGetPortFromTModule	(P_ART_TModule module, const char* name);

/**
 * Calculates the output value of the given port at the specified time index.s
 * @param port Port object, of which the timing value shall be calculated.
 * @param idx Time index of the interested value.
 * @returns T_ART_Cmplx value or <0,0> in case of an error.
 * @throws ARTerror if not using the DLL interface.
 */
__DECLSPEC T_ART_Cmplx __CALLCONV ARTGetComplexFromPort(P_ART_DataProp port, int idx);

/**
 * Finds the element in the circuit and returns is position. The first element (nearest to
 * mouth piece) of the circuit is at position 0.
 * @param circuit A pointer to the circuit.
 * @param element Pointer to the element whose position is queried.
 * @returns the position of the element or -1 if the element was not found. (DLL-interface: returns -1 also if another error occured)	
 * @throws ARTerror if not using the DLL interface
 */
  __DECLSPEC int	__CALLCONV	ARTGetReferencePosition	( P_ART_Circuit circuit, P_ART_Element element);
//int	__CALLCONV	ARTGetReferencePosition	(P_ART_Simulator simulator, P_ART_Circuit circuit, char* name);


/**
 * Append the reference to an element to the end of a circuit.
 * @param circuit A pointer to the circuit.
 * @param element A pointer to an element or circuit to be added to this circuit.
 * @returns the element appended.	The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC P_ART_Object	__CALLCONV	ARTAppendReference	(P_ART_Circuit circuit, P_ART_Element element);
//P_ART_Object	__CALLCONV	ARTAppendReference	(P_ART_Simulator simulator, P_ART_Circuit circuit, char* name);

/**
 * Insert the reference to an element before another element into a circuit.
 * @param circuit A pointer to the circuit.
 * @param referenceAfter A pointer to the reference before which the element should be inserted.
 * @param reference A pointer to an element or circuit to be inserted into this circuit.
 * @returns the element appended. The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC P_ART_Object	__CALLCONV	ARTAppendReferenceBefore	(P_ART_Circuit circuit, P_ART_Element referenceAfter, P_ART_Element reference);
//P_ART_Object	__CALLCONV	ARTAppendReferenceBefore	(P_ART_Simulator simulator, P_ART_Circuit circuit, char*elementAfter, char* name);

/**
 * Insert the reference to an element after another element into a circuit.
 * @param circuit A pointer to the circuit.
 * @param referenceBefore A pointer to the reference after which the element should be inserted.
 * @param reference A pointer to an element or circuit to be inserted into this circuit.
 * @returns the element appended.	The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC P_ART_Object	__CALLCONV	ARTAppendReferenceAfter	(P_ART_Circuit circuit, P_ART_Element referenceBefore, P_ART_Element reference);
//P_ART_Object	__CALLCONV	ARTAppendReferenceAfter	(P_ART_Simulator simulator, P_ART_Circuit circuit, char*elementBefore, char* name);

/**
 * Remove a reference to an element from a circuit.
 * @param circuit A pointer to the circuit.
 * @param reference Pointer to the reference to be removed.
 * @returns the number of elements removed, 0 if no element was removed (or if 
 *   there was an error when using the DLL interface. Use ARTGetLastErrorMessage 
 *   to get the error message.)
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC int	__CALLCONV	ARTRemoveReference	(P_ART_Circuit circuit, P_ART_Element reference);
//int	__CALLCONV	ARTRemoveReference	(P_ART_Simulator simulator, P_ART_Circuit circuit, char* element);

/**
 * Replace a reference to an element with references to another element in a circuit.
 * @param circuit A pointer to the circuit.
 * @param search Pointer to the reference to be replaced.
 * @param replace Pointer to the reference to replace search.
 * @returns the number of elements replaced, 0 if no element was replaced (or if 
 *   there was an error when using the DLL interface. Use ARTGetLastErrorMessage 
 *   to get the error message.)	
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC int	__CALLCONV	ARTReplaceReference	(P_ART_Circuit circuit, P_ART_Element search, P_ART_Element replace);
//int	__CALLCONV	ARTReplaceReference	(P_ART_Simulator simulator, P_ART_Circuit circuit, char*search, char* replace);

/**
 * Remove all references to elements from a circuit.
 * @param circuit A pointer to the circuit.
** @returns the number of elements removed, 0 if no element was removed (or if 
 *   there was an error when using the DLL interface. Use ARTGetLastErrorMessage 
 *   to get the error message.)   
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC int	__CALLCONV	ARTRemoveAllReferences	(P_ART_Circuit circuit);

/**
 * Returns the input impedance of a circuit. The impedance is evaluated as the circuit is
 * at the moment of the function call. If the circuit is changed the function has to be
 * called again to recalculate the impedance. Note that the value returned is a pointer 
 * to a property of the circuit. That means if you evaluate the impedance, store the pointer
 * in a variable and then reevaluate the impedance after changing the circuit, the old pointer you
 * have stored will point to the same data as the new pointer. If you need to compare impedances
 * of the same circuit, you must make copies.
 *	
 * @param circuit A pointer to the circuit whose input impedance should be returned.
 * @returns The input impedance of the circuit as a pointer to an ARTvariant of type C_ART_ntri
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC P_ART_DataProp	__CALLCONV	ARTInputImpedance	(P_ART_Circuit circuit);

/**
 * Returns the name of an object.
 * @param pobj A pointer to the object whose name should be returned.  
 * @returns The name of the object. *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC const char*	__CALLCONV	ARTGetName	(P_ART_Cell  pobj);

/**
 * Returns the short description of an object.
 * @param pobj A pointer to the object whose short description should be returned.  
 * @returns The short description of the object.*    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC const char*	__CALLCONV	ARTGetShortDescription	(P_ART_Cell  pobj);

/**
 * Returns the long description of an object.
 * @param pobj A pointer to the object whose long description should be returned.  
 * @returns The long description of the object.
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC const char*	__CALLCONV	ARTGetLongDescription	(P_ART_Cell  pobj);

/**
 * Returns the help filename of an object.
 * @param pobj A pointer to the object whose help filename should be returned.  
 * @returns The help filename of the object.	
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
  */
  __DECLSPEC const char*	__CALLCONV	ARTGetHelpFilename	(P_ART_Cell  pobj);

/**
 * Returns true if the property is listable. If the property given is NULL, false is returned.
 * @param pprp A pointer to the property
 * @returns true if the property is listable.
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */
  __DECLSPEC bool	__CALLCONV	ARTIsListable	(P_ART_Property pprp);


/**
 * Data properties can have a defined range. This function returns a pointer to an ARTvariant structure if a range
 * for the data property is defined. Ranges are of one of the array types. Numerical ranges are defined by a minimum
 * value at index 0 and a maximum value at index 1. The user has to ensure that the value of the data property does
 * not violate this bounds otherwise errors will be generated when processing the data container (but not when setting
 * the value!). For string types, the range data structure contains a list of possible strings if the set of values is 
 * restricted.
 *@param pprp The data property whose range should be recieved.
 * @returns The range of the data property or null if no range is defined or (only when using the DLL) if there was an error.
 */
__DECLSPEC P_ART_Variant   __CALLCONV ARTGetRange           (P_ART_DataProp pprp);

/**
 * Properties and data properties are saved in the same lists in all objects. When you are not sure if a property retrieved is a
 * data property or not, use this function to find out.
 * @param pprp The property to be examined.
 * @returns true if pprp is a data property; false if it is a property without a value field. The DLL interface also returns false
 * if an error was detected. Use ARTGetLastErrorMessage to get the error message.
 */
  __DECLSPEC bool	__CALLCONV	ARTIsDataProp	(P_ART_Property pprp);

/**
 * Returns the definition string of a data property, in the same format as it is used in the parser and in the function ARTSetParameter (see there
 * for details on the format.) A definition string is also returned if the value of the data property has not been set using ARTSetParameter. This
 * function does not (yet) work with array types.
 * @param dc A data property whose definition should be retrieved.
 * @returns A pointer to a null terminated string containing the definition. Do not change the contents of the string and do not free the associated memory.
 */
__DECLSPEC const char* __CALLCONV ARTGetDefinitionString          (P_ART_DataProp dc);

/**
 * Returns the datatype of a data property.
 * @param pprp A pointer to the property whose type should be returned.
 * @returns The type, which is one of named constants of enumartion type T_ART_Type. 
 *    The DLL interface returns C_ART_undef also if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC T_ART_Type	__CALLCONV	ARTGetDatatype	(P_ART_Variant pprp);

/**
 * Returns the length of a data property.
 * @param pprp A pointer to the property whose length should be returned.
 * @returns The length of the property. 
 *    The DLL interface returns -1 if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC int	__CALLCONV	ARTGetLength	(P_ART_Variant pprp);

/**
 * Returns the value of a data property.
 * @param pprp A pointer to the property whose value should be returned.
 * @returns A pointer to the value of the property.
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC P_ART_Variant	__CALLCONV	ARTGetValue	(P_ART_DataProp pprp);

/**
 * @param pprp A pointer to a data property.
 * @param idx The index of the string to be read. If the property is not an array, this 
 *	parameter may be omitted.
 * @returns the string saved at index idx of the data property pointed to by prpp.
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC char*	__CALLCONV	ARTGetString	(P_ART_Variant pprp, int idx);

/**
 * Set the string at index idx of the data property pointed to by pprp to s. If the 
 * property is NULL nothing is done. If the type of the data property is not C_ART_str or C_ART_nstr nothing is done. If
 * the index is out of bounds, nothing is done.
 * @param pprp A pointer to a data property.
 * @param idx The index of the string to be written. If the property is not an array, this 
 *	parameter may be omitted.
 * @param s The string to be written. 
 * @returns true if the function is successful
 *    The DLL interface returns false if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC bool	__CALLCONV	ARTSetString	(P_ART_Variant pprp, int idx, const char* s);


/**
 * @param pprp A pointer to a data property.
 * @param idx The index of the value to be read. If the property is not an array, this 
 *	parameter may be omitted.
 * @returns the integer value saved at index idx of the data property pointed to by prpp.
 *    Note that the DLL interface returns 0 if an error occurs. To make sure call
 *    ARTGetLastErrorMessage to get the error message and find out if there was an
 *    error or if the value of the data property was 0. 
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC int	__CALLCONV	ARTGetInteger	(P_ART_Variant pprp, int idx);

/**
 * Set the integer at index idx of the data property pointed to by pprp to i. 
 * @param pprp A pointer to a data property.
 * @param idx The index of the value to be written. If the property is not an array, this 
 *	parameter may be omitted.
 * @param i The value to be written. 
 * @returns true if the function is successful
 *    The DLL interface returns false if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC bool	__CALLCONV	ARTSetInteger	(P_ART_Variant pprp, int idx, int i);

/**
 * @param pprp A pointer to a data property.
 * @param idx The index of the value to be read. If the property is not an array, this 
 *	parameter may be omitted.
 * @returns the floating point value saved at index idx of the data property pointed to by prpp.
 *    Note that the DLL interface returns 0 if an error occurs. To make sure call
 *    ARTGetLastErrorMessage to get the error message and find out if there was an
 *    error or if the value of the data property was 0. 
 * @throws ARTerror if not using the DLL interface.
  */   
__DECLSPEC float	__CALLCONV	ARTGetFloat	(P_ART_Variant pprp, int idx);

/**
 * Set the floating point value at index idx of the data property pointed to by pprp to f. 
 * @param pprp A pointer to a data property.
 * @param idx The index of the value to be written. If the property is not an array, this 
 *	parameter may be omitted.
 * @param f The value to be written. 
 * @returns true if the function is successful
 *    The DLL interface returns false if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC bool	__CALLCONV	ARTSetFloat	(P_ART_Variant pprp, int idx, float f);

/**
 * @param pprp A pointer to a data property.
 * @param idx The index of the value to be read. If the property is not an array, this 
 *	parameter may be omitted.
 * @returns the floating point value saved at index idx of the data property pointed to by prpp.
 *    Note that the DLL interface returns 0 if an error occurs. To make sure call
 *    ARTGetLastErrorMessage to get the error message and find out if there was an
 *    error or if the value of the data property was 0. 
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC double	__CALLCONV	ARTGetDouble	(P_ART_Variant pprp, int idx);

/**
 * Set the double value at index idx of the data property pointed to by pprp to d. 
 * @param pprp A pointer to a data property.
 * @param idx The index of the value to be written. If the property is not an array, this 
 *	parameter may be omitted.
 * @param d The value to be written. 
 * @returns true if the function is successful
 *    The DLL interface returns false if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC bool	__CALLCONV	ARTSetDouble	(P_ART_Variant pprp, int idx, double d);

/**
 * @param pprp A pointer to a data property.
 * @param idx The index of the value to be read. If the property is not an array, this 
 *	parameter may be omitted.
 * @returns the complex value saved at index idx of the data property pointed to by prpp.
 *    Note that the DLL interface returns (0,0) if an error occurs. To make sure call
 *    ARTGetLastErrorMessage to get the error message and find out if there was an
 *    error or if the value of the data property was (0,0). 
 * @throws ARTerror if not using the DLL interface.
 */   
#ifdef __cplusplus
__DECLSPEC T_ART_Cmplx	__CALLCONV	ARTGetComplex	(P_ART_Variant pprp, int idx = 0);
#else
__DECLSPEC T_ART_Cmplx	__CALLCONV	ARTGetComplex	(P_ART_Variant pprp, int idx);
#endif

/**
 * Set the complex value at index idx of the data property pointed to by pprp to c. 
 * @param pprp A pointer to a data property.
 * @param idx The index of the complex value to be written. If the property is not an array, this 
 *	parameter may be omitted.
 * @param c The complex value to be written. 
 * @returns true if the function is successful
 *    The DLL interface returns false if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC bool	__CALLCONV	ARTSetComplex	(P_ART_Variant pprp, int idx, T_ART_Cmplx c);

/**
 * @param pprp A pointer to a data property.
 * @param idx The index of the value to be read. If the property is not an array, this 
 *	parameter may be omitted.
 * @returns the triple value saved at index idx of the data property pointed to by prpp.
 *    Note that the DLL interface returns a triple with all values set to 0 if an error occurs. To make sure call
 *    ARTGetLastErrorMessage to get the error message and find out if there was an
 *    error or if the value of the triple was (0,0,0). 
 * @throws ARTerror if not using the DLL interface.
 */
#ifdef __cplusplus
__DECLSPEC T_ART_Tripl	__CALLCONV	ARTGetTriple	(P_ART_Variant pprp, int idx = 0);
#else
__DECLSPEC T_ART_Tripl	__CALLCONV	ARTGetTriple	(P_ART_Variant pprp, int idx);
#endif

/**
 * Set the triple value at index idx of the data property pointed to by pprp to t. 
 * @param pprp A pointer to a data property.
 * @param idx The index of the triple value to be written. If the property is not an array, this 
 *	parameter may be omitted.
 * @param t The value to be written. 
 * @returns true if the function is successful
 *    The DLL interface returns false if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC bool	__CALLCONV	ARTSetTriple	(P_ART_Variant pprp, int idx, T_ART_Tripl t);

/**
 * @param pprp A pointer to a data property.
 * @param idx The index of the value to be read. If the property is not an array, this 
 *	parameter may be omitted.
 * @returns the matrix saved at index idx of the data property pointed to by prpp.
 *    Note that the DLL interface returns a matrix with default values (all 0) if an error occurs. To make sure call
 *    ARTGetLastErrorMessage to get the error message and find out if there was an
 *    error or if the value returned is indeed that of the data property's matrix. 
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC T_ART_Matrix	__CALLCONV	ARTGetMatrix	(P_ART_Variant pprp, int idx);

/**
 * Set the matrix at index idx of the data property pointed to by pprp to m. 
 * @param pprp A pointer to a data property.
 * @param idx The index of the matrix to be written. If the property is not an array, this 
 *	parameter may be omitted.
 * @param m The value to be written. 
 * @returns true if the function is successful
 *    The DLL interface returns false if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC bool	__CALLCONV	ARTSetMatrix	(P_ART_Variant pprp, int idx, T_ART_Matrix m);


/**
 * Find a property of an object.
 * @param host The object in which the property will be searched.
 * @param nam The name of the property to be searched.
 * @returns A pointer to the property or NULL if no matching property was found.
 *    The DLL interface also returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
  __DECLSPEC P_ART_Property	__CALLCONV	ARTFindProperty	(P_ART_Object  host, const char* nam);

/**
 * Find a data property of an object.
 * @param host The object in which the property will be searched.
 * @param nam The name of the property to be searched.
 * @returns A pointer to the property or NULL if no matching property was found.
 *    The DLL interface also returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC P_ART_DataProp	__CALLCONV	ARTFindDataProperty	(P_ART_Object  host, const char* nam);

/**
 * Find a method of an object.
 * @param host The object in which the method will be searched.
 * @param nam The name of the method to be searched.
 * @returns A pointer to the ARTmethod or NULL if no matching method was found.
 *    The DLL interface also returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC P_ART_Method	__CALLCONV	ARTFindMethod	(P_ART_Object  host, const char* nam);

/**
 * Find an object that is appended to another object.
 * @param host The object in which the appended object will be searched.
 * @param nam The name of the object which you are looking for.
 * @returns A pointer to the ARTobject or NULL if no matching object was found.
 *    The DLL interface also returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */  
__DECLSPEC P_ART_Object	__CALLCONV	ARTFindObject	(P_ART_ListProp host, const char* nam);

/**
 * Iterates through all properties of the object host (including data properties). To find out if a property is a data property
 * use ARTIsDataProp. If the object is an element only properties of the element itself,
  * but not of the model of the element are enumerated. Use ARTGetModel to get a pointer to the model and pass this pointer
  * to this function to get the properties of the model instead.
 * @param host A pointer to an ARTobject
 * @param pos A pointer to a property. This parameter is not used except for deciding if
 *	the iteration should start at the beginning. It is used to allow you to quickly cycle through all properties. 
 *  Check out this example:
 * @code
			P_ART_Property prop = NULL;
			while ( prop = ARTGetProperties(mySim, prop) ) 
			{
				// Do some processing with prop
			}
 * @endcode
 * Note that the property returned is not forcefully the property following pos!
 * @return The first property if pos is NULL, otherwise the next in the list. If the
 *    end of the list has been reached, the function returns NULL. 
 *    The DLL interface also returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
  __DECLSPEC P_ART_Property	__CALLCONV	ARTGetProperties	(P_ART_Object  host, P_ART_Property pos);

/**
 * Iterates through all the data properties of the object host. If the object is an element, only properties of the element,
  * but not of the model are enumerated. Use ARTGetModelProperties to get the properties of the model instead.
 * @param host A pointer to an ARTobject
 * @param pos A pointer to a property. This parameter is not used except for deciding if
 *	the iteration should start at the beginning. It is used to allow you to quickly cycle through all properties. 
 *  Check out the example given in the documentation of ARTGetProperties.
 * @return The first property if pos is NULL, otherwise the next in the list. If the
 *    end of the list has been reached, the function returns NULL. 
 *    The DLL interface also returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC P_ART_DataProp  __CALLCONV ARTGetDataProperties      (P_ART_Object  host, P_ART_DataProp pos);


/**
 * Iterates through all methods of the object host.
 * @param host A pointer to an ARTobject
 * @param pos A pointer to a method. This parameter is not used except for deciding if
 *	the iteration should start at the beginning. It is used to allow you to quickly cycle through all methods. 
 *  Check out the example given in the documentation of ARTGetProperties.   
 * @return The first method if pos is NULL, otherwise the next in the list. If the
 *    end of the list has been reached, the function returns NULL. 
 *    The DLL interface also returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC P_ART_Method	__CALLCONV	ARTGetMethods	(P_ART_Object  host, P_ART_Method pos);

/**
 * Iterates through all objects in the list host.
 * @param host A pointer to a list property.
 * @param pos A pointer to an object. This parameter is not used except for deciding if
 *	the iteration should start at the beginning. It is used to allow you to quickly cycle through all objects. 
 *  Check out the example given in the documentation of ARTGetProperties.
 * @return The first object if pos is NULL, otherwise the next in the list. If the
 *    end of the list has been reached, the function returns NULL. 
 *    The DLL interface also returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC P_ART_Object	__CALLCONV	ARTGetObjects	(P_ART_ListProp host, P_ART_Object  pos);


/**
 * Appends a data property to an ARTobject.
 * @param host The object the data property should be appended to.
 * @param val The value of the data property.
 * @param nam The name of the data property.
 * @param sds The short description of the data property.
 * @param lds The long description of the data property.
 * @param htm Filename of the html description of the data property.
 * @returns A pointer to the data property appended.
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */	
__DECLSPEC P_ART_DataProp	__CALLCONV	ARTAppendDataProp	(P_ART_Object host, P_ART_Variant val, const char* nam, const char* sds, const char* lds, const char* htm);

/**
 * Appends a listable property to an ARTobject. A listable property is an array of properties.
 * @param host The object the listable property should be appended to.
 * @param nam The name of the listable property.
 * @param sds The short description of the listable property.
 * @param lds The long description of the listable property.
 * @param htm Filename of the html description of the listable property.
 * @returns A pointer to the listable property appended.
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */	
__DECLSPEC P_ART_ListProp	__CALLCONV	ARTAppendListProp	(P_ART_Object host, const char* nam, const char* sds, const char* lds, const char* htm);

/**
 * Appends a method to an ARTobject.
 * @param host The object the method should be appended to.
 * @param nam The name of the method.
 * @param sds The short description of the method.
 * @param lds The long description of the method.
 * @param htm Filename of the html description of the method.
 * @returns A pointer to the method appended.
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */ 
__DECLSPEC P_ART_Method	__CALLCONV	ARTAppendMethod	(P_ART_Object host, const char* nam, const char* sds, const char* lds, const char* htm);

/**
 * Appends an object to a listable property.
 * @param host The listable property the object should be appended to.
 * @param nam The name of the object.
 * @param sds The short description of the object.
 * @param lds The long description of the object.
 * @param htm Filename of the html description of the object.
 * @returns A pointer to the object appended.
 *    The DLL interface returns NULL if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */ 
__DECLSPEC P_ART_Object	__CALLCONV	ARTAppendObject	(P_ART_ListProp host, const char* nam, const char* sds, const char* lds, const char* htm);

/**
 * Deletes the property prp of object host.  
 * @param host A pointer to an ARTobject.
 * @param prp A pointer to the property whose reference should be deleted form the
 *    properties of object host. 
 * @return True if a property is deleted, false otherwise.
 *    The DLL interface returns false also if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
  __DECLSPEC bool	__CALLCONV	ARTDeleteProperty	(P_ART_Object  host, P_ART_Property prp);

/**
 * Deletes the method mtd of object host.  
 * @param host A pointer to an ARTobject.
 * @param mtd A pointer to the method whose reference should be deleted form the
 *    methods of object host. 
 * @return True if a method is deleted, false otherwise.
 *    The DLL interface returns false also if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC bool	__CALLCONV	ARTDeleteMethod	(P_ART_Object  host, P_ART_Method mtd);

/**
 * Deletes the object pobj of object host.  
 * @param host A pointer to an ARTobject.
 * @param pobj A pointer to the object whose reference should be deleted form the
 *    object-list of object host. 
 * @return True if an object is deleted, false otherwise.
 *    The DLL interface returns false also if an error occurs.
 *    Use ARTGetLastErrorMessage to get the error message.
 * @throws ARTerror if not using the DLL interface.
 */   
__DECLSPEC bool	__CALLCONV	ARTDeleteObject	(P_ART_ListProp host, P_ART_Object pobj);

/**
 * Returns the last error message. Use this function only when you are using the
 * DLL interface, otherwise this function will always return an empty string. When
 * using the header files and programming in C++, catch exceptions of type 
 * ARTerror to handle errors.  
 * Once an error message has been queried, this function returns an empty string.
 * @returns a null-terminated string containing the error message with and the 
 * function where the error occured. Note that this can be a function you did not
 * call directly.  
 */ 
__DECLSPEC char *	__CALLCONV	ARTGetLastErrorMessage	();

/** NEU
 * Returns a string representation of the dependency tree. This function is for
 * debugging only and might be removed in the future.
 */  
char*	__CALLCONV	ARTGetDependencyTree	(P_ART_DataProp pprp, const char* linebreak);

/** @}
 * End of documentation group ARTinterface
 */

//for debug and command line interface
#ifdef __cplusplus

  __DECLSPEC void __CALLCONV listprops(ARTobject* obj, string ind);
  __DECLSPEC void __CALLCONV listmets(ARTobject* obj, string ind);
__DECLSPEC void __CALLCONV listobjs(ListProp* lstprp, string ind);


}
#endif /* __cplusplus */

#endif
