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
  if(bord < 0 || !integerInBereik(rij, 0, hoogte-1) || !integerInBereik(kolom, 0, breedte-1))
    return -1;
   
  int bitIndex = rij*breedte + kolom;
  
  return geefBit(bord, bitIndex);
}  // getVakje

//*************************************************************************

bool Azul::leesInBord (const char* invoerNaam)
{

    ifstream fin(invoerNaam, std::ifstream::in);
    if(fin.fail()){//error inlezen check
        Azul();
        return false;
    }
    bord = 0;
    fin >> hoogte; 
    fin >> breedte;
    if(hoogte > MaxDimensie || hoogte < 0 || breedte > MaxDimensie || breedte < 0) return false;
    for(int i = 0; i < hoogte*breedte; i++){
      int inhoud; 
      fin >> inhoud;
      if(inhoud)
        bord = bord | (1<<i);
      else
        bord = bord & ~(1<<i);
        
    }
    baseBord = bord;
  return true;

}  // leesInBord

//*************************************************************************

void Azul::drukAfBord ()
{ //TODO druk scores af
  if(bord == -1){
    cout << "Ongeldig bord" << endl;
    return;
  }

  for(int i = 0; i < hoogte*breedte; i++){
    cout << geefBit(bord, i);
    cout << " ";
    if(i%breedte == breedte-1)
      cout << endl;
  }
}  // drukAfBord

//****************************************************************************

bool Azul::doeZet (int rij, int kolom)
{
  if(bord < 0 || !integerInBereik(rij, 0, hoogte-1) || !integerInBereik(kolom, 0, breedte-1))
    return false;

  int bitIndex = rij*breedte + kolom;

  if(geefBit(bord, bitIndex))
    return false;
  
  bord = bord | (1<<bitIndex);
  zetten.push_back(make_pair(rij,kolom));

  return true;

}  // doeZet

//****************************************************************************

bool Azul::unDoeZet ()
{
  if(zetten.size() == 0 || bord < 0)
    return false;

  int unDoeRij = zetten.back().first;
  int unDoeKolom = zetten.back().second;
  zetten.pop_back(); //laatste zet weghalen uit stack
  int bitIndex = unDoeRij*breedte + unDoeKolom;

  bord = bord & ~(1<<bitIndex);

  return true;
}  // unDoeZet

//****************************************************************************

int ScorePlusBijZet(int bord, pair<int,int> zet){
  int bitIndex = zet.first*breedte + zet.second;
}

bool Azul::bepaalMiniMaxiScoreRec (int &mini, long long &volgordesMini,
                                   int &maxi, long long &volgordesMaxi)
{
  // TODO: implementeer deze memberfunctie

  // MAX
  // Base case: checken, (bord == baseBord) maxi = 0;
  // Anders:
  //      in dit geval, volgordes = Som(VolgordesB_i[k] waar MaxScoresB[k] = MaxScore(B))
  if(bord == baseBord)
    maxi = 1;

  int wegTeHalenSteen;

  int preMax = 0;
  int totaalVolgordes = 1;
  for(int i = 0; i<hoogte; i++){
    for(int j = 0; j<breedte; j++){
      int bitIndex = i*breedte + j;
      wegTeHalenSteen = geefBit(bord, bitIndex);
      if (wegTeHalenSteen){
        //steen weghalen 
        bord=bord&~(1<<bitIndex);
        //Recursieve aanroep
        
        bepaalMiniMaxiScoreRec(mini, volgordesMini, maxi, volgordesMaxi);
        if(maxi > preMax){
          totaalVolgordes = volgordesMaxi;
          preMax = maxi;
        }
        if(maxi == preMax){
          totaalVolgordes += volgordesMaxi;
        }

        bord=bord|(1<<bitIndex);
      }
    }
  }

  maxi = preMax;
  volgordesMaxi = totaalVolgordes;

  

  return true;

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

