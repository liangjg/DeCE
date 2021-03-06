/******************************************************************************/
/**     DeCE OPERATION                                                       **/
/******************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <sstream>

using namespace std;

#include "dece.h"
#include "command.h"
#include "global.h"
#include "terminate.h"

static void DeceOperationCALC            (ENDFDict *, ENDF **);
static void DeceOperationDUPLICATE       (ENDFDict *, ENDF **);
static void DeceOperationDELETE          (ENDFDict *);
static void DeceOperationREAD            (ENDFDict *, ENDF **);
static void DeceOperationANGDIST         (ENDFDict *, ENDF **);
static void DeceOperationLIBREAD         (ENDFDict *, ENDF **);
static void DeceOperationTABLE           (ENDFDict *, ENDF **, ifstream *);
static void DeceOperationEXTRACT         (ENDFDict *, ENDF **, ifstream *);
static void DeceOperationPOINT           (ENDFDict *, ENDF **);
static void DeceOperationFACTOR          (ENDFDict *, ENDF **);
static void DeceOperationAPPLYFUNC       (ENDFDict *, ENDF **);
static void DeceOperationREADJUST        (ENDFDict *, ENDF **);
static void DeceOperationCHANGEINT       (ENDFDict *, ENDF **);
static void DeceOperationCHANGEQVAL      (ENDFDict *, ENDF **);
static void DeceOperationCHECKTHRESHOLD  (ENDFDict *, ENDF **);
static void DeceOperationNUTOTAL         (ENDFDict *, ENDF **);
static void DeceOperationBOUNDCORRECT    (ENDFDict *, ENDF **);
static void DeceOperationDUPLICATEPOINT  (ENDFDict *, ENDF **);
static void DeceOperationGENPROD         (ENDFDict *, ENDF **);
static void DeceOperationRECONSTRUCT     (ENDFDict *, ENDF **);
static void DeceOperationPOINTWISE       (ENDFDict *, ENDF **);

extern CLine cmd;

/**********************************************************/
/*      Main Data Operations                              */
/**********************************************************/
void DeceOperation(ENDFDict *dict, ENDF *lib[], ifstream *fpin)
{
  string ope = CmdGetOperation();

  //--------------------------------------------------------
  //  section manipulation commands

  /*** CALC: manipulate TAB1 record */
  if(ope == "calc" || ope == "make4"){
    DeceOperationCALC(dict,lib);
  }

  /*** DUPLICATE: copy data */
  else if( (ope == "duplicate") || (ope == "copy") ){
    DeceOperationDUPLICATE(dict,lib);
  }

  /*** DELETE: delete section */
  else if( (ope == "delete") || (ope == "multidelete") ){
    DeceOperationDELETE(dict);
  }

  /*** READ: read tabulated cross section data file, and replace section */
  else if( (ope == "read") || (ope == "multiread") || (ope == "mergeread") ){
    DeceOperationREAD(dict,lib);
  }

  /*** ANGDIST: read tabulated angular distribution data file */
  else if( (ope == "angdist") || (ope == "multiangdist") ){
    DeceOperationANGDIST(dict,lib);
  }

  /*** LIBREAD: read a section from another ENDF file, and replace */
  else if( (ope == "libread") || (ope == "multilibread") ){
    DeceOperationLIBREAD(dict,lib);
  }

  /*** TABLE: tabulate data */
  else if(ope == "table"){
    DeceOperationTABLE(dict,lib,fpin);
  }

  /*** EXTRACT: dead copy section */
  else if(ope == "extract" ){
    DeceOperationEXTRACT(dict,lib,fpin);
  }

  /*** ADDPOINT, DELPOINT: add / remove a point in TAB1 record */
  else if( (ope == "addpoint") || (ope == "delpoint") ){
    DeceOperationPOINT(dict,lib);
  }

  /*** FACTOR, NORMALIZE: multiply by a factor */
  else if( (ope == "factor") || (ope == "normalize") ){
    DeceOperationFACTOR(dict,lib);
  }

  /*** FUNC1, FUNC2: multiply by a factor that is calculated with a function */
  else if( (ope == "applyfunc1") || (ope == "applyfunc2") || (ope == "applyfunc3") ){
    DeceOperationAPPLYFUNC(dict,lib);
  }

  /*** READJUST: rescale individual sections in MF3 by the summed section */
  else if(ope == "readjust"){
    DeceOperationREADJUST(dict,lib);
  }

  /*** CHANGEINT: change interpolation scheme in MF3 */
  else if(ope == "changeint"){
    DeceOperationCHANGEINT(dict,lib);
  }

  /*** CHANGEQVAL: change Q-vales in MF3 */
  else if(ope == "changeqval"){
    DeceOperationCHANGEQVAL(dict,lib);
  }

  /*** CHECHKENERGY: check Q-vales and threshold energies in MF3 */
  else if( (ope == "checkthreshold") || (ope == "fixthreshold") ){
    DeceOperationCHECKTHRESHOLD(dict,lib);
  }


  //--------------------------------------------------------
  //  MF1 manipulations

  /*** SHOWHEADERS: print parameters in the header part */
  else if(ope == "showheaders"){
    DeceShowHeaders(dict);
  }

  /*** EDITHEADER: modify header parameter */
  else if(ope == "editheader"){
    DeceEditHeader(dict,(string)cmd.text,cmd.x);
  }

  /*** FIXAWR: fix AWR in at the top of the file */
  else if(ope == "fixawr"){
    DeceFixAWR(dict);
  }

  /*** NUTOTAL: generate / reconstruct total nu-bar as the sum of 455 and 456 */
  else if(ope == "nutotal"){
    DeceOperationNUTOTAL(dict,lib);
  }


  //--------------------------------------------------------
  //  MF6 manipulations

  /*** BOUNDCORRECT: correct energy boundary in continuum in MF6 */
  else if(ope == "boundcorrect"){
    DeceOperationBOUNDCORRECT(dict,lib);
  }
  /*** DUPLICATEPOINT: duplicate the last point in MF6 */
  else if(ope == "duplicatepoint"){
    DeceOperationDUPLICATEPOINT(dict,lib);
  }
  /*** GENPROD: generate production cross section from MF6 MT5 */
  else if(ope == "genprod"){
    DeceOperationGENPROD(dict,lib);
  }


  //--------------------------------------------------------
  //  resonance manipulation

  /*** RECONSTRUCT: reconstruct cross sections from resonances */
  /*** RECONANGDIST: calculate Legendre coefficients from resonance parameters */
  /*** SMOOTHANGDIST: calculate energy averaged Legendre coefficients in RRR */
  else if( (ope == "reconstruct") || (ope == "reconangdist") || (ope == "smoothangdist")  ){
    DeceOperationRECONSTRUCT(dict,lib);
  }


  //--------------------------------------------------------
  //  data processing [experimental]

  /*** POINTWISE: create pointwise cross section */
  else if(ope == "pointwise"){
    DeceOperationPOINTWISE(dict,lib);
  }


  //--------------------------------------------------------
  //  miscellaneous operations

  /*** TPID: replace TPID */
  else if(ope == "tpid"){
    CmdExtractString(dict->tpid);
  }

  /*** INDEX: print section index */
  else if(ope == "index"){
    DeceScanIndex(dict);
  }

  /*** SET: modify global setting */
  else if(ope == "set" || ope == "unset" || ope == "showoptions"){
    DeceGlobalOption(ope,(string)cmd.text,cmd.x);
  }

  /*** ECHO: print text */
  else if(ope == "echo"){
    message << cmd.text;
    Notice("NOTE");
  }

  /*** Unknown command */
  else TerminateCode("command not found",ope);
}


