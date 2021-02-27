/////////////////////////////////////////////////////////////////////////////
// Name:         ceUtil.h
// Description:  CPP utilitiy modules 
// Author:       Yan Naing Aye
/////////////////////////////////////////////////////////////////////////////

#ifndef CE_UTIL_H
#define CE_UTIL_H

#include "ceMacros.h" // macros

// The latest soure for ceDateTime is at
//    https://github.com/yan9a/mmcal/tree/master/cpp
#include "ce/ceDateTime.h"
#include "ce/ceMmDateTime.h"

// Log
#include "ce/ceLog.h"

// The latest source for ceSerial is at
//   https://github.com/yan9a/serial
#include "ce/ceSerial.h"

// timer
#include "ce/ceWxTmr.h"

// udp
// http://cool-emerald.blogspot.com/2018/01/udptcp-socket-programming-with-wxwidgets.html
#include "ce/ceUDP.h"

// utilities
#include "ce/ceMisc.h"

// Frame encode and decode
#include "ce/ceFrame.h"

// I2C communication module
#include "ce/ceI2C.h"

// PCA9535
#include "ce/cePCA9535.h"

// Config
#include "ce/ceConfig.h"

#endif // CE_UTIL_H
