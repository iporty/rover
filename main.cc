//
// main.cpp
// ~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <boost/asio.hpp>

#include<boost/regex.hpp>
#include "server.h"
#include "../rpi/pwm/pwm.h"
#include "../rpi/pwm/command_queue.h" 
#include <boost/asio.hpp>
#include "command_server.h"

using boost::asio::ip::udp;


int startHttpServer(int argc, char* argv[]) {
  try
  {
    // Check command line arguments.
    if (argc != 4)
    {
      std::cerr << "Usage: http_server <address> <port> <doc_root>\n";
      std::cerr << "  For IPv4, try:\n";
      std::cerr << "    receiver 0.0.0.0 80 .\n";
      std::cerr << "  For IPv6, try:\n";
      std::cerr << "    receiver 0::0 80 .\n";
      return 1;
    }

    // Initialise the server.
    http::server::server s(argv[1], argv[2], argv[3]);

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }
	return 0;
}

int startUdpServer(int port) {
  
  return 0;
}

class Rover {
	public:

  Rover() {}

  void InitPwmQueue() {
    std::cerr << "R1";

		pwm_queue_.Start();
    std::cerr << "R2";
  }  	

  void InitCommandServer() {
    command_server_.Init(io_service_, &pwm_queue_);
    io_service_.run();  
  }
	
  void Init() {
    InitPwmQueue(); 
    InitCommandServer();
	};

  void ServoTest() {
    PWMSetting setting{0, .3, 0};
    pwm_queue_.AddCommand(setting);
    
    PWMSetting setting2{0, 1.2, 0};
    pwm_queue_.AddCommand(setting2);
	}
	private:
  
  pwm pwm_;
  CommandQueue<PWMSetting> pwm_queue_;
  boost::asio::io_service io_service_;
  CommandServer command_server_; // udp server that listens for commands
};

void testCommandQueue() {
  CommandQueue<PWMSetting> pwm_setting_queue;
	pwm_setting_queue.Start();
  
	float frac = .5;
  typedef std::istream_iterator<float> in;
  in eos;
  in it(std::cin);
	while (it != eos) {
    if (*it == 0) {
      break;
    } 
    frac = *it;
    ++it;
		auto wait = *it;
    printf("sending to cq frac %f\n\r", frac);
	 	PWMSetting setting{0, frac, static_cast<int64_t>(wait)};
    pwm_setting_queue.AddCommand(setting);
    ++it;
  }
}

void regex_test() {
  std::string line;
    boost::regex pat( "^Subject: (Re: |Aw: )*(.*)" );

    while (std::cin)
    {
        std::getline(std::cin, line);
        boost::smatch matches;
        if (boost::regex_match(line, matches, pat))
            std::cout << matches[2] << std::endl;
    }
}

int main(int argc, char* argv[])
{
  std::cerr << "1";
  Rover rover;
  std::cerr << "2";
   rover.Init();
  std::cerr << "3";
   //rover.ServoTest();
  std::cerr << "4";
  //testCommandQueue();
  return 0;
}
