/* Tarjoaa palvelimelle HTTP-kutsuja hoitavia HttpKuuntelija-luokan olioita luovan luokan edustajan.*/

#include "HttpKuuntelijaleipuri.h"
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

#include "HttpKuuntelija.h"
 
Poco::Net::HTTPRequestHandler* HttpKuuntelijaleipuri::createRequestHandler(const Poco::Net::HTTPServerRequest &)
{
  return new HttpKuuntelija;
}
