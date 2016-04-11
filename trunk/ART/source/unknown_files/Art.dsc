application Acoustic Research Tool Main Program (art) {
   message {
      "TAP - Transfer Analysis Package - Acoustic Research Tool - Version 1.0";
      "Copyright (c) W. Kausel, IWK MDW-Vienna 2009. All rights reserved.";
   }

   description {
      "Sample main program demonstrating some of the capabilities of ART";
      " using a command line interface. All acoustic elements can be tested";
      " individually or in sophisticated systems described by script.\n";
      
      " Receives frequencies as single column input stream with double values in binary format";
      " and creates triple column output stream consisting of (frequency, magnitude, argument)";
      " triples. Alternatively start frequency, stop frequency and frequency increment can be";
      " given as command line option.\n";
   }

   uses stdin, stdout;

   option isignal {
      description "read input signal from";
      type ipipe = "stdin";
   }

   option osignal {
      description "write output signal to";
      type opipe = "stdout";
   }

   option borejump {
      description "bore jump section (id=0)";
      argument rin {
         description "input radius";
         type double = 1.0 [cm];
      }
      argument rout {
         description "output radius";
         type double = 2.0 [cm];
      }
      argument loss {
         description "local loss factor";
         type double = 1.0;
      }
      argument temperature {
         description "local temperature";
         type double = 21.0 [degC];
      }
      argument humidity {
         description "local relative humidity";
         type double = 80.0 [%];
      }
      argument CO2 {
         description "local Mol fraction of CO2";
         type double = 382.0 [ppm];
      }
   }

   option cylinder {
      description "cylinder section (id=1)";
      argument len {
         description "length";
         type double = 100.0 [cm];
      }
      argument rin {
         description "radius";
         type double = 1 [cm];
      }
      argument loss {
         description "local loss factor";
         type double = 1.0;
      }
      argument temperature {
         description "local temperature";
         type double = 21.0 [degC];
      }
      argument humidity {
         description "local relative humidity";
         type double = 80.0 [%];
      }
      argument CO2 {
         description "local Mol fraction of CO2";
         type double = 382.0 [ppm];
      }
   }

   option cone {
      description "cone section (id=2)";
      argument len {
         description "length";
         type double = 100.0 [cm];
      }
      argument rin {
         description "input radius";
         type double = 1.0 [cm];
      }
      argument rout {
         description "output radius";
         type double = 2.0 [cm];
      }
      argument loss {
         description "local loss factor";
         type double = 1.0;
      }
      argument temperature {
         description "local temperature";
         type double = 21.0 [degC];
      }
      argument humidity {
         description "local relative humidity";
         type double = 80.0 [%];
      }
      argument CO2 {
         description "local Mol fraction of CO2";
         type double = 382.0 [ppm];
      }
   }

   option bessel {
      description "bessel section (id=3)";
      argument len {
         description "length";
         type double = 100.0 [cm];
      }
      argument rin {
         description "input radius";
         type double = 1.0 [cm];
      }
      argument rout {
         description "output radius";
         type double = 2.0 [cm];
      }
      argument flare {
         description "flare";
         type double = 2.0 [cm];
      }
      argument loss {
         description "local loss factor";
         type double = 1.0;
      }
      argument temperature {
         description "local temperature";
         type double = 21.0 [degC];
      }
      argument humidity {
         description "local relative humidity";
         type double = 80.0 [%];
      }
      argument CO2 {
         description "local Mol fraction of CO2";
         type double = 382.0 [ppm];
      }
   }

   option list {
      description "list section element (id=4)";
      argument filename {
         description "name of script file containing instrument description";
         type char*;
      }
   }

   option bcylinder {
      description "cylinder bend section (id=5)";
      argument len {
         description "length";
         type double = 100.0 [cm];
      }
      argument rin {
         description "radius";
         type double = 1 [cm];
      }
      argument rbend {
         description "bend radius";
         type double = 50.0 [cm];
      }
      argument loss {
         description "local loss factor";
         type double = 1.0;
      }
      argument temperature {
         description "local temperature";
         type double = 21.0 [degC];
      }
      argument humidity {
         description "local relative humidity";
         type double = 80.0 [%];
      }
      argument CO2 {
         description "local Mol fraction of CO2";
         type double = 382.0 [ppm];
      }
   }

   option bcone {
      description "cone bend section (id=6)";
      argument len {
         description "length";
         type double = 100.0 [cm];
      }
      argument rin {
         description "input radius";
         type double = 1.0 [cm];
      }
      argument rout {
         description "output radius";
         type double = 2.0 [cm];
      }
      argument rbend {
         description "bend radius";
         type double = 50.0 [cm];
      }
      argument loss {
         description "local loss factor";
         type double = 1.0;
      }
      argument temperature {
         description "local temperature";
         type double = 21.0 [degC];
      }
      argument humidity {
         description "local relative humidity";
         type double = 80.0 [%];
      }
      argument CO2 {
         description "local Mol fraction of CO2";
         type double = 382.0 [ppm];
      }
   }

   option pressure {
      description "athmospheric pressure";
      type double = 101.325 [kPa];
   }

   option modes {
      description { "number of modes to calculate";
                    " 0   = Mapes Riordan (fastest)";
                    " 1   = plane wave (fast, accurate)";
                    " 2-3 = efficient multimode"; 
                    " 4-n = very slow multimode"; }
      type int = 1;
   }
   
   option bends {
      description { "enforce calculation of radial modes";
                    " 0   = ignore radial modes";
                    }
      type int = 1;
   }
   
   option accumulate {
      description "enforce matrix accumulation";
   }


   option lossless {
      description "use lossless models";
   }

   option simplified {
      description "use simplified method";
   }

   option accuracy {
      description "slicing accuracy for cone and bessel sections";
      type double = 0.5 [%];
   }

   option columns {
     description { "number of returned complex data points per frequency";
                   " 0 = composite wave"; 
                   " 1 = plane wave only"; 
                   " n = diagonal impedance modes only (if n<=modes)";
                   " N = complete square matrix (if N>modes)"; }
     type int = 1;
   }

   option radiationfile {
      description "specify radiation impedance, calculate input impedance";
      argument filename {
         description { "name of multi column blank separated text file ";
                       " freq mag_0 arg_0 mag_1 arg_1...\n"; }
         type char*;
      }
      argument columns {
         description { "number of data points per frequency";
                       " 1 = plane wave only"; 
                       " n = diagonal impedance modes only (if n<=modes)";
                       " N = complete square matrix (if N>modes)"; }
         type int=1;
      }
   }

   option impedancefile {
      description "specify input impedance, calculate radiation impedance";
      argument filename {
         description { "name of multi column blank separated text file ";
                       " freq mag_0 arg_0 mag_1 arg_1...\n"; }
         type char*;
      }
      argument columns {
         description { "number of data points per frequency";
                       " 1 = plane wave only"; 
                       " n = diagonal impedance modes only (if n<=modes)";
                       " N = complete square matrix (if N>modes)"; }
         type int=1;
      }
   }

   option interiorwave {
      description "calculate interior wave quantities p, u, z right of specified element";
      argument impedancefile {
         description { "name of multi column blank separated text file ";
                       " freq mag_0 arg_0 mag_1 arg_1...\n"; }
         type char*;
      }
      argument columns {
         description { "number of data points per frequency";
                       " 1 = plane wave only"; 
                       " n = diagonal impedance modes only (if n<=modes)";
                       " N = complete square matrix (if N>modes)"; }
         type int=1;
      }
      argument elementname {
         description "name of horn element to calculate wave quantities at ";
         type char*;
      }
      argument radius {
         description "distance from element axis in radial dimension (or first coordinate)";
         type double = 0 [m];
      }
      argument arg {
         description "angular distance from horizontal plane (or second coordinate)";
         type double = 0 [rad|m];
      }
   }

   option termination {
      description { "termination mode";
                    " 0..reflecting";
                    " 1..Levine/Zorumsky"; }
      type int = 1;
   }

   option frequencies {
      description "specify frequency grid (do not use input stream)";
      argument f0 {
         description "starting frequency";
         type double = 10.0 [Hz];
      }
      argument f1 {
         description "stop frequency";
         type double = 1800.0 [Hz];
      }
      argument df {
         description "frequency increment";
         type double = 10.0 [Hz];
      }
      argument refine {
         description "refine peaks";
         type int = 0 [bool];
      }
      argument target {
         description "accuracy target";
         type double = 0.00001 [%];
      }
   }

   option run {
      description "run complete script file";
      argument filename {
         description "name of script file to execute";
         type char*;
      }
   }

   option capabilities {
      description "list library capabilities";
   }

   option debug {
      description "debug output to statistics";
   }

   option stat {
      description "write program statistics to file";
      type char*;
   }

   option swap {
      description "exchange adjacent bytes of values";
   }
   
   invalid {
      if (IsDef (borejump)   +
          IsDef (cylinder)  +
          IsDef (cone)  +
          IsDef (bessel)  +
          IsDef (list)  +
          IsDef (bcylinder)  +
          IsDef (bcone)  +
          IsDef (capabilities)  +
          IsDef (run) > 1);
      message "section types and script file are exclusive options";
   }

   invalid {
      if (IsDef (borejump)   +
          IsDef (cylinder)  +
          IsDef (cone)  +
          IsDef (bessel)  +
          IsDef (list)  +
          IsDef (bcylinder)  +
          IsDef (bcone)  +
          IsDef (capabilities)  +
          IsDef (run) < 1);
      message "one section type or a script file must be specified";
   }
   
   invalid {
      if (IsDef (radiationfile)   +
          IsDef (impedancefile)  +
          IsDef (frequencies) > 1);
      message "radiationfile, impedancefile and frequencies are exclusive";
   }
}

