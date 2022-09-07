#pragma once
#include "XTcp.h"
//kp的上层服务
class KpControl :public XTcp
{
public:
	KpControl(unsigned short port);
	~KpControl();

	void close();

protected:
	void HandleReceiveData(const QByteArray& data)override;
};