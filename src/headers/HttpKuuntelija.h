#ifndef HttpKuuntelija_H
#define HttpKuuntelija_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <istream>
#include <string>

class HttpKuuntelija : public Poco::Net::HTTPRequestHandler
{
 public:
  virtual void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp);
 private:
  void lataa(Poco::Net::HTTPServerResponse &resp);
  void ennuste(Poco::Net::HTTPServerResponse &resp);
  void opeta(Poco::Net::HTTPServerResponse &resp);
  void valikko(Poco::Net::HTTPServerResponse &resp);
  void FMIkysely(std::string kutsu, std::string tiedosto);
  void parsiDataa(std::string lukutiedosto, std::string kirjoitustiedosto, std::string avainsana);
};

#endif
