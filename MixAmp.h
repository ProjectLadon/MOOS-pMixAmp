/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: MixAmp.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef MixAmp_HEADER
#define MixAmp_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include <vector>
#include <memory>
#include <string>

class Mixer;    // Forward declaration

class MixAmp : public AppCastingMOOSApp {
    public:
        MixAmp() {};
        ~MixAmp() {};
        bool notify(std::string var, double val) {
            return Notify(var, val);
        };
        bool registerVar(std::string var) {
            return Register(var);
        };

    protected: // Standard MOOSApp functions to overload
        bool OnNewMail(MOOSMSG_LIST &NewMail);
        bool Iterate();
        bool OnConnectToServer();
        bool OnStartUp();

        // Standard AppCastingMOOSApp function to overload
        bool buildReport();
        void registerVariables();

    private: // State variables
        std::vector<Mixer> mixers;
};

class MixAmpInput {
    public:
        MixAmpInput(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg);
        bool subscribe(MixAmp* m) {m->registerVar(var);};
        double raw() {return val;};
        double cooked() {return ((val + offset) * gain);};
        const std::string &getVar() {return var;};

    private:
        double gain = 1;
        double offset = 0;
        double val = 0;
        std::string var = "";
	std::string var_upper = "";
};

class Mixer {
    public:
        Mixer(rapidjson::Value &v);
        bool procMail(CMOOSMsg &msg);
        bool subscribe (MixAmp *m) {for (auto &i: inputs) i.subscribe(m);};
        bool transmit (MixAmp *m) {return m->notify(var, cooked());};
        std::list<std::string> buildReportHeader();
        std::list<std::string> buildReportLines();
	void sum();
        double raw() {return output;};
        double cooked() {return ((output + offset) * gain);};

    private:
        double gain = 1;
        double offset = 0;
        double output = 0;
        std::string var = "";
        std::vector<MixAmpInput> inputs;
};

#endif
