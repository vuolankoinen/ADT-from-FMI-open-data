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

using namespace Poco::Net;
using namespace Poco::Util;
using namespace std;



class MyRequestHandler : public HTTPRequestHandler
{
public:
  virtual void handleRequest(HTTPServerRequest &req, HTTPServerResponse &resp)
  {
    resp.setStatus(HTTPResponse::HTTP_OK);


    if (req.getURI() == "/lataa/") {
    resp.setContentType("text/xml");
    ostream& ulos = resp.send();

      string salausavain;
      ifstream tiedosto;
      tiedosto.open("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/fmi_avain");
      getline(tiedosto, salausavain);
      tiedosto.close();

      string kutsu = "/fmi-apikey/" + salausavain + "/wfs?request=getFeature&storedquery_id=fmi::observations::weather::timevaluepair&parameters=windspeedms,temperature&fmisid=101003&starttime=2010-04-12T03:59:59Z&endtime=2010-04-14T05:59:59Z";
      HTTPClientSession s("data.fmi.fi");
      HTTPRequest request(HTTPRequest::HTTP_GET, kutsu);
      s.sendRequest(request);
      HTTPResponse response;
      std::istream& rs = s.receiveResponse(response);
      Poco::StreamCopier::copyStream(rs, ulos);

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

int MyRequestHandler::count = 0;

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
