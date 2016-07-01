/*
 * main.cc
 *
 *  Created on: 2016年7月1日
 *      Author: jack
 */
#include  "proxyserver/proxyserver.hpp"
#include <iostream>

int main() {
  try {
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 3547);
    my::ProxyServer proxyServerr(io_service,endpoint);
    io_service.run();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}


