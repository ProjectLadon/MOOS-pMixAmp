/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: MixAmp.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <cstdlib>
#include "MBUtils.h"
#include "ACTable.h"
#include "MixAmp.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/schema.h"
#include "rapidjson/writer.h"
#include "rapidjson/error/en.h"
#include "schema/mixer_schema.h"

using namespace std;
using namespace rapidjson;

MixAmpInput::MixAmpInput (rapidjson::Value &v) {
        var = v["name"].GetString();
        if (v.HasMember("gain")) gain = v["gain"].GetDouble();
        if (v.HasMember("offset")) offset = v["offset"].GetDouble();
}

bool MixAmpInput::procMail(CMOOSMsg &msg) {
    if ((msg.GetKey() == var) && (msg.IsDouble())) {
        val = msg.GetDouble();
        return true;
    } else {
        return false;
    }
}

Mixer::Mixer(rapidjson::Value &v) {
    var = v["output"]["name"].GetString();
    if (v["output"].HasMember("gain")) gain = v["output"]["gain"].GetDouble();
    if (v["output"].HasMember("offset")) offset = v["output"]["offset"].GetDouble();
    for (auto &i: v["inputs"].GetArray()) {
        inputs.emplace_back(i);
    }
}

bool Mixer::procMail(CMOOSMsg &msg) {
    bool result = false;
    for (auto &i: inputs) {
        result |= i.procMail(msg);
    }
    return result;
}

std::list<std::string> Mixer::buildReportHeader() {
    std::list<std::string> headers;
    headers.push_back("Output: " + var);
    for (auto &i: inputs) {
        headers.push_back("Input: " + i.getVar());
    }
}

std::list<std::string> Mixer::buildReportLines() {
    std::list<std::string> headers;
    headers.push_back(to_string(cooked()));
    for (auto &i: inputs) {
        headers.push_back(to_string(i.cooked()));
    }
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool MixAmp::OnNewMail(MOOSMSG_LIST &NewMail) {
    AppCastingMOOSApp::OnNewMail(NewMail);

    for (auto &msg: NewMail) {
        string key = msg.GetKey();
        bool result = false;
        for (auto &m: mixers) {
            bool tmp = m.procMail(msg);
            if (tmp) m.transmit(this);
            result |= tmp;
        }
        if(!result || (key != "APPCAST_REQ")) { // handled by AppCastingMOOSApp
            reportRunWarning("Unhandled Mail: " + key);
        }
    }
    return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool MixAmp::OnConnectToServer() {
    registerVariables();
    return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool MixAmp::Iterate() {
    AppCastingMOOSApp::Iterate();
    AppCastingMOOSApp::PostReport();
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool MixAmp::OnStartUp() {
    AppCastingMOOSApp::OnStartUp();
    Document sd;
    // check that we can load and parse the
    if (sd.Parse(reinterpret_cast<char*>(mixer_schema_json), mixer_schema_json_len).HasParseError()) {
        cerr << "Parse error " << GetParseError_En(sd.GetParseError());
        cerr << " in JSON schema at offset " << sd.GetErrorOffset() << endl;
        std::abort();
    }
    SchemaDocument schemadoc(sd);
    SchemaValidator validator(schemadoc);

    STRING_LIST sParams;
    m_MissionReader.EnableVerbatimQuoting(false);
    if(!m_MissionReader.GetConfiguration(GetAppName(), sParams)) {
        reportConfigWarning("No config block found for " + GetAppName());
    }

    for(auto &p: sParams) {

        string orig  = p;
        string line  = p;
        string param = toupper(biteStringX(line, '='));
        string value = line;

        bool handled = false;
        if("MIXER" == param) {
            rapidjson::Document d;
            if (d.Parse(value.c_str()).HasParseError()) {
                cerr << "JSON parse error " << GetParseError_En(d.GetParseError());
                cerr << " in " << param << " at offset " << d.GetErrorOffset() << endl;
                std::abort();
            } else {
                if (d.Accept(validator)) {
                    mixers.emplace_back(d);
                } else {
                    StringBuffer buf;
                    Writer<StringBuffer> writer(buf);
                    d.Accept(writer);
                    cerr << "JSON failed validation:" << endl;
                    cerr << "\t" << buf.GetString() << endl;
                    validator.GetInvalidSchemaPointer().StringifyUriFragment(buf);
                    cerr << "Invalid schema: " << buf.GetString() << endl;
                    cerr << "Invalid keyword: " << validator.GetInvalidSchemaKeyword() << endl;
                    std::abort();
                }
            }
            handled = true;
        }

        if(!handled)
          reportUnhandledConfigWarning(orig);

    }

    registerVariables();
    return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void MixAmp::registerVariables() {
    AppCastingMOOSApp::RegisterVariables();
    for (auto &m: mixers) m.subscribe(this);
}


//------------------------------------------------------------
// Procedure: buildReport()

bool MixAmp::buildReport()
{
  m_msgs << "============================================ \n";
  m_msgs << " pMixAmp                                     \n";
  m_msgs << "============================================ \n";

  list<string> headers;
  list<string> values;

  for (auto &m: mixers) {
      list<string> tmph = m.buildReportHeader();
      for (auto &h: tmph) headers.push_back(h);
      list<string> tmpv = m.buildReportLines();
      for (auto &v: tmpv) values.push_back(v);
  }

  // Assemble the response
  ACTable actab(headers.size());
  for (auto &h: headers) actab << h;
  actab.addHeaderLines();
  for (auto &v: values) actab << v;
  m_msgs << actab.getFormattedString();

  return(true);
}
