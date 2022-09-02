#include "KpControl.h"
#include <iostream>
KpControl::KpControl(unsigned short port)
	:XTcp(port)
{

}

KpControl::~KpControl()
{

}

void KpControl::HandleReceiveData(const char* data)
{
	std::cout << "recv :" << data << std::endl;
}

void KpControl::close()
{
	Close();
}