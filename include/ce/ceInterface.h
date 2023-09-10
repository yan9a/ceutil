/////////////////////////////////////////////////////////////////////////////
// Name:         ceInterface.h
// Description:  Interface to communitcate modules
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_INTERFACE_H
#define CE_INTERFACE_H

#include <cstdint>
#include <string>
#include <queue>
#include <vector>
#include <algorithm>
#include <tuple>
#include <functional>

namespace ce {
class ceInterface {
public:
	ceInterface();
	ceInterface(ceInterface* if, std::function<void(std::tuple<std::vector<char>,std::string,int>)> cbf);
	virtual ~ceInterface();
	void check(); // to regularly check message queue (e.g. 10 ms) to trigger callback function
	int send(std::tuple<std::vector<char>,std::string,int>);
	void setRxCallback(std::function<void(std::tuple<std::vector<char>,std::string,int>)> cbf);
	void setIf2Connect(ceInterface* if);
private:
	std::queue<std::tuple<std::vector<char>,std::string,int>> _mes;
	std::function<void(std::tuple<std::vector<char>,std::string,int>)> _cbf;
	ceInterface* _if = nullptr; 
};


} // namespace ce 
#endif // CE_INTERFACE_H
