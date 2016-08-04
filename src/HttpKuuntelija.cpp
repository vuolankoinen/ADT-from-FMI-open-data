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
#include <limits> // NaN
#include <sstream>
#include <iomanip>
#include <ctime>
#include <cstdio> // sprintf

#include "XmlLukija.h"   //XmlLukija
#include "ADT.h"
#include "ADT_opettaja.h"


HttpKuuntelija::HttpKuuntelija() 
{
  std::ifstream salasanatiedosto("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/fmi_avain");
  getline(salasanatiedosto, salausavain);
  salasanatiedosto.close();
  std::ifstream aikatiedosto("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/viimeisin_lataus");
  aikatiedosto >> viimeisin_lataus;
  aikatiedosto.close();
}

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
  time_t aika;
  time(&aika);
  struct tm * hetki_ptr = gmtime(&aika);
  {  // Laitetaan latausaika muistiin.
    std::ofstream aikatiedosto("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/viimeisin_lataus");
    aikatiedosto << aika;
    aikatiedosto.close();
  }

  int ladattavia_vuosia = 2; // Monelta vuodelta ladataan tiedot.
  std::vector< std::string> alkuajat(ladattavia_vuosia);
  std::vector< std::string> loppuajat(ladattavia_vuosia);
  std::vector< std::string> xmlt(ladattavia_vuosia);
  std::vector< std::string> parsitut_datat(ladattavia_vuosia);
  for (int tt = 0; tt < ladattavia_vuosia; tt++)
    { // asetetaan ladattavien jaksojen alku- ja loppuhetket.
      char buffer1 [50], buffer2 [50], buffer3 [80], buffer4 [80];
      sprintf (buffer1, "%u-%.2u-%.2uT%.2u:%.2u:00Z", 
	       (hetki_ptr->tm_year - tt + 1900), 
	       ( (hetki_ptr->tm_mday > 6) ? (hetki_ptr->tm_mon) : (hetki_ptr->tm_mon -1) ), 
	       ( (hetki_ptr->tm_mday > 6) ? (hetki_ptr->tm_mday - 6) : ((hetki_ptr->tm_mday > 4) ? 1 : (31 - hetki_ptr->tm_mday))), 
	       hetki_ptr->tm_hour, 
	       hetki_ptr->tm_min);
      alkuajat[tt] = buffer1;
      sprintf (buffer2, "%u-%.2u-%.2uT%.2u:%.2u:00Z", 
	       (hetki_ptr->tm_year - tt + 1900), 
	       hetki_ptr->tm_mon, 
	       hetki_ptr->tm_mday, 
	       hetki_ptr->tm_hour, 
	       hetki_ptr->tm_min);
      loppuajat[tt] = buffer2;
      sprintf (buffer3, "/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/Helsinki%u.xml", (hetki_ptr->tm_year - tt + 1900));
      xmlt[tt] = buffer3;
      sprintf (buffer4, "/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/Helsinki%u.data", (hetki_ptr->tm_year - tt + 1900));
      parsitut_datat[tt] = buffer4;
    }
  resp.setContentType("text/html");
  std::ostream& ulos = resp.send();

  std::string kutsu;
  std::ifstream luku;
  std::ofstream pos("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/satoi.data"); pos << ""; pos.close();
  std::ofstream neg("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/ei_satanut.data"); neg << ""; neg.close();
  pos.open("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/satoi.data", std::ios::app);
  neg.open("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/ei_satanut.data", std::ios::app);
  
  for (int tt = 0; tt < ladattavia_vuosia; tt++) // Ladataan, parsitaan ja tallennetaan tiedot.
    {
      kutsu = "/fmi-apikey/" + salausavain + "/wfs?request=getFeature&storedquery_id=fmi::observations::weather::multipointcoverage&place=Helsinki&starttime=" + alkuajat[tt]  + "&endtime=" + loppuajat[tt]  + "&timestep=120&parameters=r_1h,t2m,ws_10min,wg_10min,wd_10min,rh,td,p_sea,vis,n_man";

      FMIkysely(kutsu, xmlt[tt]);
      parsiDataa(xmlt[tt], parsitut_datat[tt], "DataBlock");

      luku.open( (parsitut_datat[tt]).c_str() );
      luokitteleDataa(luku, pos, neg, hetki_ptr->tm_hour);
      luku.close();
    }
  pos.close();
  neg.close();

  ulos << "<h1>Heips!</h1>"
       << "<p>Valitse vaihtoehdoista:</p>"
       << "<p>Datat ladattu ja tallennettu.</p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/opeta-algoritmi/\"><b>Opeta ADT-algoritmi</b> ladattujen tietojen perusteella.</a></p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/ennuste/\"><b>Ennusta huomisen sadetilanne Helsingin tienoilla</b> Ilmatieteen laitoksen tietojen perusteella.</a></p>";
  ulos.flush();

}

