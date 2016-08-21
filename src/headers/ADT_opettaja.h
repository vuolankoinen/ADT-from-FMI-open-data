/* Luokka laskee ADT:lle sen opettamisvaiheessa tarpeelliset tiedot. */

#ifndef ADT_opettaja_H
#define ADT_opettaja_H

#include <vector>
#include <istream>

struct ADT_opettaja
{
  // Konstruktori. Opetusdatan kaksi luokkaa tiedostoissa parametreina.
  ADT_opettaja(std::istream &pos, std::istream &neg);
  std::vector<int> vasenHaara(std::vector<int> osajoukko, int muuttuja, double jakopiste);
  std::vector<int> oikeaHaara(std::vector<int> osajoukko, int muuttuja, double jakopiste);
  // Laskee tietyn mahdollisen uuden haaran hyvyyden.
  double Zarvo(std::vector<int> osajoukko, int muuttuja, double jakopiste);
  // Laskee muuttujan arvot, joiden kohdalle voisi uuden splitter noden asettaa.
  std::vector<double> mahdollisetJakopisteet(std::vector<int> osajoukko, int muuttuja);
  // Ajantasaistaa boostauksessa tarpeelliset opetusdatan painokertoimet.
  void uudetPainot(std::vector<int> osajoukko, int muuttuja, double jakopiste, double vasen_ennuste, double oikea_ennuste);
  // Palauttaa 2-vektorissa uusien haarojen konstruktorien tarvitsemat ennustearvot.
  std::vector<double> ennustearvot(std::vector<int> osajoukko, int muuttuja, double jakopiste);
  // Konstruktorin tarvitsema alustusarvo ADT:n juurta varten.
  double ekaEnnustearvo();
  // Datan dimensio.
  int dimensio();
  // Palauttaa kaikkien opetustapausten osajoukkovektorin.
  std::vector<int> kaikkiTapaukset();
  // Turhaksi!!!! muuttunut metodi, tarkistaa, onko harjoitusdatan osajoukko kokonaan samaa luokkaa.
  bool onko_lehti(std::vector<int> osajoukko);
private:
  std::vector< std::vector<double> > opetusdata;
  int ekan_positiivisen_indeksi;
  std::vector<double> painot;
  double s2d(std::string s);
};

#endif
