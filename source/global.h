/*
   global.h : 
        global variables, can be changed by the "set" command
 */

/**************************************/
/*      Code Options                  */
/**************************************/
class GlobalOption{
 public:
  bool   LineNumber            ;    // print line numbers
  double AngleStep             ;    // calculate angular distribution at this step
  double ReadXdataConversion   ;    // conversion factor of energy when importing data
  double ReadYdataConversion   ;    // conversion factor of cross section
  double WriteXdataConversion  ;    // conversion factor when tabulating energy
  double WriteYdataConversion  ;    // conversion factor when tabulating cross section

  GlobalOption(){
    LineNumber           =  false;
    AngleStep            =  0.0;    // when zero, Legendre coefficients are printed
    ReadXdataConversion  =  1.0e+6; // default in MeV
    ReadYdataConversion  =  0.001;  // default in milli-barn
    WriteXdataConversion =  1.0;    // default in eV
    WriteYdataConversion =  1.0;    // default in barn
  }
};


#ifndef DECE_TOPLEVEL
extern GlobalOption opt;
#endif
