#pragma once
#include<QtCore/QByteArray>

template <typename T> auto checkDataComplete(const T& t)noexcept
{
	return t.isNotComplete();
}

//将来根据协议订数据包的格式，数据包单独用类来表示
template <typename T> auto parseReceiveData(const QByteArray& receive_data)noexcept
{
	using DataPacketPtr = std::shared_ptr<T>;
	using DataPacketPtrList = std::list<DataPacketPtr>;
	thread_local QByteArray last_data;
	auto& data = last_data.isEmpty() ? receive_data : last_data.append(receive_data);
	QDataStream in(data);
	DataPacketPtrList packet_list;
	while (!in.atEnd())
	{
		typename T::HeadType head;
		while (!in.atEnd())
		{
			in >> head;
			uint64_t remain_data_size = in.device()->bytesAvailable() + sizeof(typename T::HeadType);
			if (head == T::HEAD)
			{
				DataPacketPtr recivePacket(new T);
				in >> (*recivePacket);
				if (recivePacket->isInvalid())
				{
					break;
				}
				if (checkDataComplete<T>(*recivePacket))
				{
					//处理断包，需要包含isNotComplete函数
					last_data = data.right(remain_data_size);
					return packet_list;
				}
				packet_list.push_back(recivePacket);
			}
		}
	}
	if (!last_data.isEmpty())
	{
		last_data.clear();
	}
	return packet_list;
}