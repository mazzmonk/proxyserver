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

/* 连接远程服务器地址和端口，绑定socket_*/
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

/* 构造请求，有如下的几种情况
 *  1.www.abc.com
 *  2.www.abc.com/1.jpg
 *  3.www.abc.com:8080
 *  4.www.abc.com:8080/1.jpg
 *  解析上述的url
 */
void my::ClientOfLaunchRequest::SplitUrl(const std::string& urlOfLanuchRequest) {
  std::string url = urlOfLanuchRequest;

  /* XXX 检查是否包含":"， 即是否包含端口 */
  std::size_t isFound = url.find(":");
  if (isFound != std::string::npos) {
    remoteServerHostname_ = url.substr(0, isFound);
    remoteServerPort_ = url.substr(isFound + 1);

    /* 检查是否包含path部分*/
    std::size_t is_first_match_colon_pos = url.find_first_of("/");
    if (is_first_match_colon_pos != std::string::npos) {
      pathOfLauchRequest_ = url.substr(is_first_match_colon_pos);
    }
    pathOfLauchRequest_ = "";
  }

  /* XXX 不包含":"时候，检查是否有path部分 */
  std::size_t is_first_match_colon_pos = url.find_first_of("/");
  if (is_first_match_colon_pos != std::string::npos) {
    remoteServerHostname_ = url.substr(0, is_first_match_colon_pos);
    pathOfLauchRequest_ = url.substr(is_first_match_colon_pos);
  }
  remoteServerHostname_ = url;
  pathOfLauchRequest_ = "";
}

/* 主要处理url是否带有http/https头 */
void my::ClientOfLaunchRequest::ResolveUrl(std::string& urlOfLanuchRequest) {
  std::string url = urlOfLanuchRequest;

  /* 对比url开头是否是以http或者https开头 */
  const std::string requestHttp("http");
  const std::string requestHttps("https");
  if (url.compare(requestHttp) == 0 || url.compare(requestHttps) == 0) {
    std::size_t first_match_colon_pos = url.find_first_of(":");     //第一个匹配的冒号的pos值

    std::string splitedUrl = url.substr(first_match_colon_pos + 3);
    SplitUrl(splitedUrl);
  }
  SplitUrl(url);
}

/* 构造GET头，并发送*/
void my::ClientOfLaunchRequest::SendClientOfRequest(std::string& urlOfRequest) {
  std::string requestMethod = "GET";
  std::string httpVersion = "HTTP/1.0\r\n";
  std::string hostname = "Host" + remoteServerHostname_ + "\r\n";
  std::string accept = "Accept: */*\r\n";
  std::string status = "Connection: close\r\n\r\n";

  std::string getHead = requestMethod + urlOfRequest + httpVersion + hostname + accept + status;

  std::cout << getHead << std::endl;

  socket_(io_service_);
  ConnectRemoteServer(remoteServerHostname_, remoteServerPort_);
  size_t length = socket_.write_some(boost::asio::buffer(getHead));

  std::cout << "send data length: " << length << std::endl;

}

void my::ClientOfLaunchRequest::ContentOfReponse() {

}

