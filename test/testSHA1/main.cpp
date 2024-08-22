
/////////////////////////////////////////////////////////////////////////////
// Project:      testSHA1
// Description:  Test program for SHA1
/////////////////////////////////////////////////////////////////////////////
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <iostream>
#include <iomanip>
#include "ce/ceSHA1.h"
#include "ce/ceMisc.h"
using namespace std;
using namespace ce;
int main() {
    std::string message = "12345678";
    ceSHA1 mes_sha;
    mes_sha.Init();
    mes_sha.Update(message);
    std::string mshastr =  mes_sha.Final();
    std::string mssh = ceMisc::ToStr16(ceMisc::str2cvec(mshastr),"");
    cout<<"SHA1: "<<mssh<<endl;
    assert(mssh ==  "7C222FB2927D828AF22F592134E8932480637C0D");
    return 0;
}
