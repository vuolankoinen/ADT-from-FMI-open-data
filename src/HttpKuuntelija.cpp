/* Hoitaa palvelimen saamat HTTP-kutsut.*/

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

#include "XmlLukija.h"
#include "HttpKuuntelija.h"

void HttpKuuntelija::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp)
  {
    resp.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
    
    if (req.getURI() == "/lataa/") {
      resp.setContentType("text/html");
      std::ostream& ulos = resp.send();
      std::string salausavain;
      std::ifstream tiedosto;
      tiedosto.open("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/fmi_avain");
      getline(tiedosto, salausavain);
      tiedosto.close();
      
      std::string kutsu = "/fmi-apikey/" + salausavain + "/wfs?request=getFeature&storedquery_id=fmi::observations::weather::timevaluepair&parameters=windspeedms,temperature&fmisid=101003&starttime=2010-04-12T03:59:59Z&endtime=2010-04-14T05:59:59Z";
      /*
Muita käypiä kyselyitä on.
weather voidaan korvata:
   lightning
   mareograph       (merenpinnan korkeus)
   radiation        (Auringon säteily)
   wave             (aalto- ja lämpötiedot poijuilta)
   weather::daily   (päivittäiset sade, lämmön keskiarvo ja ääriarvot)

   fmisid - FMI observation station identifier.

parameters:
   Temperature
   Pressure
   Humidity
   WindDirection
   WindSpeedMS
   WindUMS
   WindVMS
   MaximumWind

   Käyttistä lienee myös vuorokauden- ja vuodenaika.
*/
      Poco::Net::HTTPClientSession sessio("data.fmi.fi");
      Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_GET, kutsu);
      sessio.sendRequest(request);
      Poco::Net::HTTPResponse vastaus;
      std::istream& rs = sessio.receiveResponse(vastaus);
      std::ofstream datatiedosto("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/datat.xml");
      Poco::StreamCopier::copyStream(rs, datatiedosto);
      datatiedosto.close();
      
      std::ifstream datat("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/datat.xml");
      
      XmlLukija handlari("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/puretut_datat.xml");
      Poco::XML::SAXParser parseri;
      parseri.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
      parseri.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
      parseri.setContentHandler(&handlari);
      parseri.setProperty(Poco::XML::XMLReader::PROPERTY_LEXICAL_HANDLER, static_cast<Poco::XML::LexicalHandler*>(&handlari));

      Poco::XML::InputSource luettava(datat);
      parseri.parse(& luettava);

      datat.close();

      ulos << "<p>Datat ladattu ja tallennettu.</p>";
      return;
    }

    resp.setContentType("text/html");
    std::ostream& ulos = resp.send();
    ulos << "<h1>Heips!</h1>"
         << "<p>Applikaation käyttöön on seuraavat vaihtoehdot:.</p>"
         << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/lataa/\">Lataa säätiedot Ilmatieteenlaitoksen sivulta palvelimelle.</a></p>";
    
  }
