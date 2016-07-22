#ifndef HttpKuuntelija_H
#define HttpKuuntelija_H

#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <iostream>
#include <string>

class HttpKuuntelija : public Poco::Net::HTTPRequestHandler
{
 public:
  virtual void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp);
 private:
  // Vastaavat HTTP-kyselyihin:
  void lataa(Poco::Net::HTTPServerResponse &resp);
  void ennuste(Poco::Net::HTTPServerResponse &resp);
  void opeta(Poco::Net::HTTPServerResponse &resp);
  void valikko(Poco::Net::HTTPServerResponse &resp);
  // Toteuttavat muun toiminnallisuuden:
  void FMIkysely(std::string kutsu, std::string tiedosto);
  void parsiDataa(std::string lukutiedosto, std::string kirjoitustiedosto, std::string avainsana);
  void luokitteleDataa(std::istream& lukutiedosto, std::ostream& kirjoitustiedosto1, std::ostream& kirjoitustiedosto2);
  void tulostaRivi(int kellonaika, std::vector<double> datat1, std::vector<double> datat2, std::vector<double> datat3, std::ostream& tiedosto);
  double s2d(std::string s);
};

#endif
