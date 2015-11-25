#include "command_server.h"
#include<iostream>
#include <boost/bind.hpp>
#include "../rpi/pwm/command_queue.h"

void CommandServer::startReceive() {
  socket_->async_receive_from(
    boost::asio::buffer(recv_buffer_), remote_endpoint_,  
    boost::bind(&CommandServer::handleCommand, this,
      boost::asio::placeholders::error,
      boost::asio::placeholders::bytes_transferred));
}

void CommandServer::Init(boost::asio::io_service& io_service, 
    CommandQueue<PWMSetting>* command_queue) {
  socket_.reset(new udp::socket(io_service, udp::endpoint(udp::v4(), UDP_PORT)));
  pwm_queue_ = command_queue;
  startReceive();
}

void CommandServer::handleCommand(const boost::system::error_code& error,
    std::size_t bytes_transferred) {
  ++command_count_;
  //std::cerr << "Received " << bytes_transferred << " count: " << command_count_ << std::endl;
  if (error) {
    startReceive();
    return;  
  }
  if (bytes_transferred != MESSAGE_SIZE) {
    std::cerr << "Incompleted Message " << bytes_transferred << std::endl;
    startReceive();
    return;
  }
  int32_t* data = reinterpret_cast<int32_t*>(&recv_buffer_);
  int32_t port = data[0];
  int32_t setting = data[1];
  //std::cerr << "Port:" << port << " Setting:" << setting << std::endl;
  float frac = static_cast<float>(setting)/100.0;
	PWMSetting pwm_setting{port, frac, 0};
  pwm_queue_->AddCommand(pwm_setting);
  startReceive(); 
}
