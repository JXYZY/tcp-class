#pragma once
#include "XTcp.h"
//kp���ϲ����
class KpControl :public XTcp
{
public:
	KpControl(unsigned short port);
	~KpControl();

	void close();

protected:
	void HandleReceiveData(const QByteArray& data)override;
};