/*
 * proxyserver.cc
 *
 *  Created on: 2016年7月1日
 *      Author: jack
 */

#include "proxyserver/proxyserver.hpp"
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

/*
 void ConnectRemoteServer(const char* remoteServerHostname, int remoteServerPort);
 void SendClientOfRequest(std::string& contentOfRequest);
 void ContentOfReponse();
 */

/*连接远程服务器地址和端口，绑定socket_*/
void my::ClientOfLaunchRequest::ConnectRemoteServer(char* remoteServerHostname,
                                                    int remoteServerPort) {
  remoteServerHostname = remoteServerHostname_;
  remoteServerPort = remoteServerPort_;
  boost::asio::ip::tcp::resolver resolver(socket_.get_io_service());
  boost::asio::ip::tcp::resolver::query queryEndpoints(
      remoteServerHostname, boost::lexical_cast<std::string>(remoteServerPort));
  boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(queryEndpoints);
  boost::asio::ip::tcp::resolver::iterator end;
  boost::system::error_code ec = boost::asio::error::host_not_found;

  for (; ec && endpoint_iterator != end; ++endpoint_iterator) {
    socket_.close();
    socket_.connect(*endpoint_iterator, ec);
  }
  if (ec) {
    std::cout << "can't connect." << std::endl;
    throw boost::system::system_error(ec);
  }
  std::cout << "connect success." << std::endl;
}

/*构造请求*/
void my::ClientOfLaunchRequest::ResolveUrl(const std::string& urlOfLanuchRequest) {
    std::string url = urlOfLanuchRequest;
    std::size_t first_match_pos = url.find_first_of("/");                          //查询第一个"/"出现的pos值
    std::string urlProtocol = url.substr(0, (first_match_pos - 1));          //得到url请求的头，http或者https

    /*分解出url中的host，port，内容*/
    const std::string requestHttp("http");
    const std::string requestHttps("https");
    if (urlProtocol.compare(requestHttp) == 0 || urlProtocol.compare(requestHttps) == 0) {
      std::size_t second_match_pos = url.find_first_of("/", first_match_pos + 1);  //查询第二个"/"出现的pos值
      std::size_t third_match_pos = url.find_first_of("/", second_match_pos + 1);  //查询第三个"/"出现的pos值

      /*   */
      remoteServerHostname_ = url.substr(second_match_pos + 1,
                                         third_match_pos - second_match_pos - 1);

      /* */
      pathOfLauchRequest_ = url.substr(third_match_pos);


    }


}

void my::ClientOfLaunchRequest::SendClientOfRequest(std::string& urlOfRequest) {
  std::string requestMethod = "GET";
  std::string urlOfLanuchRequest_;
  std::string httpVersion = "HTTP/1.0\r\n";
  std::string hostname = "";

}

void my::ClientOfLaunchRequest::ContentOfReponse() {

}

