/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: MixAmp.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef MixAmp_HEADER
#define MixAmp_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"

class MixAmp : public AppCastingMOOSApp
{
 public:
   MixAmp();
   ~MixAmp();

 protected: // Standard MOOSApp functions to overload  
   bool OnNewMail(MOOSMSG_LIST &NewMail);
   bool Iterate();
   bool OnConnectToServer();
   bool OnStartUp();

 protected: // Standard AppCastingMOOSApp function to overload 
   bool buildReport();

 protected:
   void registerVariables();

 private: // Configuration variables

 private: // State variables
};

#endif 
