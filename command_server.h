#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H
#include <cstdint>
#include <iostream>
#include <memory>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "../rpi/pwm/command_queue.h"
 
using boost::asio::ip::udp;

class CommandServer {
  public:
  
  void Init(boost::asio::io_service& io_service, CommandQueue<PWMSetting>* command_queue); 
  private: 


  void startReceive();
  void handleCommand(const boost::system::error_code& error,
    std::size_t bytes_transferred);

  static const int UDP_PORT = 8000;
  static const int MESSAGE_SIZE = sizeof(int32_t) * 2;
  std::unique_ptr<udp::socket> socket_;
  CommandQueue<PWMSetting>* pwm_queue_; // does not take ownership
  udp::endpoint remote_endpoint_;
  boost::array<char, MESSAGE_SIZE> recv_buffer_; // change to std::array
  uint64_t command_count_ = 0;
};

#endif
