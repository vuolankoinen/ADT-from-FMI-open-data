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

#include <Poco/SAX/SAXParser.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/SAX/LexicalHandler.h>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/Locator.h>

using namespace Poco::Net;
using namespace Poco::Util;
using namespace Poco::XML;
using namespace std;


//http://poco.roundsquare.net/2010/08/08/pocoxmlsaxparser/
class MyHandler : public Poco::XML::ContentHandler, public Poco::XML::LexicalHandler
{
public:
  MyHandler(string nimi ) : tiedosto(nimi.c_str()) { } //ofstreamin alustaminen std::stringillä tulee vasta C++11:ssa
  
  // ContentHandler
  void setDocumentLocator(const Poco::XML::Locator* loc)
  {
    
  }
  
  void startDocument()
  {
    
  }
  
  void endDocument()
  {
    
  }
  
  void startElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname, const Poco::XML::Attributes& attributes)
  {
    /*m_Msg.Message(Poco::format("    uri:       %s", uri));
      m_Msg.Message(Poco::format("    localName: %s", localName));
      m_Msg.Message(Poco::format("    qname:     %s", qname));
      if(0 != attributes.getLength())
  {
  m_Msg.Message("    Attributes: ");
  for(int i=0; i<attributes.getLength(); ++i)
  {
  m_Msg.Message(Poco::format("     %s=\"%s\"", attributes.getLocalName(i), attributes.getValue(i)));
  }
  }*/
  }
  
  void endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname)
  {
    
  }
  
  void characters(const Poco::XML::XMLChar ch[], int start, int length)
  {
    int myStart = start;
    int myLength = length;
    while((0 != myLength) && (('\n' == ch[myStart]) || ('\t' == ch[myStart]) || (' ' == ch[myStart]))) // Jätetään tyhjät alut huomiotta.
      {
	++myStart;
	--myLength;
      }
    if(0 != myLength)
      {
	tiedosto << std::string(ch + myStart, myLength) << endl;
      }
  }
  
  void ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int length)
  {
    
  }
  
  void processingInstruction(const Poco::XML::XMLString& target, const Poco::XML::XMLString& data)
{

}
 
  void startPrefixMapping(const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri)
  {

  }
 
  void endPrefixMapping(const Poco::XML::XMLString& prefix)
  {

  }
 
  void skippedEntity(const Poco::XML::XMLString& name)
  {

  }
 
  // LexicalHandler
void startDTD(const Poco::XML::XMLString& name, const Poco::XML::XMLString& publicId, const Poco::XML::XMLString& systemId)
  {

  }
 
  void endDTD()
  {

  }
 
  void startEntity(const Poco::XML::XMLString& name)
  {

  }
 
  void endEntity(const Poco::XML::XMLString& name)
  {

  }
 
  void startCDATA()
  {

  }
 
  void endCDATA()
  {

  }
 
  void comment(const Poco::XML::XMLChar ch[], int start, int length)
  {

  }

  ofstream tiedosto;
 
protected:
 
private:


};	

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
	ofstream datatiedosto("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/datat.xml");
	Poco::StreamCopier::copyStream(rs, datatiedosto);
	datatiedosto.close();
      
	ifstream datat("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/datat.xml");
	
	MyHandler handlari("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/puretut_datat.xml");
	Poco::XML::SAXParser parseri;
	parseri.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACES, true);
	parseri.setFeature(Poco::XML::XMLReader::FEATURE_NAMESPACE_PREFIXES, true);
	parseri.setContentHandler(&handlari);
	parseri.setProperty(Poco::XML::XMLReader::PROPERTY_LEXICAL_HANDLER, static_cast<Poco::XML::LexicalHandler*>(&handlari));

	InputSource luettava(datat);
	parseri.parse(& luettava);

	datat.close();

	ulos << "<p>Datat ladattu ja tallennettu.</p>";
	return;
      }

      resp.setContentType("text/html");
      ostream& ulos = resp.send();
      ulos << "<h1>Heips!</h1>"
	   << "<p>Aplikaatio sai juuri pyynnon, muotoa " << req.getMethod() << ".</p>"
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
