/////////////////////////////////////////////////////////////////////////////
// Name:         ceInterface.h
// Description:  Interface to communitcate modules
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////
#include "ce/ceInterface.h"
namespace ce {
//-----------------------------------------------------------------------------
ceInterface::ceInterface() {

}
//-----------------------------------------------------------------------------
ceInterface::ceInterface(std::function<void(std::tuple<std::vector<char>,std::string,int>)> cbf): _cbf(cbf) {
	
}
//-----------------------------------------------------------------------------
void ceInterface::setRxCallback(std::function<void(std::tuple<std::vector<char>,std::string,int>)> cbf) {
	_cbf = cbf;
}
//-----------------------------------------------------------------------------
void ceInterface::check(){
	
}
//-----------------------------------------------------------------------------
} // namespace ce 