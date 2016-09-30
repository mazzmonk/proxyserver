/*
 * proxyserver.hpp
 *
 *  Created on: 2016年7月1日
 *      Author: jack
 */

#ifndef INCLUDE_PROXYSERVER_PROXYSERVER_HPP_
#define INCLUDE_PROXYSERVER_PROXYSERVER_HPP_

#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <vector>

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

  //包装ProxyServerConnection对象，并调用其中的写socket函数
  void WriteRequestsOfClientHandle(ProxyServerConnection::proxser_pointer new_connection,
                                   const boost::system::error_code& error) {
    if (!error) {
      new_connection->WriteConnectionSocket();
    }
    ProcessRequestOfClients();
  }
  boost::asio::ip::tcp::acceptor acceptor_;
};

class ClientOfLaunchRequest {
 public:
  ClientOfLaunchRequest(boost::asio::io_service io_service, boost::asio::ip::tcp::socket socket,
                        std::string urlOfLanuchRequest)
      : io_service_(io_service),
        socket_(socket),
        urlOfLanuchRequest_(urlOfLanuchRequest) {
    std::cout << "ClientOfLaunchRequest begin work." << std::endl;
    ResolveUrl(urlOfLanuchRequest_);
  }
  /*连接远程服务器及端口*/
  void ConnectRemoteServer(char* remoteServerHostname, int remoteServerPort);
  void SendClientOfRequest(std::string& urlOfRequest);
  void ContentOfReponse();

 private:
  ClientOfLaunchRequest() {                       //不允许构造无参数构造函数
  }
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
  std::string urlOfLanuchRequest_;

  /*远程服务器地址，端口，已经请求内容*/
  char* remoteServerHostname_ = "127.0.0.1";
  int remoteServerPort_ = 80;
  std::string pathOfLauchRequest_;

  /*存储从服务器返回内容*/
  const int kMaxBytesOfBuff = 2048;
  std::vector<char> contentOfReponse_(kMaxBytesOfBuff);

  /* 解析url,并且写入成员变量remoteServerHostname_及remoteServerPort_，pathOfLauchRequest_*/
  void ResolveUrl(const std::string& urlOfLanuchRequest);

};

}  //namespace my

#endif /* INCLUDE_PROXYSERVER_PROXYSERVER_HPP_ */
