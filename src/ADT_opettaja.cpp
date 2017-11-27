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

// Konstruktori, positiiviset ja negatiiviset opetustapaukset streameina.
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
  for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
    {
      painot[tt] = painot[tt] / Z;
    }
}

// Konstruktori: opetusdata vektorina, indeksi kertoo kohdan, josta negatiiviset tapaukset loppuvat ja positiiviset alkavat.
ADT_opettaja::ADT_opettaja(std::vector< std::vector<double> > harjoitusdata, int ekan_pos_ind)
: ekan_positiivisen_indeksi(ekan_pos_ind), opetusdata(harjoitusdata)
{
  double r_nolla = (opetusdata.size() - 2.0 * ekan_positiivisen_indeksi) / (1.0 * opetusdata.size());
  // Laske ekat painot.
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
  for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
    {
      painot[tt] = painot[tt] / Z;
    }
}

// Apufunktio, muuttaa merkkijonon liukuluvuksi.
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

// Palauttaa vektorin, jossa on opetusdatan alkioiden verran alkiota 1.
std::vector<int> ADT_opettaja::kaikkiTapaukset()
{
  std::vector<int> kaikki;
  for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt) kaikki.push_back(1);
  return kaikki;
}

// Palauttaa ADT:n ekan haarakohdan ennustearvon.
double  ADT_opettaja::ekaEnnustearvo()
{
  double r_nolla = (opetusdata.size() - 2.0 * ekan_positiivisen_indeksi) / (1.0*opetusdata.size());
  return 0.5 * log( (1.0+r_nolla)/(1.0-r_nolla)  );
}

// Palauttaa opetusdatan dimension.
int ADT_opettaja::dimensio() { return opetusdata[0].size();}

// Palauttaa jakoehdon perusteella vasempaan haaraan kuuluvan osuuden opetusdatasta.
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

// Palauttaa jakoehdon perusteella oikeaan haaraan kuuluvan osuuden opetusdatasta.
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

// Palauttaa annetun jakoehdon hyvyyden.
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
  double plussat = 0;
  double miinukset = 0;
  // Toka termi: vasemman haaran homogeenisuus.
  std::vector<int> vasemmat = vasenHaara(osajoukko, muuttuja, jakopiste);
  for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt)
    {
      if (vasemmat[tt] == 1) 
	{
	  if (tt < ekan_positiivisen_indeksi)
	    {
	      miinukset += painot[tt];
	    } else {
	    plussat += painot[tt];
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
	      miinukset += painot[tt];
	    } else {
	    plussat += painot[tt];
	  }
	}
    }
  tulos += 2*sqrt(miinukset * plussat);
  return tulos;
}

// Palauttaa tietyn muuttujan mahdolliset jakopisteet opetusdatassa.
std::vector<double> ADT_opettaja::mahdollisetJakopisteet(std::vector<int> osajoukko, int muuttuja)
{
  std::set<double> yksiperarvo_pos, yksiperarvo_neg;
  std::vector<double> tulos, pos, neg;
  for (int tt = 0; tt < ekan_positiivisen_indeksi; ++tt) // Negatiivinen luokka joukoksi ja sitten vektoriksi.
    {
      if (osajoukko[tt] > 0)
	{
	  yksiperarvo_neg.insert( (opetusdata[tt])[muuttuja] );
	}
    }
  for (int tt = ekan_positiivisen_indeksi, loppu = opetusdata.size(); tt < loppu; ++tt) // Positiviinen luokka joukoksi ja sitten vektoriksi.
    {
      if (osajoukko[tt] > 0)
	{
	  yksiperarvo_pos.insert( (opetusdata[tt])[muuttuja] );
	}
    }

  for (std::set<double>::iterator iter_Y = yksiperarvo_pos.begin(), viimeinen_Y = yksiperarvo_pos.end(); iter_Y != viimeinen_Y; ++iter_Y)
    if (*iter_Y == *iter_Y) // Ohitetaan NaN:it
      pos.push_back(*iter_Y);
  for (std::set<double>::iterator iter_Y = yksiperarvo_neg.begin(), viimeinen_Y = yksiperarvo_neg.end(); iter_Y != viimeinen_Y; ++iter_Y)
    if (*iter_Y == *iter_Y) // Ohitetaan NaN:it
      neg.push_back(*iter_Y);
  if (pos.size()==0 || neg.size()==0) return tulos; // Surkastuneet tapaukset.
  std::sort (pos.begin(), pos.end());
  std::sort (neg.begin(), neg.end());
  int p_ind(0), n_ind(0), p_vai_n(0), loppu_p, loppu_n;
  p_vai_n = pos[0] < neg[0] ? 0 : 1;
  loppu_p = pos.size();
  loppu_n = neg.size();
  while (p_ind < loppu_p && n_ind < loppu_n) // Loppuehtona on jomman kumman luokan muuttuja-arvojen loppuun saapuminen.
    if (p_vai_n == 0) {
      while (n_ind < loppu_n && neg[n_ind] <= pos[p_ind]) {
	++n_ind;
      }
      if (n_ind < loppu_n) {
	tulos.push_back( (neg[n_ind]+pos[p_ind]) / 2.0 );
	p_vai_n = 1;
      }
    } else {
      while (p_ind < loppu_p && neg[n_ind] >= pos[p_ind]) {
	++p_ind;
      }
      if (p_ind < loppu_p) {
	tulos.push_back( (neg[n_ind]+pos[p_ind]) / 2.0 );
	p_vai_n = 0;
      }
    }
  return tulos;
}

// Saattaa painot ajan tasalle kun uusi haara on luotu.
ADT_opettaja::uudetPainot(std::vector<int> osajoukko, int muuttuja, double jakopiste, double vasen_ennuste, double oikea_ennuste)
{ 
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
  }
 
  double alpha = 0.5 * log((1-epsilon)/epsilon); // Korjauskerroin.

  // Ensin normalisoimaton ajantasaistaminen.
  double oikea_luokka = -1.0;
  for (int tt = 0, loppu = opetusdata.size(); tt < loppu; ++tt) // Kaikki
    {
      if (tt >= ekan_positiivisen_indeksi) oikea_luokka = 1.0; // Tutkittavien pisteiden todellinen luokka vaihtuu, loput +-luokkaa.
      if (vasemmat[tt] > 0) painot[tt] = painot[tt] * exp(-1.0 * alpha * vasen_ennuste * oikea_luokka);
      if (oikeat[tt] > 0) painot[tt] = painot[tt] * exp(-1.0 * alpha * oikea_ennuste * oikea_luokka);
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

// Palauttaa tietylle jakoehdolla syntyvien uusien haarojen ennustearvot.
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
  miinukset_v = miinukset_v == 0 ? 0.0001 : miinukset_v;
  miinukset_o = miinukset_o == 0 ? 0.0001 : miinukset_o;
  plussat_v = plussat_v == 0 ? 0.0001 : plussat_v;
  plussat_o = plussat_o == 0 ? 0.0001 : plussat_o;
  tulos[0] = 0.5 * log( plussat_v / miinukset_v );
  tulos[1] = 0.5 * log( plussat_o / miinukset_o );
  return tulos;
}
