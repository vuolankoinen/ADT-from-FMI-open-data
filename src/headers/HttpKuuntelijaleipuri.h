#ifndef HttpKuuntelijaleipuri_H
#define HttpKuuntelijaleipuri_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

struct HttpKuuntelijaleipuri : public Poco::Net::HTTPRequestHandlerFactory
{
  virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest &);
};

#endif
