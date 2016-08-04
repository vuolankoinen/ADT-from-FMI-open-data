#ifndef ADT_H
#define ADT_H

#include <vector>
#include "ADT_opettaja.h"

struct ADT
{
  // Konstruktorit
  ADT();
  ADT(double alkuarvo); // Luo uusi puu antamalla juurelle ADT_opettaja-luokan ekaEnnustearvo-metodin palauttama konstruktorille.
  double ennuste(std::vector<double> data);
  // Opeta algoritmi:
  void uusiaHaaroja(int montako, ADT_opettaja ope);
private:
  double ennustearvo;
  std::vector<ADT> haarat;
  std::vector<int> ehtomuuttujat;
  std::vector<double> ehtojenJakokohdat;
  double parasZarvo(std::vector<int> osajoukko, ADT_opettaja ope);
  int uusiHaaraZarvolle(double Zarvo, std::vector<int> osajoukko, ADT_opettaja ope);
};

#endif
