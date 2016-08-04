/* Luokka laskee ADT:lle sen opettamisvaiheessa tarpeelliset tiedot. */

#ifndef ADT_opettaja_H
#define ADT_opettaja_H

#include <vector>
#include <istream>

struct ADT_opettaja
{
  ADT_opettaja(std::istream &pos, std::istream &neg);
  std::vector<int> vasenHaara(std::vector<int> osajoukko, int muuttuja, double jakopiste);
  std::vector<int> oikeaHaara(std::vector<int> osajoukko, int muuttuja, double jakopiste);
  double Zarvo(std::vector<int> osajoukko, int muuttuja, double jakopiste);
  std::vector<double> mahdollisetJakopisteet(std::vector<int> osajoukko, int muuttuja);
  void uudetPainot(std::vector<int> osajoukko, int muuttuja, double jakopiste, double vasen_ennuste, double oikea_ennuste);
  std::vector<double>  ennustearvot(std::vector<int> osajoukko, int muuttuja, double jakopiste); // Palauttaa 2-vektorissa uusien haarojen arvot.
  double  ekaEnnustearvo(); // Alustusarvo ADT:n juurta varten.
  int dimensio();
  std::vector<int> kaikkiTapaukset(); // Palauttaa kaikkien opetustapausten osajoukkovektorin
private:
  std::vector< std::vector<double> > opetusdata;
  int ekan_positiivisen_indeksi;
  std::vector<double> painot;
  double s2d(std::string s);
};

#endif