/**********************************************************/
/* CALC                                                   */
/*      Add, subtract, multiplicate, divide subsections   */
/*      in MF3                                            */
/* MAKE4                                                  */
/*      Add subsections from 51 to 91 to create 4 in MF3  */
/*      abbreviation of calc 4 = 51:91                    */
/* calc MTdest = MTsrc1 [+ - * / :] MTsrc2                */
/**********************************************************/
void DeceOperationCALC(ENDFDict *dict, ENDF *lib[])
{
  string ope = CmdGetOperation();

  if(ope == "make4"){
    cmd.text[0] = ':';
    cmd.mt = 4;
    cmd.opt1 = 51;
    cmd.opt2 = 91;
  }
  DeceCreateLib(dict,3,cmd.mt);
  DeceCalc(dict,lib,cmd.mt,cmd.opt1,cmd.opt2,cmd.text[0]);

  message << "MF3:MT" << cmd.mt << " created by ";
  if(     cmd.text[0] == '+'){ message << "adding MT" << cmd.opt1 << " and MT" << cmd.opt2; }
  else if(cmd.text[0] == '-'){ message << "subtracting MT" << cmd.opt2 << " from MT" << cmd.opt1; }
  else if(cmd.text[0] == '*'){ message << "multiplying MT" << cmd.opt1 << " and MT" << cmd.opt2; }
  else if(cmd.text[0] == '/'){ message << "dividing MT" << cmd.opt1 << " by MT" << cmd.opt2; }
  else if(cmd.text[0] == ':'){ message << "adding subsections from MT" << cmd.opt1 << " to MT" << cmd.opt2; }
  Notice("DeceOperationCALC");
}


