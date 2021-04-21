/////////////////////////////////////////////////////////////////////////////
// Name:         ceFraCmd.h
// Description:  Byte stuffing- sending and receiving commands as frames
// Author:       Yan Naing Aye
// Ref: http://cool-emerald.blogspot.com/2011/05/byte-stuffing.html
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_FRACMD_H
#define CE_FRACMD_H
#include "ce/ceFrame.h"
namespace ce {
//-----------------------------------------------------------------------------
class ceFraCmd : public ceFrame {
public:
	ceFraCmd();
	size_t SetTxFrame(char* d, size_t n) override;
	size_t ReceiveRxFrame(char ch) override;//get receiving frame from received char
};
//-----------------------------------------------------------------------------
} // namespace ce 

#endif // CE_FRACMD_H