/*
 * launchrequest.hpp
 *
 *  Created on: 2016年11月21日
 *      Author: jack
 */

#ifndef SERVICE_LAUNCHREQUEST_HPP_
#define SERVICE_LAUNCHREQUEST_HPP_

#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

namespace my {

/* 作为客户端向远程服务器发送http请求，模仿访问网站行为
 *
 */
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

  /* 构造http请求*/
  void SendClientOfRequest(std::string& urlOfRequest);
  void ContentOfReponse();

 private:
  ClientOfLaunchRequest() {                       //不允许构造无参数构造函数
  }
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::socket socket_;
  std::string urlOfLanuchRequest_;

  /*远程服务器地址，端口，请求内容（path）*/
  char* remoteServerHostname_ { "127.0.0.1" };
  int remoteServerPort_ { 80 };
  std::string pathOfLauchRequest_;

  /*存储从服务器返回内容*/
  const int kMaxBytesOfBuff = 2048;
  std::vector<char> contentOfReponse_(kMaxBytesOfBuff);

  /* 拆分url，格式为以下几种
   * 1.domain.com/abc.jgp
   * 2.domain.com:8080/abc.jpg
   * 3.domain.com
   * 4.domain.com:8080
   */
  void SplitUrl(const std::string& urlOfLanuchRequest);

  /* 解析url,并且写入数据成员remoteServerHostname_及remoteServerPort_，pathOfLauchRequest_
   * url分为2种，带有http（https）头及不带此头的，可以把带头的去掉，用SplitUrl函数统一处理
   */
  void ResolveUrl(std::string& urlOfLanuchRequest);

};

}   //namespace my

#endif /* SERVICE_LAUNCHREQUEST_HPP_ */
