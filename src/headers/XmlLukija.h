#ifndef XmlLukija_H
#define XmlLukija_H

#include <fstream>
#include <Poco/SAX/ContentHandler.h>
#include <Poco/SAX/SAXParser.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/SAX/LexicalHandler.h>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/Locator.h>

//http://poco.roundsquare.net/2010/08/08/pocoxmlsaxparser/
struct XmlLukija : public Poco::XML::ContentHandler, public Poco::XML::LexicalHandler
{
 private:
  std::ofstream tiedosto;
  int kirjoitetaan;
  std::string avain;

 public:
  XmlLukija(std::string nimi);
  XmlLukija(std::string nimi, std::string avainsana);
  //omat
  void asetaAvainsana(std::string avainsana);
  // ContentHandler
  void setDocumentLocator(const Poco::XML::Locator* loc);
  
  void startDocument();
  
  void endDocument();
  
  void startElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname, const Poco::XML::Attributes& attributes);
  
  void endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname);
  
  void characters(const Poco::XML::XMLChar ch[], int start, int length);
  
  void ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int length);
  
  void processingInstruction(const Poco::XML::XMLString& target, const Poco::XML::XMLString& data);
 
  void startPrefixMapping(const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri);
 
  void endPrefixMapping(const Poco::XML::XMLString& prefix);
 
  void skippedEntity(const Poco::XML::XMLString& name);
 
  // LexicalHandler
  void startDTD(const Poco::XML::XMLString& name, const Poco::XML::XMLString& publicId, const Poco::XML::XMLString& systemId);
 
  void endDTD();
 
  void startEntity(const Poco::XML::XMLString& name);
 
  void endEntity(const Poco::XML::XMLString& name);
 
  void startCDATA();
 
  void endCDATA();
 
  void comment(const Poco::XML::XMLChar ch[], int start, int length);
};


#endif
