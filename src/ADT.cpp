/* Luokka ADT:n haarakohdalle: ennustearvo ja sen alla olevat haarat. Kokonainen puu koostuu toisiaan seuraavista ADT-olioista. */

#include "ADT.h"
#include <vector>
#include <set>
#include <algorithm> // sort, find
#include <limits> // NaN
#include <cmath> // log (= ln)
#include "ADT_opettaja.h"

#include <iostream>//deb
#include <sstream> // rakennetta varten


// Konstruktorit.
ADT::ADT() {   }

ADT::ADT(double alkuarvo) : ennustearvo(alkuarvo) 
{ }

// Luo puuhun uusia haaroja int montako kappaletta. Uudet haarat ovat informatiivisimmat mahdolliset.
void ADT::uusiaHaaroja(int montako, ADT_opettaja & ope) 
{
  std::vector<int> kaikki = ope.kaikkiTapaukset();
  for (int tt = 0; tt < montako; tt++)
    {
      uusiHaaraZarvolle(parasZarvo(kaikki, ope), kaikki, ope);
  }
}

// Palauttaa annetulle datalle puun tuottaman ennusteen: positiviinen palaute ennustaa sadetta, suurempi arvo varmempaa kuin pieni.
double ADT::ennuste(std::vector<double> data) 
{
  if (haarat.size() == 0) return ennustearvo; // Lehti.
  double tulos = ennustearvo;
  std::cout << "   Tulos nyt: " << tulos <<std::endl;//deb
  for (int tt = 0, viimeinen = ehtomuuttujat.size(); tt < viimeinen; tt++) 
    {
      if (data[ ehtomuuttujat[tt] ] < ehtojenJakokohdat[tt])
	{
	  tulos += (haarat[2*tt]).ennuste(data); // Vasen haara.
	} else {
	tulos += (haarat[2*tt+1]).ennuste(data); // Oikea haara.
      }
    }
  return tulos;
}

// Palauttaa puun rakenteen merkkijonona. Hankalasti luettava, mutta antaa kuitenkin mahdollisuuden luodun puun tarkasteluun.
std::string ADT::rakenne(std::string sisennys = "")
{
  std::string tulos = sisennys + static_cast<std::ostringstream*>( &(std::ostringstream() << ennustearvo) )->str() + "<br />";
  for (int tt = 0, loppu = ehtomuuttujat.size(); tt < loppu; ++tt)
    {
      tulos = tulos + sisennys +  "Muuttuja -> " + static_cast<std::ostringstream*>( &(std::ostringstream() << ehtomuuttujat[tt]) )->str() + " | " + static_cast<std::ostringstream*>( &(std::ostringstream() << ehtojenJakokohdat[tt]) )->str() + " <- jakopiste<br />";
      tulos = tulos + sisennys + "Vasen haara:<br />" + (haarat[2*tt]).rakenne(sisennys + ".....") + "<br />";
      tulos = tulos + sisennys + "Oikea haara:<br />" + (haarat[2*tt+1]).rakenne(sisennys + ".....") + "<br />";
    }
  return tulos;
}

// *******
// Privaatit:

// Etsii ja palauttaa parhaan jakoehdon arvon. 
double ADT::parasZarvo(std::vector<int> osajoukko, ADT_opettaja ope) 
{ 
  double tulos = 100000.0, ehdokas = 100001.0;
  for (int tt = 0, loppu = ope.dimensio(); tt < loppu; ++tt)
    {
    if (std::find(ehtomuuttujat.begin(),ehtomuuttujat.end(),tt) != ehtomuuttujat.end()) continue; // Jatka, jos muuttujalle on jo jakoehto.
    std::vector<double> ehdokkaat_jakopisteiksi = ope.mahdollisetJakopisteet(osajoukko, tt);
    for (std::vector<double>::iterator jako_ptr = ehdokkaat_jakopisteiksi.begin(), vika = ehdokkaat_jakopisteiksi.end(); jako_ptr != vika; ++jako_ptr)
      {	
	ehdokas = ope.Zarvo(osajoukko, tt, *jako_ptr);
	tulos = tulos < ehdokas ? tulos : ehdokas;
      }
  }
 if (ehtomuuttujat.size() > 0)
   for (int tt = 0, viimeinen = ehtomuuttujat.size(); tt < viimeinen; ++tt)
     { // Rekursiolla kaikki alipuut.
       ehdokas = (haarat[2*tt]).parasZarvo(ope.vasenHaara(osajoukko, ehtomuuttujat[tt], ehtojenJakokohdat[tt]), ope);
       tulos = tulos < ehdokas ? tulos : ehdokas;
       ehdokas = (haarat[2*tt+1]).parasZarvo(ope.oikeaHaara(osajoukko, ehtomuuttujat[tt], ehtojenJakokohdat[tt]), ope);
       tulos = tulos < ehdokas ? tulos : ehdokas;
     } 
 return tulos;
}

// Etsii annetun arvon tuottavan jakoehdon ja luo uuden haaran puuhun kyseiselle jakoehdolle.
int ADT::uusiHaaraZarvolle(double parasZarvo, std::vector<int> osajoukko, ADT_opettaja & ope) 
{
  for (int tt = 0, loppu = ope.dimensio(); tt < loppu; ++tt)
    { // Kolutaan kaikki vapaat muuttujat uuden parhaan jakoehdon toivossa.
      std::vector<double> ehdokkaat_jakopisteiksi = ope.mahdollisetJakopisteet(osajoukko, tt);
      for (std::vector<double>::iterator jako_ptr = ehdokkaat_jakopisteiksi.begin(), vika = ehdokkaat_jakopisteiksi.end(); jako_ptr != vika; ++jako_ptr)
	if (ope.Zarvo(osajoukko, tt, *jako_ptr) == parasZarvo)
	  { // Hep! Luodaan siis uusi jakoehto.
	    if (std::find(ehtomuuttujat.begin(),ehtomuuttujat.end(),tt) != ehtomuuttujat.end()) continue; // Jatka, jos muuttujalle on jo jakoehto.
	    ehtomuuttujat.push_back(tt);
	    std::cout<< "*";
	    ehtojenJakokohdat.push_back(*jako_ptr);
	    std::cout<< "*";
	    std::vector<double> uudet = ope.ennustearvot(osajoukko, tt, *jako_ptr);
	    std::cout<< "*";
      ADT vasen(uudet[0]);
      ADT oikea(uudet[1]);
      haarat.push_back(vasen);
      haarat.push_back(oikea);
      ope.uudetPainot(osajoukko, tt, *jako_ptr, uudet[0], uudet[1]);
      return 1;
    }
  }
 if (ehtomuuttujat.size() > 0)
   for (int tt = 0, viimeinen = ehtomuuttujat.size(); tt < viimeinen; ++tt)
     { // Rekursiolla kaikki alipuut.
       if ( (haarat[2*tt]).uusiHaaraZarvolle(parasZarvo, ope.vasenHaara(osajoukko, ehtomuuttujat[tt], ehtojenJakokohdat[tt]), ope) == 1 ) return 1;
       if ( (haarat[2*tt+1]).uusiHaaraZarvolle(parasZarvo, ope.oikeaHaara(osajoukko, ehtomuuttujat[tt], ehtojenJakokohdat[tt]), ope)  == 1) return 1;
     }
}
