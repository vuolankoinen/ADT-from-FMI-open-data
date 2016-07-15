#ifndef rajapinnat_H
#define rajapinnat_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPServerRequest.h>

class MyRequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory
{
public:
  virtual Poco::Net::HTTPRequestHandler* createRequestHandler(const Poco::Net::HTTPServerRequest &);
};

#endif
