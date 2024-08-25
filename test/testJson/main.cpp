
/////////////////////////////////////////////////////////////////////////////
// Project:      testMsg
// Description:  Test program for Json
/////////////////////////////////////////////////////////////////////////////
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <iostream>
#include <iomanip>
#include "ce/ceconf.h"
using namespace std;
using namespace ce;
int main() {
    int i = 0;
    string jstr = R"({"id":1})";
    Json::Value obj = ceConfig::GetJson(jstr);
    if(!obj["id"].isNull()){
        i = obj["id"].asInt();
        cout<<"ID = "<<i<<endl;
    }
    else {
        cout<<"ID not found"<<endl;
    }

    if(!obj["val"].isNull()){
        cout<<"Val = "<<obj["val"].asInt()<<endl;
    }
    else {
        cout<<"Val not found"<<endl;
    }

    assert(i==1);
    return 0;
}
