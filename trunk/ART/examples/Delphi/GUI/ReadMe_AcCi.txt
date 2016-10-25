
Acoustic Circuit v 1.0
**********************

This application makes use of the ART library to calculate the impedance of wind instruments.

Add elements by right clicking on empty spots of the canvas. Elements are represented by boxes, each with one output and one input port. If we consider the propagation of the radiation impedance through the instrument as the direction of the signal in the circuit, the output port is on the left (!) side of an element's box, the input port on the right side. If you create a wind instrument, the radiating element would be on the right side of the canvas, the mouthpiece on the left side. Be careful to grasp this detail before building your circuits. Remember how we calculate impedance (see artool.sf.net for details)

To edit the data properties of an element right click on it and choose "Properties" in the context menu. To edit the simulator properties right click on an empty spot or an element and choose "Simulator Properties". 

Connect elements by clicking on input or output port of one element and dragging the cursor to another element. Only if the connection is valid the line will turn green. To create branches connect the output ports of several instruments to one input port (start by choosing the output port if the input already has one connection). To remove a connection click on one of its ports and drag and drop the connection off the element.

Right click on an element to calculate the input impedance at the element's output port; that means the impedance is propagated through all branches and through the element you have clicked.

The programme can read ins files and bore lists. A few examples are included in the package.


Authors
*******

The GUI application was written by Sadjad Siddiq (University of Music and Performing Arts Vienna) and is based on the ART library written by Jonathan Kemp, Wilfried Kausel, Alistair Braden, Delphine Chadefaux, Vasileios Chatziioannou, Sadjad Siddiq and Clemens Geyer. (see artool.sf.net for more details)

