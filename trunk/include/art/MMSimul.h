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

/* todo: 
- calculate termination
- 
*/
/*
#include "WindInstrument.h"

typedef const char* pChar;
typedef int* __stdcall TStatus_Indication(char* msg, int completion);

WindInstrument* ins = NULL;
TStatus_Indication* Status_Indication = NULL;
FILE* logfil;


int __stdcall load_net(int N, int* m, pChar* p0, double* p1, double* p2, double* p3, double* p4, double* p5, double* p6);
int __stdcall unload_net(int dummy);
int __stdcall set_nModes(int nModes, int lossy, int simplified, double boreResolution);
int __stdcall calc_imp(double f, double* mag, double* pha, double* tmag, double* tpha, int term, double gain);
int __stdcall preview_imp(double fMin, double fMax, double fStep, int term);
int __stdcall refine_imp(double eps, int term);

int __stdcall get_number_of_elements(int dummy);
int __stdcall goto_first_section(int dummy);
int __stdcall goto_last_section(int dummy);
int __stdcall section_plus_plus(int dummy);
int __stdcall section_minus_minus(int dummy);
int __stdcall return_section(int* m, pChar* p0, double* p1, double* p2, double* p3, double* p4, double* p5, double* p6);
int __stdcall modify_section(int m, pChar p0, double p1, double p2, double p3, double p4, double p5, double p6);
int __stdcall insert_section(int m, pChar p0, double p1, double p2, double p3, double p4, double p5, double p6);
int __stdcall delete_section(int dummy);

int __stdcall begin_trace(char* filename);
int __stdcall end_trace(int dummy);
int __stdcall run_script(char* filename);
int __stdcall store_callback(TStatus_Indication func);

*/