/**********************************************************/
/* DUPLICATE, COPY                                        */
/*      copy MTsrc subsection to MTdest in the same MF    */
/* duplicate MFsrc MTsrc MTdest                           */
/**********************************************************/
void DeceOperationDUPLICATE(ENDFDict *dict, ENDF *lib[])
{
  DeceCreateLib(dict,cmd.mf,cmd.opt1);

  int k0 = dict->getID(cmd.mf,cmd.mt);
  int k1 = dict->getID(cmd.mf,cmd.opt1);

  if(k0 < 0) TerminateCode("source MT number not found",cmd.mt);

  ENDFLibCopy(lib[k0],lib[k1]);
  lib[k1]->setENDFmt(cmd.opt1);

  message << "MF" << cmd.mf << ":MT" << cmd.mt << " copied to ";
  message << "MF" << cmd.mf << ":MT" << cmd.opt1;
  Notice("DeceOperationDUPLICATE");
}


/**********************************************************/
/* DELETE                                                 */
/*      delete subsection or whole section                */
/* delete MF                                              */
/* delete MF MT                                           */
/* delete MF MTstart MTend                                */
/**********************************************************/
void DeceOperationDELETE(ENDFDict *dict)
{
  for(int mt=cmd.mt ; mt <= cmd.mtend ; mt++){
    DeceDelete(dict,cmd.mf,mt);
  }
}


/**********************************************************/
/* READ                                                   */
/*      read MF3 data from external file                  */
/*      mergeread replace a part of existing data         */
/* read MF MT "datafile"                                  */
/* meargeread MF MT "datafile"                            */
/**********************************************************/
void DeceOperationREAD(ENDFDict *dict, ENDF *lib[])
{
  string ope = CmdGetOperation();

  for(int mt=cmd.mt ; mt <= cmd.mtend ; mt++){
    DeceCreateLib(dict,cmd.mf,mt);
    if(ope == "mergeread")
      DeceRead(dict,lib[dict->getID(cmd.mf,mt)],cmd.mf,mt,cmd.text,cmd.opt1,true);
    else
      DeceRead(dict,lib[dict->getID(cmd.mf,mt)],cmd.mf,mt,cmd.text,cmd.opt1,false);
  }
}


/**********************************************************/
/* ANGDIST                                                */
/*      read external file to create MF2 or MF6           */
/* angdist MF MT "datafile"                               */
/**********************************************************/
void DeceOperationANGDIST(ENDFDict *dict, ENDF *lib[])
{
  for(int mt=cmd.mt ; mt <= cmd.mtend ; mt++){
    DeceCreateLib(dict,cmd.mf,mt);
    DeceAngdist(dict,lib,cmd.mf,mt,cmd.text,cmd.opt1);
  }
}


