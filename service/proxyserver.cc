/*
 * proxyserver.cc
 *
 *  Created on: 2016年7月1日
 *      Author: jack
 */

#include "proxyserver.hpp"
#include "boost/lexical_cast.hpp"

//写socket用测试函数
std::string hello() {
  std::string s("test success.\n");
  return s;
}

void my::ProxyServerConnection::WriteConnectionSocket() {
  sendmessage_ = hello();
  boost::asio::async_write(
      socket_,
      boost::asio::buffer(sendmessage_),
      boost::asio::transfer_at_least(32),
      boost::bind(&ProxyServerConnection::WriteConnectionSocketHandle, shared_from_this(),
                  boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void my::ProxyServer::ProcessRequestOfClients() {
  ProxyServerConnection::proxser_pointer newConnection = ProxyServerConnection::Create(
      acceptor_.get_io_service());
  acceptor_.async_accept(
      newConnection->GetSocket(),
      boost::bind(&ProxyServer::WriteRequestsOfClientHandle, this, newConnection,
                  boost::asio::placeholders::error));
}


