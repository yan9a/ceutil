
/////////////////////////////////////////////////////////////////////////////
// Project:      testRSA_OS
// Description:  Test program for RSA_OS
/////////////////////////////////////////////////////////////////////////////
#include <string>
#include <cassert>
#include <cstdio>
#include <vector>
#include <iostream>
#include <iomanip>
#include "ce/cersaos.h"
#include "ce/cemisc.h"
using namespace std;
using namespace ce;
// ----------------------------------------------------------------------------
std::string str_hex2byte(std::string str)
{
	string bstr;
	str.erase(remove_if(str.begin(), str.end(), [](char ch) {
		return !(
			(ch >= '0' && ch <= '9') ||
			(ch >= 'A' && ch <= 'Z') ||
			(ch >= 'a' && ch <= 'z')); }), str.end());
	int n = str.length();
	for (int i = 0; i < n; i += 2) 
		bstr += ((char)stoi(str.substr(i, 2), NULL, 16));
	return bstr;
}

std::string str_byte2hex(std::string bstr)
{
    unsigned char u;
    std::stringstream ss;
    for (auto c: bstr) {
        u = (unsigned char)(c);
        ss << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(u);
    }
    return ss.str();
}
// ----------------------------------------------------------------------------
int main() {
    std::string mes = str_hex2byte("6a02541333ff12289999990101f001f984ab5d901597b87806063f77086ecc434a34ac1ae5fe056de000fbc6b9ff85328de9eda93654864929b938f16ac5bcc1988c21b4b024a075c1dcdce73613cd50658b5bc6ca6fd3493043185565659dbd0bb74b37dce662c17c7410b32eb8dc1da119093a0f82a0fca71765c5c16e885cb2d544432a32debf5cf6c80354bc8f71761e33e367b959cfec030c465ead85dc310b6803baca8f70db5b5cf468db1c32941a6ad3f574e6494d7e34e77ca144d3bd21f21ef495e5d24a6ac5d1440e58737e96086297aa4a20b911a8c5f81914c5eebe72c99e700bde265d274522402d467fe1c90d5fa8b5bc");
    std::string d = str_hex2byte("6e09f65f2d1ab8124c390cb04ce89ba55241c4e3695e015280fea181b3f8b71cbdfe1dfda496917e6a30ba4abf39bc2f7cab246c5bee00e1fccc4d1f08aaee65ee6103b915dd21b47c73a834a9a1c6fb1e250b33adcdc40087878ad928d7387f3531563ba9c668a57b734fb6628395e38459dde5854ac0d88177a1633a0356feca9036ed8c4799e811cb5a4a553f0b9d4b882fab9177e41a00c61a310e8cb0d059110b236b88522742fe75e7630a701d8e3cff5bf0d76a1752cc879ffdeebfa954866115d3efa082794af9118cfae460af2aa0f19a73f152239ae190fd50eed40bd9d3a32b9e287311b4de98891999d6a9427d458120b5d3");
    std::string n = str_hex2byte("a50ef18ec3a8141b72559308735ce977fb62a7551e0d01fbc17df2428df512ab1cfd2cfc76e1da3d9f4917701ed69a473b00b6a289e50152fb3273ae8d006598e5918595a0cbb28ebaad7c4efe72aa78ad3790cd84b4a600cb4b5045bd42d4becfca01597ea99cf8392cf79193c560d54686ccd847f02144c2337216724d7d347a960565c210cddffcd79a5b6e0b46de010976d5cb9263569633dfff57d73702141efc0d6549bbfcf3a9a8b122bd1db6afdd9c9206ce4cd4cc612b58d311ff499299477d6ccb8bd356b3187b4188c01527a9da08e8ec543c821a7c43c694f45f0836b89a45b1a0f6ed5f191050d9360c642fa5b514a07bdd");
    std::string e = str_hex2byte("03");
    std::cout << "RSA d (Len "<< d.length() <<"): " << str_byte2hex(d) << std::endl;
    std::cout << "RSA e (Len "<< e.length() <<"): " << str_byte2hex(e) << std::endl;
    std::cout << "RSA n (Len "<< n.length() <<"): " << str_byte2hex(n) << std::endl;

    string cert = ceRSA_OS::decrypt(mes, d,n,e);
    cout<<"Cert: "<<ceMisc::ToStr16(ceMisc::str2cvec(cert),"")<<endl;

    std::string data =  ceRSA_OS::encrypt(cert,n,e);
    cout<<"Data: "<<ceMisc::ToStr16(ceMisc::str2cvec(data),"")<<endl;
    assert(data ==  str_hex2byte("6a02541333ff12289999990101f001f984ab5d901597b87806063f77086ecc434a34ac1ae5fe056de000fbc6b9ff85328de9eda93654864929b938f16ac5bcc1988c21b4b024a075c1dcdce73613cd50658b5bc6ca6fd3493043185565659dbd0bb74b37dce662c17c7410b32eb8dc1da119093a0f82a0fca71765c5c16e885cb2d544432a32debf5cf6c80354bc8f71761e33e367b959cfec030c465ead85dc310b6803baca8f70db5b5cf468db1c32941a6ad3f574e6494d7e34e77ca144d3bd21f21ef495e5d24a6ac5d1440e58737e96086297aa4a20b911a8c5f81914c5eebe72c99e700bde265d274522402d467fe1c90d5fa8b5bc"));
    return 0;
}
