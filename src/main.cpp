/* Luo ja käynnistää palvelinsovelluksen. 
   Palvelinsovelluksen luonteen sanelevat kokonaan rajapinnasta saatavat RequestHandlerFactory ja sen luomat RequestHandlerit.*/

#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/SocketAddress.h>
#include <Poco/Util/ServerApplication.h>
#include <fstream>
#include <string>
#include <vector> 

#include "rajapinnat.h"  //MyRequestHandlerFactory 


class Palvelinsovellus : public Poco::Util::ServerApplication
{
protected:
  int main(const std::vector<std::string> &)
  {
    Poco::Net::HTTPServer palvelin(new MyRequestHandlerFactory, Poco::Net::ServerSocket(Poco::Net::SocketAddress("127.2.41.1:8080")), new Poco::Net::HTTPServerParams);
    palvelin.start();
    waitForTerminationRequest();
    palvelin.stop();
    return Poco::Util::Application::EXIT_OK;
  }
};
  
int main(int argc, char** argv)
{
    
  Palvelinsovellus sovellus;
  return sovellus.run(argc, argv);
}
