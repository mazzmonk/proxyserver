/*
 * proxyserver.hpp
 *
 *  Created on: 2016年7月1日
 *      Author: jack
 */

#ifndef INCLUDE_PROXYSERVER_PROXYSERVER_HPP_
#define INCLUDE_PROXYSERVER_PROXYSERVER_HPP_


#include <iostream>
#include <vector>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

namespace my {

/* 处理客户端连接以后的socket，比如写入相应的信息*/
class ProxyServerConnection : public boost::enable_shared_from_this<ProxyServerConnection> {
 public:
  typedef boost::shared_ptr<ProxyServerConnection> proxser_pointer;
  static proxser_pointer Create(boost::asio::io_service& io_service) {
    return proxser_pointer(new ProxyServerConnection(io_service));
  }

  boost::asio::ip::tcp::socket& GetSocket() {
    return socket_;
  }

  //异步写socket
  void WriteConnectionSocket();

 private:
  ProxyServerConnection(boost::asio::io_service& io_service)
      : socket_(io_service) {
  }

  //异步写socket时使用的句柄函数
  void WriteConnectionSocketHandle(const boost::system::error_code& /*error*/,
                                   std::size_t /*bytes_transferred*/) {
  }
  boost::asio::ip::tcp::socket socket_;
  std::string sendmessage_;
};

/*绑定地址及端口，监听client的socket连接，发送消息给连接的socket*/
class ProxyServer {
 public:
  ProxyServer(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint& endpoint)
      : acceptor_(io_service, endpoint) {
    ProcessRequestOfClients();
  }

 private:
  //监听新的socket连接
  void ProcessRequestOfClients();

  //包装ProxyServerConnection对象，并调用该对象的写socket函数
  void WriteRequestsOfClientHandle(ProxyServerConnection::proxser_pointer new_connection,
                                   const boost::system::error_code& error) {
    if (!error) {
      new_connection->WriteConnectionSocket();
    }
    ProcessRequestOfClients();
  }
  boost::asio::ip::tcp::acceptor acceptor_;
};



}   //namespace my

#endif /* INCLUDE_PROXYSERVER_PROXYSERVER_HPP_ */
