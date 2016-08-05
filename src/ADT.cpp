#include "ADT.h"
#include <vector>
#include <set>
#include <algorithm> // sort, find
#include <limits> // NaN
#include <cmath> // log (= ln)
#include "ADT_opettaja.h"

#include <iostream>//deb
#include <sstream> // rakennetta varten

//Tyhjille datoille seulat!
//Painokertoimet boostauksen mukaisesti!
//opetusdata pois luokasta - jokaisella alipuulla on omansa, kankeaa!

ADT::ADT() {   }

ADT::ADT(double alkuarvo) : ennustearvo(alkuarvo) 
{ 
std::cout << "Uuden haaran ennustearvo: " << ennustearvo<<std::endl;//deb
}

void ADT::uusiaHaaroja(int montako, ADT_opettaja ope) 
{
std::vector<int> kaikki = ope.kaikkiTapaukset();
  for (int tt = 0; tt < montako; tt++)
    {
std::cout << std::endl << std::endl<<"###############################"<<std::endl<< "uusia haaroja, mones "<< tt <<std::endl;//deb
uusiHaaraZarvolle(parasZarvo(kaikki, ope), kaikki, ope);
    }
}

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
  std::cout << " Tulos lisayksen (vasen haara) jälkeen: " << tulos <<std::endl;//deb
	} else {
	  tulos += (haarat[2*tt+1]).ennuste(data); // Oikea haara.
std::cout << " Tulos lisayksen (oikea haara) jälkeen: " << tulos <<std::endl;//deb
      }
    }
  return tulos;
}

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
// Privat:

double ADT::parasZarvo(std::vector<int> osajoukko, ADT_opettaja ope) 
{ 
  double tulos = 100000.0, ehdokas = 0.0;

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
//std::cout << "  omista haaroista paras Z-arvo: " << tulos << std::endl;//deb
    
  if (ehtomuuttujat.size() > 0)
    for (int tt = 0, viimeinen = ehtomuuttujat.size(); tt < viimeinen; ++tt)
      { // Rekursiolla kaikki alipuut.
	std::cout << "     Liikutaan alipuuhun: " << 2*tt << std::endl;//deb
ehdokas = (haarat[2*tt]).parasZarvo(ope.vasenHaara(osajoukko, ehtomuuttujat[tt], ehtojenJakokohdat[tt]), ope);
	tulos = tulos < ehdokas ? tulos : ehdokas;
	std::cout << "     Liikutaan alipuuhun: " << 2*tt+1 << std::endl;//deb
ehdokas = (haarat[2*tt+1]).parasZarvo(ope.oikeaHaara(osajoukko, ehtomuuttujat[tt], ehtojenJakokohdat[tt]), ope);
	tulos = tulos < ehdokas ? tulos : ehdokas;
      } 
std::cout << "  Alipuista ja omista paras Z-arvo: " << tulos << std::endl;//deb
  return tulos;
}

int ADT::uusiHaaraZarvolle(double parasZarvo, std::vector<int> osajoukko, ADT_opettaja ope) 
{
  for (int tt = 0, loppu = ope.dimensio(); tt < loppu; ++tt)
    { // Kolutaan kaikki vapaat muuttujat uuden parhaan jakoehdon toivossa.

      std::vector<double> ehdokkaat_jakopisteiksi = ope.mahdollisetJakopisteet(osajoukko, tt);
      for (std::vector<double>::iterator jako_ptr = ehdokkaat_jakopisteiksi.begin(), vika = ehdokkaat_jakopisteiksi.end(); jako_ptr != vika; ++jako_ptr)
	if (ope.Zarvo(osajoukko, tt, *jako_ptr) == parasZarvo)
	  { // Hep! Luodaan siis uusi jakoehto.
	    if (std::find(ehtomuuttujat.begin(),ehtomuuttujat.end(),tt) != ehtomuuttujat.end()) continue; // Jatka, jos muuttujalle on jo jakoehto.
	    std::cout <<std::endl<< "************************" << "Paras Z-arvo ja sen sijainti tiedossa! Luodaan uusi haara!! " << std::endl << "Ensin muuttujatiedot."<< std::endl;//deb
std::cout << " Muuttuja: "<<tt << " jakopiste" << *jako_ptr<< std::endl;//deb

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
	    std::cout << "Luonti onnistui. Muuttuja " << tt << "  jakopiste " << *jako_ptr << std::endl << "********************************"<<std::endl<<std::endl;//deb
ope.uudetPainot(osajoukko, tt, *jako_ptr, uudet[0], uudet[1]);
	    return 1;
	  }
    }

	    std::cout << "   Ei osumaa, alipuuhun siis. " << std::endl;//deb

  if (ehtomuuttujat.size() > 0)
    for (int tt = 0, viimeinen = ehtomuuttujat.size(); tt < viimeinen; ++tt)
      { // Rekursiolla kaikki alipuut.
	if ( (haarat[2*tt]).uusiHaaraZarvolle(parasZarvo, ope.vasenHaara(osajoukko, ehtomuuttujat[tt], ehtojenJakokohdat[tt]), ope) == 1 ) return 1;
	if ( (haarat[2*tt+1]).uusiHaaraZarvolle(parasZarvo, ope.oikeaHaara(osajoukko, ehtomuuttujat[tt], ehtojenJakokohdat[tt]), ope)  == 1) return 1;
      }
}
