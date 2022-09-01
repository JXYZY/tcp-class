#pragma once
#include "XTcp.h"
class TcpThread
{
public:
	TcpThread(XTcp clientXTcp);
	~TcpThread();
	void TcpHandlerClient();

private:
	XTcp clientXTcp;
};