/**********************************************************/
/* LIBREAD                                                */
/*      read external ENDF-6 file                         */
/* libread MF MT "libraryfile"                            */
/**********************************************************/
void DeceOperationLIBREAD(ENDFDict *dict, ENDF *lib[])
{
  for(int mt=cmd.mt ; mt <= cmd.mtend ; mt++){
    DeceCreateLib(dict,cmd.mf,mt);
    DeceLibRead(dict,lib[dict->getID(cmd.mf,mt)],cmd.text);
  }
}


/**********************************************************/
/* TABLE                                                  */
/*      print data in XY column format                    */
/* table MF MT                                            */
/* table 2                                                */
/**********************************************************/
void DeceOperationTABLE(ENDFDict *dict, ENDF *lib[], ifstream *fpin)
{
  if((cmd.mf == 2) && (cmd.mt == 0)) cmd.mt = 151;

  DeceTable(dict,lib,fpin,cmd.mf,cmd.mt);
}


/**********************************************************/
/* EXTRACT                                                */
/*      print data in original format                     */
/* extract MF MT                                          */
/* extract 2                                              */
/**********************************************************/
void DeceOperationEXTRACT(ENDFDict *dict, ENDF *lib[], ifstream *fpin)
{
  DeceExtract(dict,lib,fpin,cmd.mf,cmd.mt);
}


/**********************************************************/
/* ADDPOINT, DELPOINT                                     */
/*      insert or delete one (x,y) data point in array    */
/* addpoint MF MT Xdata Ydata                             */
/* delpoint MF MT Xdata Ydata                             */
/**********************************************************/
void DeceOperationPOINT(ENDFDict *dict, ENDF *lib[])
{
  string ope = CmdGetOperation();
  DeceCheckMT(cmd.mt);
  DecePoint(dict,lib,cmd.mf,cmd.mt,cmd.x,cmd.y,ope);
}


/**********************************************************/
/* FACTOR, NORMALIZE                                      */
/*      re-scale subsection                               */
/* factor MF MT factor                                    */
/* normalize MF MT factor                                 */
/**********************************************************/
void DeceOperationFACTOR(ENDFDict *dict, ENDF *lib[])
{
  DeceCheckMT(cmd.mt);
  DeceFactor(dict,lib,cmd.mf,cmd.mt,cmd.x,cmd.y,cmd.xmin,cmd.xmax);
}


/**********************************************************/
/* APPLYFUNC1, APPLYFUNC2, APPLYFUNC3                     */
/*      multiply by function parameterized by input       */
/* applyfunc1 MF MT p1 p2 p3                              */
/* applyfunc2 MF MT p1 p2 p3                              */
/* applyfunc3 MF MT p1 p2 p3                              */
/**********************************************************/
void DeceOperationAPPLYFUNC(ENDFDict *dict, ENDF *lib[])
{
  DeceCheckMT(cmd.mt);
  DeceApplyFunc(dict,lib,cmd.mf,cmd.mt,cmd.opt1,cmd.x,cmd.y,cmd.xmin);
}


/**********************************************************/
/* READJUST                                               */
/*      re-scale summed subsection by partial subsections */
/* readjust MT                                            */
/**********************************************************/
void DeceOperationREADJUST(ENDFDict *dict, ENDF *lib[])
{
  DeceCheckMT(cmd.mt);
  int mtmp = 99;  // use MT = 99 as a temporal section
  DeceCreateLib(dict,3,mtmp);
  DeceReadjust(dict,lib,cmd.mt,mtmp);
  DeceDelete(dict,3,mtmp);
}


/**********************************************************/
/* CHANGEINT                                              */
/*      change interpolation                              */
/* changeint MT range point int                           */
/**********************************************************/
void DeceOperationCHANGEINT(ENDFDict *dict, ENDF *lib[])
{
  DeceCheckMT(cmd.mt);
  DeceChangeInt(dict,lib,cmd.mt,cmd.opt1,cmd.opt2,cmd.opt3);
}


