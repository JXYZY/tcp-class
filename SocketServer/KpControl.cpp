#include "KpControl.h"
#include <iostream>
#include<QtCore/QByteArray>
KpControl::KpControl(unsigned short port)
	:XTcp(port)
{

}

KpControl::~KpControl()
{

}

void KpControl::HandleReceiveData(const QByteArray& data)
{
	std::cout << "Data thread id:" << std::this_thread::get_id() << std::endl;
	std::cout << "recv :" << data.data() << std::endl;
	std::cout << "sizeof data.data():" << sizeof(data.data()) << std::endl;
	std::cout << "strlen data.data():" << strlen(data.data()) << std::endl;
	Send(data.data(), (int)strlen(data.data()));
}

void KpControl::close()
{
	Close();
}