void HttpKuuntelija::opeta(Poco::Net::HTTPServerResponse &resp)
{
  std::ifstream pos("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/satoi.data");
  std::ifstream neg("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/ei_satanut.data");
  ADT_opettaja ope(pos, neg);
  {
    ADT puu2(ope.ekaEnnustearvo());
    puu = puu2;
  }
  puu.uusiaHaaroja(4, ope);
  resp.setContentType("text/html");
  std::ostream& ulos = resp.send(); // ostreamit voisi tulla parametreina
  ulos << "<h1>Puu on kasvatettu nelihaaraiseksi.</h1>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/ennuste/\"><b>Ennusta huomisen sadetilanne Helsingin tienoilla</b> Ilmatieteen laitoksen tietojen perusteella.</a></p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/\">Takaisin.</a></p>";
}

void HttpKuuntelija::ennuste(Poco::Net::HTTPServerResponse &resp)
{
  resp.setContentType("text/html");
  std::ostream& ulos = resp.send();
  std::string rivi, vikarivi;
  std::vector<double> kaisaniemi;
  time_t aika;
  time(&aika);
  struct tm * hetki_ptr = gmtime(&aika);
  int tunti =  hetki_ptr->tm_hour;
  kaisaniemi.push_back(tunti);
  for (int tt = 0; tt < 2; ++tt)
  {
    char loppuaika [50]; 
    char alkuaika [50];
    sprintf (loppuaika, "%u-%.2u-%.2uT%.2u:00:00Z", 
	     (hetki_ptr->tm_year + 1900), 
	     hetki_ptr->tm_mon,
	     tunti > 0 ? hetki_ptr->tm_mday : hetki_ptr->tm_mday - tt,
	     tunti > 0 ? tunti - tt : 23);
    sprintf (alkuaika, "%u-%.2u-%.2uT%.2u:00:00Z", 
	     (hetki_ptr->tm_year + 1900), 
	     hetki_ptr->tm_mon,
	     tunti > 2 ? hetki_ptr->tm_mday : hetki_ptr->tm_mday - tt,
	     tunti > 2 ? tunti -1- tt : tunti + 23  -tt);
    std::string kutsu  = "/fmi-apikey/" + salausavain + "/wfs?request=getFeature&storedquery_id=fmi::observations::weather::multipointcoverage&place=Helsinki&parameters=ri_10min,t2m,ws_10min,wg_10min,wd_10min,rh,td,p_sea,vis,n_man&endtime=" + loppuaika + "&starttime=" + alkuaika;
    FMIkysely(kutsu, "/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/nyky.xml");
    parsiDataa("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/nyky.xml", 
	     "/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/nyky.data", 
	     "DataBlock");
    std::ifstream tiedot("/var/lib/openshift/574a156e0c1e6668bd000175/app-root/runtime/data/nyky.data");
    while (getline(tiedot, rivi)) {if (rivi != "") vikarivi = rivi;}
    std::stringstream ss(vikarivi);
    std::string luku;
    while (ss>>luku) kaisaniemi.push_back(s2d(luku));
  }
  ulos << "<p>Viimeisin meteorologinen tieto, Kaisaniemi:<br />"
       << "sadetta:             "<< kaisaniemi[1] << "<br />"
       << "asteita:        "<< kaisaniemi[2] << "<br />"
       << "tuulennopeus:        "<< kaisaniemi[3] << "<br />"
       << "puuskanopeus:        "<< kaisaniemi[4] << "<br />"
       << "tuulen suunta:        "<< kaisaniemi[5] << "<br />"
       << "suhteellinen kosteus: "<< kaisaniemi[6] << "<br />"
       << "kastepiste: "<< kaisaniemi[7] << "<br />"
       << "ilmanpaine:          "<< kaisaniemi[8] << "<br />"
       << "visuaalinen kantama: "<< kaisaniemi[9] << "<br />"
       << "pilvenpeitto: "<< kaisaniemi[10] << "<br />"
       <<"</p>";
  double enne = puu.ennuste(kaisaniemi);
  if (enne < 0)  ulos << "<p>ADT-puun mukaan luvassa ei ole sadetta.</p>";
  if (enne > 0)  ulos << "<p>ADT-puun mukaan luvassa on sadetta.</p>";
  ulos << "<p>Ennusteen varmuusaste:  "<< enne<<"   (Nollan tienoilla ennusteessa ei ole varmuutta. Jos varmuus on tasan nolla, kannattaa uusia algoritmin opetus.)</p>";
  ulos << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/\">Takaisin.</a></p>";
}


