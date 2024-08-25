#include <wx/wx.h>
#include "ce/cersaos.h"
#include "ce/celog.h"
#include "ce/cemisc.h"
#include <vector>

#include <iostream>
using namespace ce;
using namespace std;
int main(){
	ceLog l("./",30);
	int bit_len,exp;

	cout << "Enter bit length of the key [2048]: ";
	cin >> bit_len;

	cout << "Enter exponent [3]: ";
	cin >> exp;
	// RSA* private_key = ceRSA_OS::generateKeyPair("public_key.pem", "private_key.pem", bit_len, exp); // use this to save key files
	RSA* private_key = ceRSA_OS::generateKeyPair(bit_len, exp);

	string dstr;
	string estr;
	string nstr;
	ceRSA_OS::getBN(private_key, dstr, nstr, estr);
	std::cout << "RSA d (Len " << dstr.length() << "): " << ceMisc::ToStr16(dstr) << std::endl;
	std::cout << "RSA e (Len " << estr.length() << "): " << ceMisc::ToStr16(estr) << std::endl;
	std::cout << "RSA n (Len " << nstr.length() << "): " << ceMisc::ToStr16(nstr) << std::endl;

	return 0;
}
