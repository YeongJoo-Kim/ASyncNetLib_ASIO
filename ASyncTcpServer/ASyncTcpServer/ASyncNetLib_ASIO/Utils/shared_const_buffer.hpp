
#pragma once
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>

#include <string>
#include <vector>
#if 0
class shared_const_buffer {
public:
	explicit shared_const_buffer(const std::string &data) : m_data(new std::vector<char>(data.begin(), data.end())),
		m_buffer(boost::asio::buffer(*m_data)) {}
	~shared_const_buffer(){
		std::cout << "call destructor of shared_const_buffer" << endl;
	}
public:
	typedef boost::asio::const_buffer value_type;
	typedef const boost::asio::const_buffer *const_iterator;
	const boost::asio::const_buffer *begin() const { return &m_buffer; }
	const boost::asio::const_buffer *end() const { return &m_buffer + 1; }
private:
	boost::shared_ptr< std::vector<char> > m_data;
	boost::asio::const_buffer m_buffer;
};
#else
class shared_const_buffer {
public:
	explicit shared_const_buffer(const uint8_t* pData, std::size_t len) : data_(new std::vector<uint8_t>(pData, pData + len)),
		buffer_(boost::asio::buffer(*data_)),
		data_length(len){
			//std::cout << "call constructor of shared_const_buffer " << this << std::endl;
	}
/*
	shared_const_buffer(const shared_const_buffer& t)
	{
		std::cout << "call copy constructor of shared_const_buffer " << this << std::endl;
	}
	*/
	~shared_const_buffer() {
		//std::cout << "call destructor of shared_const_buffer " << this << std::endl;
	}
public:
	typedef boost::asio::const_buffer value_type;
	typedef const boost::asio::const_buffer *const_iterator;
	const boost::asio::const_buffer *begin() const { return &buffer_; }
	const boost::asio::const_buffer *end() const { return &buffer_ + 1; }
private:
	boost::shared_ptr< std::vector<uint8_t> > data_;
	boost::asio::const_buffer buffer_;
	std::size_t data_length;
};

#endif