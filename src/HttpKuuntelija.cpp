/* Hoitaa palvelimen saamat HTTP-kutsut.*/

#include "HttpKuuntelija.h"
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Util/ServerApplication.h>
#include <iostream>
#include <fstream>
#include <Poco/Net/HTTPClientSession.h>
#include <Poco/StreamCopier.h>
#include <string>
#include <vector> 
#include <Poco/SAX/ContentHandler.h>
#include <Poco/SAX/SAXParser.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/SAX/LexicalHandler.h>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/Locator.h>

#include "XmlLukija.h"   //XmlLukija

void HttpKuuntelija::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
  {
    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);

    if (req.getURI() == "/lataa/") {lataa(resp); return;}
    if (req.getURI() == "/opeta-algoritmi/") {opeta(resp); return;}
    if (req.getURI() == "/ennuste/") {ennuste(resp); return;}

    valikko(resp);  //Muihin osoitteisiin tulevat kutsut.
    
  }

void HttpKuuntelija::lataa(Poco::Net::HTTPServerResponse &resp)
{
      resp.setContentType("text/html");
      std::ostream& ulos = resp.send();
      std::string salausavain;
      std::ifstream salasanatiedosto;
      salasanatiedosto.open("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/fmi_avain");
      getline(salasanatiedosto, salausavain);
      salasanatiedosto.close();
      
      std::string kutsu = "/fmi-apikey/" + salausavain + "/wfs?request=getFeature&storedquery_id=fmi::observations::weather::multipointcoverage&place=Kaskinen&timestep=120";
      FMIkysely(kutsu, "/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/Kaskinen.xml");
      kutsu = "/fmi-apikey/" + salausavain + "/wfs?request=getFeature&storedquery_id=fmi::observations::weather::multipointcoverage&place=Helsinki&timestep=120";
      FMIkysely(kutsu, "/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/Helsinki.xml");

      parsiDataa("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/Kaskinen.xml", 
		 "/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/Kaskinen.data", 
		 "DataBlock");
      parsiDataa("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/Helsinki.xml", 
		 "/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/Helsinki.data", 
		 "DataBlock");

  ulos << "<h1>Heips!</h1>"
       << "<p>Valitse vaihtoehdoista:</p>"
       << "<p>Datat ladattu ja tallennettu.</p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/opeta-algoritmi/\"><b>Opeta algoritmi</b> ladattujen tietojen perusteella.</a></p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/ennuste/\"><b>Ennusta huomisen sadetilanne</b> Ilmatieteenlaitoksen tietojen perusteella.</a></p>";
  ulos.flush();
}

void HttpKuuntelija::opeta(Poco::Net::HTTPServerResponse &resp)
{
  resp.setContentType("text/html");
  std::ostream& ulos = resp.send(); // ostreamit voisi tulla parametreina
  ulos << "<h1>Toiminnallisuus on vasta toteutettavana...</h1>"
       << "<p>...</p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/\">Takaisin.</a></p>";
}

void HttpKuuntelija::ennuste(Poco::Net::HTTPServerResponse &resp)
{
  resp.setContentType("text/html");
  std::ostream& ulos = resp.send();
  ulos << "<h1>Toiminnallisuus on vasta toteutettavana...</h1>"
       << "<p>...</p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/\">Takaisin.</a></p>";
}


void HttpKuuntelija::valikko(Poco::Net::HTTPServerResponse &resp)
{
  resp.setContentType("text/html");
  std::ostream& ulos = resp.send();
  ulos << "<h1>Heips!</h1>"
       << "<p>Valitse vaihtoehdoista:</p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/lataa/\"><b>Lataa meteorologiset tiedot</b> Ilmatieteenlaitoksen sivulta sovellukselle.</a></p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/opeta-algoritmi/\"><b>Opeta algoritmi</b> ladattujen tietojen perusteella.</a></p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/ennuste/\"><b>Ennusta huomisen sadetilanne</b> Ilmatieteenlaitoksen tietojen perusteella.</a></p>";
  ulos.flush();
}

void HttpKuuntelija::FMIkysely(std::string kutsu, std::string tiedosto) 
{
  Poco::Net::HTTPClientSession sessio("data.fmi.fi");
  Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, kutsu);
  sessio.sendRequest(request);
  Poco::Net::HTTPResponse vastaus;
  std::istream& ladatut = sessio.receiveResponse(vastaus);
  std::ofstream datatiedosto(tiedosto.c_str());
  Poco::StreamCopier::copyStream(ladatut, datatiedosto);
}

void HttpKuuntelija::parsiDataa(std::string lukutiedosto, std::string kirjoitustiedosto, std::string avainsana)
{
  std::ifstream datat(lukutiedosto.c_str());
  XmlLukija handlari(kirjoitustiedosto.c_str());
  handlari.asetaAvainsana(avainsana);
  Poco::XML::SAXParser parseri;
  parseri.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
  parseri.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
  parseri.setContentHandler(&handlari);
  parseri.setProperty(Poco::XML::XMLReader::PROPERTY_LEXICAL_HANDLER, static_cast<Poco::XML::LexicalHandler*>(&handlari));
	
  Poco::XML::InputSource luettava(datat);
  parseri.parse(& luettava);
	
  datat.close();
}