/**********************************************************/
/* CHANGEQVAL                                             */
/*      change Q-value                                    */
/* changeqval MT Qm Qi                                    */
/**********************************************************/
void DeceOperationCHANGEQVAL(ENDFDict *dict, ENDF *lib[])
{
  DeceCheckMT(cmd.mt);
  DeceChangeQvalue(dict,lib,cmd.mt,cmd.x,cmd.y);
}


/**********************************************************/
/* CHECKTHRESHOLD, FIXTHRESHOLD                           */
/*      check and fix threshold energies                  */
/* checkthreshold                                         */
/* fixthreshold                                           */
/**********************************************************/
void DeceOperationCHECKTHRESHOLD(ENDFDict *dict, ENDF *lib[])
{
  string ope = CmdGetOperation();
  bool fix = (ope == "fixthreshold") ? true : false;
  DeceCheckThreshold(dict,lib,fix);
}


/**********************************************************/
/* NUTOTAL                                                */
/*      calculate total nu-bar from nu_p and nu_d         */
/* nutotal                                                */
/**********************************************************/
void DeceOperationNUTOTAL(ENDFDict *dict, ENDF *lib[])
{
  DeceCreateLib(dict,1,452);
  DeceCalc452(dict,lib);
}


/**********************************************************/
/* BOUNDCORRECT                                           */
/*      fix boundary energies in MF6                      */
/* boundcorrect                                           */
/**********************************************************/
void DeceOperationBOUNDCORRECT(ENDFDict *dict, ENDF *lib[])
{
  DeceCheckMT(cmd.mt);
  DeceBoundCorrect(dict,lib,cmd.mt);
}


/**********************************************************/
/* DUPLICATEPOINT                                         */
/*      duplicate boundary point in MF6                   */
/* duplicatepoint                                         */
/**********************************************************/
void DeceOperationDUPLICATEPOINT(ENDFDict *dict, ENDF *lib[])
{
  DeceCheckMT(cmd.mt);
  DeceDuplicatePoint(dict,lib,cmd.mt,cmd.x);
}


/**********************************************************/
/* GENPROD                                                */
/*      generate particle production cross section in MF3 */
/* genprod MT ZAP                                         */
/**********************************************************/
void DeceOperationGENPROD(ENDFDict *dict, ENDF *lib[])
{
  DeceCreateLib(dict,3,cmd.mt);
  DeceGenProdCS(dict,lib,cmd.mt,cmd.opt1);
}


/**********************************************************/
/* RECONSTRUCT, RECONANGDIST, SMOOTHANGDIST               */
/*      reconstruct cross section or angular distribution */
/*      from resonance parameters                         */
/* reconstruct emin emax estep                            */
/* reconangdist emin emax estep                           */
/* smoothangdist broadenwidth                             */
/**********************************************************/
void DeceOperationRECONSTRUCT(ENDFDict *dict, ENDF *lib[])
{
  string ope = CmdGetOperation();

  if(dict->getID(2,151) >= 0){
    if(ope == "reconstruct"){
      gfrPtCross(dict,lib,cmd.xmin,cmd.xmax,cmd.x);
    }
    else if(ope == "reconangdist"){
      gfrAngDist(dict,lib,cmd.xmin,cmd.xmax,cmd.x);
    }
    else if(ope == "smoothangdist"){
      gfrAngDistSmooth(dict,lib,cmd.xmin);
    }
  }
}


/**********************************************************/
/* POINTWISE                                              */
/*      reconstruct pointwise cross section from          */
/*      resonance parameters and merge into MF3           */
/**********************************************************/
void DeceOperationPOINTWISE(ENDFDict *dict, ENDF *lib[])
{
  if(dict->getID(2,151) >= 0){
    DeceCreateLib(dict,3,901);
    DeceCreateLib(dict,3,902);
    DeceCreateLib(dict,3,903);
    DeceCreateLib(dict,3,904);
    DeceGeneratePointwise(dict,lib);
  }
}

