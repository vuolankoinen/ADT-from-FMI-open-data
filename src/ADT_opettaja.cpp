/* Luokka laskee ADT:lle sen opettamisvaiheessa tarpeelliset tiedot. */

#include "ADT_opettaja.h"
#include <cmath> // log (= ln)
#include <set>

#include <iostream>
#include <istream>
#include <algorithm>
#include <string>
#include <limits>
#include <sstream> // s2d vaatii

ADT_opettaja::ADT_opettaja(std::istream &pos, std::istream &neg) 
{
// Lue opetusdatat tiedostosta.
  std::string rivi, ehdokas;
int laskuri = 0;
 double luku;
  while (getline(neg, rivi))
    {
std::vector<double> uusin;
      std::stringstream ss(rivi);
	while (ss >> ehdokas) 
	  {
luku = s2d(ehdokas);
uusin.push_back(luku);
	  }
opetusdata.push_back(uusin);
++laskuri;
}
ekan_positiivisen_indeksi = laskuri;
std::cout << "\n1. positiivinen indeksi: " << ekan_positiivisen_indeksi << std::endl;//deb
  while (getline(pos, rivi))
    {
std::vector<double> uusin;
      std::stringstream ss(rivi);
	while (ss >> ehdokas) 
	  {
luku = s2d(ehdokas);
uusin.push_back(luku);
	  }
opetusdata.push_back(uusin);
}


  double r_nolla = (opetusdata.size() - 2.0 * ekan_positiivisen_indeksi) / (1.0 * opetusdata.size());

// Laske ekat painot.
// std::cout << "Painojen r_0 = " << r_nolla << "  " << opetusdata.size()<< std::endl;//deb
 for (int tt = 0; tt < ekan_positiivisen_indeksi; ++tt) painot.push_back(1.0/(1.0-r_nolla));
 for (int tt = ekan_positiivisen_indeksi, loppu = opetusdata.size(); tt < loppu; ++tt) 
   {
   painot.push_back( 1.0/(1.0+r_nolla) );
   }
 // Normalisointivakio.
 double Z;
 for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
   {
     Z += painot[tt];
   }
 // std::cout << "Painojen normalisointivakio = " << Z << std::endl;//deb
 for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
   {
     painot[tt] = painot[tt] / Z;
   }
 // std::cout << " Ekat painot: "  << std::endl;//deb
 // for (int tt = 0; tt < painot.size();++tt) std::cout << " "<<painot[tt];//deb
 //std::cout << "r_0: " << r_nolla   << std::endl;//deb
}

double ADT_opettaja::s2d(std::string s) 
{
  double d;
  std::stringstream ss(s);
  ss >> d;
  if (d==0) { // Tarkistetaan NaN:in varalta.
    if (s.find_first_of("0123456789") > s.size())  return std::numeric_limits<double>::quiet_NaN(); // Ei ole numeroa, joten NaN.
  }
  return d;
}

std::vector<int> ADT_opettaja::kaikkiTapaukset()
{
  std::vector<int> kaikki;
  for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt) kaikki.push_back(1);
  return kaikki;
}

double  ADT_opettaja::ekaEnnustearvo()
{
  double r_nolla = (opetusdata.size() - 2.0 * ekan_positiivisen_indeksi) / (1.0*opetusdata.size());
  return 0.5 * log( (1.0+r_nolla)/(1.0-r_nolla)  );
}

int ADT_opettaja::dimensio() { return opetusdata[0].size();}

std::vector<int> ADT_opettaja::vasenHaara(std::vector<int> osajoukko, int muuttuja, double jakopiste) 
{
std::vector<int> tulos = osajoukko;
for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
{
if (osajoukko[tt] > 0)
  {
if ((opetusdata[tt])[muuttuja] > jakopiste) // Poistetaan.
  tulos[tt] = 0;
}
}
return tulos;
}

std::vector<int> ADT_opettaja::oikeaHaara(std::vector<int> osajoukko, int muuttuja, double jakopiste)
{
std::vector<int> tulos = osajoukko;
for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
{
if (osajoukko[tt] > 0)
  {
if ((opetusdata[tt])[muuttuja] < jakopiste) // Poistetaan.
  tulos[tt] = 0;
}
}
return tulos;
}

double ADT_opettaja::Zarvo(std::vector<int> osajoukko, int muuttuja, double jakopiste)
{   
double tulos(0.0);
 for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt) // Eka termi: kattavuus
   {
     if (osajoukko[tt] == 0) // Ei haarassa -> mukaan
       {
	 tulos += painot[tt];
       }
   }
 int plussat =0;
 int  miinukset=0;
// Toka termi: vasemman haaran homogeenisuus.
 std::vector<int> vasemmat = vasenHaara(osajoukko, muuttuja, jakopiste);
 for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
   {
     if (vasemmat[tt] == 1) 
       {
	 if (tt < ekan_positiivisen_indeksi)
	   {
	     ++miinukset ;
	   } else {
	   ++plussat;
	 }
       }
   }
 tulos += 2*sqrt(miinukset * plussat);
// Kolmas termi: oikean haaran homogeenisuus.
plussat = 0;
miinukset = 0;
std::vector<int> oikeat = oikeaHaara(osajoukko, muuttuja, jakopiste);
for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
  {
    if (oikeat[tt] == 1) 
      {
	if (tt < ekan_positiivisen_indeksi)
	  {
	    ++miinukset ;
	  } else {
	  ++plussat;
	}
      }
  }
    tulos += 2*sqrt(miinukset * plussat);
    return tulos;
}

