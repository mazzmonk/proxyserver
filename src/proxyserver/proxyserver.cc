/*
 * proxyserver.cc
 *
 *  Created on: 2016年7月1日
 *      Author: jack
 */

#include "proxyserver/proxyserver.hpp"

std::string hello() {
  std::string s("test success.\n");
  return s;
}

void my::ProxyServerConnection::ProcessConnection() {
  sendmessage_ = hello();
  boost::asio::async_write(
      socket_,
      boost::asio::buffer(sendmessage_),
      boost::asio::transfer_at_least(32),
      boost::bind(&ProxyServerConnection::WriteConnectionHandle, shared_from_this(),
                  boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
}

void my::ProxyServer::ProcessAccept() {
  ProxyServerConnection::proxser_pointer newConnection = ProxyServerConnection::Create(
      acceptor_.get_io_service());
  acceptor_.async_accept(
      newConnection->GetSocket(),
      boost::bind(&ProxyServer::WriteAcceptHandle, this, newConnection,
                  boost::asio::placeholders::error));
}

