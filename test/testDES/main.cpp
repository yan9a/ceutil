
/////////////////////////////////////////////////////////////////////////////
// Project:      testDES
// Description:  Test program for DES
/////////////////////////////////////////////////////////////////////////////
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <iostream>
#include <iomanip>
#include "ce/ceDES.h"
#include "ce/ceMisc.h"
using namespace std;
using namespace ce;
int main() {
    std::string message = "12345678";
    std::string keyL = "abcdefgh";
    std::string keyR = "ijklmnop";
    std::string estr =  ceDES::Encrypt(message,keyL);
    std::string estrh = ceMisc::ToStr16(ceMisc::str2cvec(estr),"");
    cout<<"DES: "<<estrh<<endl;
    assert(estrh ==  "21C60DA534248BCE");
    cout<<"DES decrypted: "<<ceDES::Decrypt(estr,keyL)<<endl;

    std::string testr =  ceDES::Encrypt(message,keyL+keyR);
    std::string testrh = ceMisc::ToStr16(ceMisc::str2cvec(testr),"");
    cout<<"TDES: "<<testrh<<endl;
    assert(testrh ==  "BA0C49533453E94F");
    cout<<"TDES decrypted: "<<ceDES::Decrypt(testr,keyL+keyR)<<endl;
    return 0;
}
