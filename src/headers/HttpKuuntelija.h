#ifndef HttpKuuntelija_H
#define HttpKuuntelija_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>

class HttpKuuntelija : public Poco::Net::HTTPRequestHandler
{
public:
  virtual void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp);
};

#endif
