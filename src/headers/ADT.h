#ifndef ADT_H
#define ADT_H

#include <vector>
#include <string>
#include "ADT_opettaja.h"

struct ADT
{
  // Konstruktori:
  ADT();
  // Konstruktori, luo uuden puun antamalla juurelle ADT_opettaja-luokan ekaEnnustearvo-metodin palauttama konstruktorille:
  ADT(double alkuarvo); 

  // Palauttaa ADT:n datalle tuottaman ennusteen - >0  ennustaa sadetta:
  double ennuste(std::vector<double> data);

  // Luo puuhun uusia haaroja montako kappaletta:
  void uusiaHaaroja(int montako, ADT_opettaja & ope);

  // Puun rakenne merkkijonona:
  std::string rakenne(std::string sisennys);


private:
  double ennustearvo;
  std::vector<ADT> haarat;
  std::vector<int> ehtomuuttujat;
  std::vector<double> ehtojenJakokohdat;
  double parasZarvo(std::vector<int> osajoukko, ADT_opettaja & ope);
  int uusiHaaraZarvolle(double Zarvo, std::vector<int> osajoukko, ADT_opettaja ope);
};

#endif
