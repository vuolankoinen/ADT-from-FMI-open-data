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
#include <ctime>
#include <Poco/SAX/ContentHandler.h> //xml 

// Paree ottaa XML pois ja kokeilla ihan vaan Utilsilla, kts. 
// http://pocoproject.org/docs/Poco.Util.XMLConfiguration.html

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::XML;
using namespace std;



 class MyHandler: public ContentHandler//xml 
 {//xml 
 public://xml 
   MyHandler();//xml 
   void setDocumentLocator(const Locator* loc);//xml 
   void startDocument();//xml 
   void endDocument();//xml 
   void startElement(//xml 
 		    const XMLString& namespaceURI,//xml 
 		    const XMLString& localName,//xml 
 		    const XMLString& qname,//xml 
 		    const Attributes& attributes);//xml 
   void endElement(//xml 
 		  const XMLString& uri,//xml 
 		  const XMLString& localName,//xml 
 		  const XMLString& qname);//xml 
   void characters(const XMLChar ch[], int start, int length);//xml 
   void ignorableWhitespace(const XMLChar ch[], int start, int len);//xml 
   void processingInstruction(//xml 
 			     const XMLString& target,//xml 
 			     const XMLString& data);//xml 
   void startPrefixMapping(//xml 
 			  const XMLString& prefix,//xml 
 			  const XMLString& uri);//xml 
   void endPrefixMapping(const XMLString& prefix);//xml 
   void skippedEntity(const XMLString& name);//xml 
 };//xml 
	
	

class MyRequestHandler : public HTTPRequestHandler
{
public:
  virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
  {
    resp.setStatus(HTTPResponse::HTTP_OK);
    
    
    if (req.getURI() == "/lataa/") {
      resp.setContentType("text/html");
      ostream& ulos = resp.send();
      
      string salausavain;
      ifstream tiedosto;
      tiedosto.open("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/fmi_avain");
      getline(tiedosto, salausavain);
      tiedosto.close();
      
      string kutsu = "/fmi-apikey/" + salausavain + "/wfs?request=getFeature&storedquery_id=fmi::observations::weather::timevaluepair&parameters=windspeedms,temperature&fmisid=101003&starttime=2010-04-12T03:59:59Z&endtime=2010-04-14T05:59:59Z";
      HTTPClientSession sessio("data.fmi.fi");
      HTTPRequest request(HTTPRequest::HTTP_GET, kutsu);
      sessio.sendRequest(request);
      HTTPResponse vastaus;
      std::istream& rs = sessio.receiveResponse(vastaus);
      ofstream datatiedosto;
      datatiedosto.open("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/datat.xml");
      Poco::StreamCopier::copyStream(rs, datatiedosto);
      datatiedosto.close();

      ulos << "<p>Datat ladattu ja tallennettu.</p>";

      return;
    }

    resp.setContentType("text/html");
    ostream& ulos = resp.send();
    ulos << "<h1>Heips!</h1>"
	 << "<p>Aplikaatio sai juuri pyynnon, muotoa " << req.getMethod() << ",</p>"
	 << "<p>Hienoa!</p>";
    
  }

private:
  static int count;
};

class MyRequestHandlerFactory : public HTTPRequestHandlerFactory
{
public:
  virtual HTTPRequestHandler* createRequestHandler(const HTTPServerRequest &)
  {
    return new MyRequestHandler;
  }
};

class MyMLApp : public ServerApplication
{
protected:
  int main(const vector<string> &)
  {
    HTTPServer s(new MyRequestHandlerFactory, ServerSocket(SocketAddress("127.2.41.1:8080")), new HTTPServerParams);

    s.start();

    waitForTerminationRequest();

    s.stop();

    return Application::EXIT_OK;
  }
};

int main(int argc, char** argv)
{

  MyMLApp app;
  return app.run(argc, argv);
}
