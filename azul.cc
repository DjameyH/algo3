// Implementatie van klasse Azul

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "azul.h"
using namespace std;

//*************************************************************************

Azul::Azul ()
{
  // TODO: implementeer zo nodig deze constructor

}  // default constructor

//*************************************************************************

Azul::Azul (int nwHoogte, int nwBreedte)
{
  // TODO: implementeer deze constructor

}  // constructor met parameters

//*************************************************************************

Azul::~Azul ()
{
  // TODO: implementeer zo nodig deze destructor

}  // destructor

//*************************************************************************

int Azul::getVakje (int rij, int kolom)
{ 
  // TODO: implementeer deze memberfunctie

  return -1;

}  // getVakje

//*************************************************************************

bool Azul::leesInBord (const char* invoerNaam)
{

    ifstream fin(invoerNaam, std::ifstream::in);
    if(fin.fail()){//error inlezen check
        Azul();
        return false;
    }
    fin >> hoogte; 
    fin >> breedte;
    if(hoogte > MaxDimensie || hoogte < 0 || breedte > MaxDimensie || breedte < 0) return false;
    for(int i = 0; i < hoogte; i++){
        for(int j = 0; j < breedte; j++){
            fin >> Bord[i][j];
            if(Bord[i][j] != 0 && Bord[i][j] != 1) return false;
        }
    }

  return true;

}  // leesInBord

//*************************************************************************

void Azul::drukAfBord ()
{
  for(int i = 0; i < hoogte; i++){
      for(int j = 0; j < breedte; j++){
          cout << Bord[i][j] << ' ';
      }
      cout << endl;
  }
}  // drukAfBord

//****************************************************************************

bool Azul::doeZet (int rij, int kolom)
{
  // TODO: implementeer deze memberfunctie

  return false;

}  // doeZet

//****************************************************************************

bool Azul::unDoeZet ()
{
  // TODO: implementeer deze memberfunctie

  return false;

}  // unDoeZet

//****************************************************************************

bool Azul::bepaalMiniMaxiScoreRec (int &mini, long long &volgordesMini,
                                   int &maxi, long long &volgordesMaxi)
{
  // TODO: implementeer deze memberfunctie
  
  return false;

}  // bepaalMiniMaxiScoreRec

//*************************************************************************

bool Azul::bepaalMiniMaxiScoreTD (int &mini, long long &volgordesMini,
                                  int &maxi, long long &volgordesMaxi)
{
  // TODO: implementeer deze memberfunctie

  return false;

}  // bepaalMiniMaxiScoreTD

//*************************************************************************

bool Azul::bepaalMiniMaxiScoreBU (int &mini, long long &volgordesMini,
                                  int &maxi, long long &volgordesMaxi,
                                  vector< pair<int,int> > &zettenReeksMini,
                                  vector< pair<int,int> > &zettenReeksMaxi)
{
  // TODO: implementeer deze memberfunctie

  return false;

}  // bepaalMiniMaxiScoreBU

//*************************************************************************

void Azul::drukAfZettenReeksen (vector<pair <int,int> > &zettenReeksMini,
                                vector<pair <int,int> > &zettenReeksMaxi)
{
  // TODO: implementeer deze memberfunctie

}  // drukAfZettenReeksen

//*************************************************************************

// TODO: implementatie van uw eigen private memberfuncties