void HttpKuuntelija::valikko(Poco::Net::HTTPServerResponse &resp)
{
  time_t aika;
  time(&aika);
  resp.setContentType("text/html");
  std::ostream& ulos = resp.send();
  ulos << "<h1>Heips!</h1>"
       << "<p>Valitse vaihtoehdoista:</p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/lataa/\"><b>Lataa meteorologiset tiedot</b> Ilmatieteen laitoksen sivulta sovellukselle.</a>";
  ulos << " Datat on ladattu viimeksi ";
  if (aika - viimeisin_lataus > 172800) {
    ulos << "yli " << (aika - viimeisin_lataus)/86400 << " vuorokautta sitten.";
  } else if (aika - viimeisin_lataus > 7200) {
    ulos << (aika - viimeisin_lataus)/3600 << " tuntia sitten.";
  } else if (aika - viimeisin_lataus > 300) {
    ulos << (aika - viimeisin_lataus)/60 << " minuuttia sitten.";
  } else {
    ulos << "hetki sitten.";
  } 

  ulos << "</p><p><a href=\"http://ml-vuolankoinen.rhcloud.com/opeta-algoritmi/\"><b>Opeta algoritmi</b> ladattujen Ilmatieteen laitoksen tietojen perusteella.</a></p>"
       << "<p><a href=\"http://ml-vuolankoinen.rhcloud.com/ennuste/\"><b>Ennusta sadetilanne Helsingin tienoilla seuraavien kahden tunnin aikana</b>.</a></p>";
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

//Luokittelee sen mukaan, onko rivin eka muuttuja nollasta poikkeava vai ei. Olettaa aloituskellonajaksi 20:00 ja askelpituudeksi 2 tuntia.
void HttpKuuntelija::luokitteleDataa(std::istream& lukutiedosto, std::ostream& kirjoitustiedosto1, std::ostream& kirjoitustiedosto2, int kellonaika = 20) 
{
  std::string rivi, rivi2, ehdokas;
  double luku;
  int osoitin = 1, kirjoituskohde = 0, laskuri, dim = 9; // dim on parametrien lukum. -1
  std::vector<double> eka(dim +1), toka(dim +1), kolmas(dim +1);
  for (int tt = 0; tt < 2; tt++) {// Pari ekaa paivaa pohjille alkuun.
    getline(lukutiedosto, rivi);
    std::stringstream ss(rivi);
    laskuri = 0;
    while (ss>>ehdokas) 
      {
	luku = s2d(ehdokas);
	if (tt == 0) {eka[laskuri] = luku;} else {toka[laskuri] = luku;}
	laskuri++;
      }
    if (laskuri < dim) { // Katkonainen rivi.
      laskuri = 0;
      getline(lukutiedosto, rivi2);
      rivi = rivi + rivi2;
      ss.str(rivi);
      while (ss>>ehdokas)  // Korjataan vajaa rivi.
	{
	  luku = s2d(ehdokas);
	  if (tt == 0) { eka[laskuri] = luku;} else{ toka[laskuri] = luku;}
	  laskuri++;
	}
    }
  }
  while (getline(lukutiedosto, rivi))
    {
      std::stringstream ss(rivi);
      osoitin = osoitin == 2 ? 0 : osoitin + 1;
      // Seuraava on turhan kankeaa (toisteista), voisi korvata paremmalla tavalla roikottaa kahta aiempaa rivii mukana.
      if (osoitin == 0) {
	laskuri = 0;
	while (ss>>ehdokas) 
	  {
	    luku = s2d(ehdokas);
	    eka[laskuri] = luku;
	    laskuri++;
	  }
	if (laskuri < dim) { // Katkonainen rivi.
	  laskuri = 0;
	  getline(lukutiedosto, rivi2);
	  rivi = rivi + rivi2;
	  ss.str(rivi);
	  while (ss>>ehdokas)  // Korjataan katkonainen rivi.
	    {
	      luku = s2d(ehdokas);
	      eka[laskuri] = luku;
	      laskuri++;
	    }
	}
	tulostaRivi(kellonaika,  eka,  kolmas,  toka, eka[0] > 0.0 ? kirjoitustiedosto1 : kirjoitustiedosto2); // Kirjoitetaan rivi oikeaan tiedostoon.	
      } else if (osoitin == 1) {
	laskuri = 0;
	while (ss>>ehdokas) 
	  {
	    luku = s2d(ehdokas);
	    toka[laskuri] = luku;
	    laskuri++;
	  }
	if (laskuri < dim) { // Vajaa rivi.
	  laskuri = 0;
	  getline(lukutiedosto, rivi2);
	  rivi = rivi + rivi2;
	  ss.str(rivi);
	  while (ss>>ehdokas)  // Korjataan katkonainen rivi.
	    {
	      luku = s2d(ehdokas);
	      toka[laskuri] = luku;
	      laskuri++;
	    }
	}
	tulostaRivi(kellonaika,  toka,  eka, kolmas, toka[0] > 0.0 ? kirjoitustiedosto1 : kirjoitustiedosto2); // Kirjoitetaan rivi oikeaan tiedostoon.	
      } else {
	laskuri = 0;
	while (ss>>ehdokas) 
	  {
	    luku = s2d(ehdokas);
	    kolmas[laskuri] = luku;
	    laskuri++;
	  }
	if (laskuri < dim) { // Katkonainen rivi.
	  laskuri = 0;
	  getline(lukutiedosto, rivi2);
	  rivi = rivi + rivi2;
	  ss.str(rivi);
	  while (ss>>ehdokas)  // Korjataan katkonainen rivi.
	    {
	      luku = s2d(ehdokas);
	      kolmas[laskuri] = luku;
	      laskuri++;
	    }
	}
	tulostaRivi(kellonaika,  kolmas, toka,  eka, kolmas[0] > 0.0 ? kirjoitustiedosto1 : kirjoitustiedosto2); // Kirjoitetaan rivi oikeaan tiedostoon.	
      }
      kellonaika = (kellonaika >= 22) ? kellonaika - 22 : kellonaika + 2; 
    }
}


double HttpKuuntelija::s2d(std::string s) {
  double d;
  std::stringstream ss(s);
  ss >> d;
  if (d==0) { // Tarkistetaan NaN:in varalta.
    if (s.find_first_of("0123456789") > s.size())  return std::numeric_limits<double>::quiet_NaN(); // Ei ole numeroa, joten NaN.
  }
  return d;
}

void HttpKuuntelija::tulostaRivi(int kellonaika, std::vector<double> datat1, std::vector<double> datat2, std::vector<double> datat3, std::ostream& tiedosto) 
{
  tiedosto << std::setw (2) << kellonaika << " ";
  /*  for (int tt = 0, viimeinen = datat1.size(); tt < viimeinen; tt++) tiedosto << std::setw(7) << datat1[tt] << " ";// Pois, koska samanhetkiset tiedot ovat ennustamiseen melko hoopoja. .*/
  for (int tt = 0, viimeinen = datat2.size(); tt < viimeinen; tt++) tiedosto << std::setw(7) << datat2[tt] << " ";
  for (int tt = 0, viimeinen = datat3.size(); tt < viimeinen; tt++) tiedosto << std::setw(7) << datat3[tt] << " ";
  tiedosto << std::endl;
}