std::vector<double> ADT_opettaja::mahdollisetJakopisteet(std::vector<int> osajoukko, int muuttuja)
{
  std::set<double> yksiperarvo;
  std::vector<double> tulos;
for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
  {
if (osajoukko[tt] > 0)
  {
yksiperarvo.insert( (opetusdata[tt])[muuttuja] );
}
}
  //  yksiperarvo.erase(std::numeric_limits<double>::quiet_NaN());
    for (std::set<double>::iterator iter_Y = yksiperarvo.begin(), viimeinen_Y = yksiperarvo.end(); iter_Y != viimeinen_Y; ++iter_Y)
      tulos.push_back(*iter_Y);
if (tulos.size()<2) return tulos; // Surkastuneet tapaukset.
  std::sort (tulos.begin(), tulos.end());
  for (int tt=0, viimeinen = tulos.size()-1; tt < viimeinen; ++tt)
    tulos[tt] = (tulos[tt] + tulos[tt+1]) / 2.0;
  tulos.pop_back();
for (std::vector<double>::iterator iter_Y = tulos.begin(), viimeinen_Y = tulos.end(); iter_Y != viimeinen_Y; ++iter_Y) std::cout <<*iter_Y<< " ";//deb
  return tulos;
}

 void ADT_opettaja::uudetPainot(std::vector<int> osajoukko, int muuttuja, double jakopiste, double vasen_ennuste, double oikea_ennuste)
{ // Saattaa painot ajan tasalle kun uusi haara on luotu.

  double epsilon;
  std::vector<int> vasemmat, oikeat;
  vasemmat = vasenHaara(osajoukko, muuttuja, jakopiste);
  oikeat = oikeaHaara(osajoukko, muuttuja, jakopiste);

    // Vasemman haaran virheiden osuus.
  if (vasen_ennuste < 0)
    {
      for (int tt = ekan_positiivisen_indeksi, loppu = opetusdata.size(); tt < loppu; ++tt) // Vain positiiviset on huteja.
	if (vasemmat[tt]>0)
	  epsilon += painot[tt];
    } else  {
    for (int tt = 0; tt < ekan_positiivisen_indeksi; ++tt)  // Kolutaan vain negatiiviset
      if (vasemmat[tt]>0)
	epsilon += painot[tt];
  }
 
  // Oikean haaran virheiden osuus.
  if (oikea_ennuste < 0)
    {
      for (int tt = ekan_positiivisen_indeksi, loppu = opetusdata.size(); tt < loppu; ++tt) // Vain positiiviset on huteja.
	{
	  if (oikeat[tt]>0)
	    epsilon += painot[tt];
	}
    } else  {
    for (int tt = 0; tt < ekan_positiivisen_indeksi; ++tt)  // Kolutaan vain negatiiviset
      {
	if (oikeat[tt]>0)
	  epsilon += painot[tt];
      }
 
    double alpha = 0.5 * log((1-epsilon)/epsilon); // Korjauskerroin.


    // Ensin normalisoimaton ajantasaistaminen.
    double vas = vasen_ennuste < 0 ? -1.0 : 1.0;
    double oik = oikea_ennuste < 0 ? -1.0 : 1.0;
    for (int tt = 0; tt < ekan_positiivisen_indeksi; ++tt) // negatiiviset
      {
	if (vasemmat[tt] > 0) painot[tt] = painot[tt] * exp(alpha * vas);
	if (oikeat[tt] > 0) painot[tt] = painot[tt] * exp(alpha * oik);
      }
    for (int tt = ekan_positiivisen_indeksi, loppu = opetusdata.size(); tt < loppu; ++tt) // positiiviset
      {
	if (vasemmat[tt] > 0) painot[tt] = painot[tt] * exp(-1.0 * alpha * vas);
	if (oikeat[tt] > 0) painot[tt] = painot[tt] * exp(-1.0 * alpha * oik);
      }
 
    // Normalisointivakio.
    double Z;
    for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
      {
	Z += painot[tt];
      }
    for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
      {
	painot[tt] = painot[tt] / Z;
      }
  }
}

 std::vector<double>  ADT_opettaja::ennustearvot(std::vector<int> osajoukko, int muuttuja, double jakopiste)
  {
    std::vector<double> tulos(2);
    std::vector<int> vas, oik;
    double plussat_v = 0, miinukset_v = 0, plussat_o = 0, miinukset_o = 0;
    vas = vasenHaara( osajoukko, muuttuja, jakopiste);
    oik = oikeaHaara( osajoukko, muuttuja, jakopiste);
    for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
      {
	if (vas[tt] > 0)
	  {
	    if (tt < ekan_positiivisen_indeksi) {
	      ++miinukset_v;
	    } else {
	      ++plussat_v;
	    }
	  } else if (oik[tt] > 0)
	  {
	    if (tt < ekan_positiivisen_indeksi) {
	      ++miinukset_o;
	    } else {
	      ++plussat_o;
	    }
	  }
      }
    std::cout << "v- " << miinukset_v << " v+ "<<plussat_v<<std::endl;//deb
    std::cout << "o- " << miinukset_o << " o+ "<<plussat_o<<std::endl;//deb
    miinukset_v = miinukset_v == 0 ? 0.0001 : miinukset_v;
    miinukset_o = miinukset_o == 0 ? 0.0001 : miinukset_o;
    plussat_v = plussat_v == 0 ? 0.0001 : plussat_v;
    plussat_o = plussat_o == 0 ? 0.0001 : plussat_o;
    tulos[0] = 0.5 * log( plussat_v / miinukset_v );
    tulos[1] = 0.5 * log( plussat_o / miinukset_o );
    return tulos;
  }
