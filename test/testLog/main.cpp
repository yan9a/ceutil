
/////////////////////////////////////////////////////////////////////////////
// Project:      testLog
// Description:  Test program for ceLog
/////////////////////////////////////////////////////////////////////////////
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <iostream>
#include <iomanip>
#include "ce/celog.h"
#include <wx/wx.h>
using namespace std;
using namespace ce;
int main() {
    int i = 0;
    i++;

    ceLog l("../",1);
    l.Print("Test");

    ceDateTime d;
    double jd = l.LastModified("../L"+d.ToString("%yyyy-%mm-%dd")+".log");
    string nowstr = d.ToString("%yyyy-%mm-%dd %HH:%nn:%ss");
    printf("Now: %s\n", nowstr.c_str());

    wxMilliSleep(3000);//sleep 3 s

    d.SetJD(jd);
    string str = d.ToString("%yyyy-%mm-%dd %HH:%nn:%ss");
    printf("Last modified: %s\n", str.c_str());

    l.Clean("../",".log",2.0d);

    assert(i==1);
    return 0;
}
