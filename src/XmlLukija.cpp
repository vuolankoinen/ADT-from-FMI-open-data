#include "XmlLukija.h"
#include <Poco/SAX/ContentHandler.h>
#include <Poco/SAX/SAXParser.h>
#include <Poco/SAX/InputSource.h>
#include <Poco/SAX/LexicalHandler.h>
#include <Poco/SAX/Attributes.h>
#include <Poco/SAX/Locator.h>


XmlLukija::XmlLukija(std::string nimi ) : tiedosto(nimi.c_str()), kirjoitetaan(0) { } //ofstreamin alustaminen std::stringillä tulee vasta C++11:ssa
XmlLukija::XmlLukija(std::string nimi, std::string avainsana ) : tiedosto(nimi.c_str()), kirjoitetaan(0), avain(avainsana) { }

//omat
void XmlLukija::asetaAvainsana(std::string avainsana) 
{avain = avainsana;}
  
// ContentHandler
void XmlLukija::setDocumentLocator(const Poco::XML::Locator* loc)
{
    
}
  
void XmlLukija::startDocument()
{
    
}
  
void XmlLukija::endDocument()
{
    
}
  
void XmlLukija::startElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname, const Poco::XML::Attributes& attributes)
{
  if (localName == avain) kirjoitetaan = 1;
}
  
void XmlLukija::endElement(const Poco::XML::XMLString& uri, const Poco::XML::XMLString& localName, const Poco::XML::XMLString& qname)
{
  if (localName == avain) kirjoitetaan = 0;
}
  
void XmlLukija::characters(const Poco::XML::XMLChar ch[], int start, int length)
{
  if (kirjoitetaan != 0) {
    int myStart = start;
    int myLength = length;
    while((0 != myLength) && (('\n' == ch[myStart]) || ('\t' == ch[myStart]) || (' ' == ch[myStart]))) // JÃ¤tetÃ¤Ã¤n tyhjÃ¤t alut huomiotta.
      {
	++myStart;
	--myLength;
      }
    if(0 != myLength)
      {
	tiedosto << std::string(ch + myStart, myLength) << std::endl;
      }
  }
}  

void XmlLukija::ignorableWhitespace(const Poco::XML::XMLChar ch[], int start, int length)
{
    
}
  
void XmlLukija::processingInstruction(const Poco::XML::XMLString& target, const Poco::XML::XMLString& data)
{

}
 
void XmlLukija::startPrefixMapping(const Poco::XML::XMLString& prefix, const Poco::XML::XMLString& uri)
{

}
 
void XmlLukija::endPrefixMapping(const Poco::XML::XMLString& prefix)
{

}
 
void XmlLukija::skippedEntity(const Poco::XML::XMLString& name)
{

}
 
// LexicalHandler
void XmlLukija::startDTD(const Poco::XML::XMLString& name, const Poco::XML::XMLString& publicId, const Poco::XML::XMLString& systemId)
{

}
 
void XmlLukija::endDTD()
{

}
 
void XmlLukija::startEntity(const Poco::XML::XMLString& name)
{

}
 
void XmlLukija::endEntity(const Poco::XML::XMLString& name)
{

}
 
void XmlLukija::startCDATA()
{

}
 
void XmlLukija::endCDATA()
{

}
 
void XmlLukija::comment(const Poco::XML::XMLChar ch[], int start, int length)
{

}
