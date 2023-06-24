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


int Azul::ScorePlusBijZet(int bord, pair<int,int> zet){
  int scoreV = 1, scoreH = 1, score;
  for(int j = zet.second+1; j < breedte; j++){
    int bitIndex = zet.first*breedte+j;
    if(geefBit(bord, bitIndex))
      scoreH++;
    else
      break;
  }
  for(int j = zet.second-1; j >= 0; j--){
    int bitIndex = zet.first*breedte+j;
    if(geefBit(bord, bitIndex))
      scoreH++;
    else
      break;
  }  

  for(int i = zet.first+1; i < hoogte; i++){
    int bitIndex = i*breedte+zet.second;
    if(geefBit(bord, bitIndex))
      scoreV++;
    else
      break;
  }
  for(int i = zet.first-1; i >= 0; i--){
    int bitIndex = i*breedte+zet.second;
    if(geefBit(bord, bitIndex))
      scoreV++;
    else
      break;
  }

  if(scoreV == 1)
    score = scoreH;
  else {
    if (scoreH == 1)
      score = scoreV;
    else{
      score = scoreV + scoreH;
    } 
  }

  return score;
}

bool Azul::bepaalMiniMaxiScoreRec (int &mini, long long &volgordesMini,
                              int &maxi, long long &volgordesMaxi){
  bord = (1<<(hoogte*breedte)) - 1; //maak het hele bord vol
  hulpBepaalMiniMaxiScoreRec(mini, volgordesMini, maxi, volgordesMaxi);
  bord = baseBord;
  return true;

}

int Azul::aantalTegels(int speelBord){
  int count = 0;
    while (speelBord != 0) {
        if ((speelBord & 1) == 1) {
            count++;
        }
        speelBord >>= 1;
    }
    return count;
}

vector<int> Azul::getDeelOplossingen(int bord) {
    vector<int> deelOplossingen;

    for(int i = 0 ; i<breedte*hoogte; i++){
        if(geefBit(bord, i) && (bord&~(1<<i)) >= baseBord)
          deelOplossingen.push_back(bord&~(1<<i));
    }

    return deelOplossingen;
}

bool Azul::hulpBepaalMiniMaxiScoreRec (int &mini, long long &volgordesMini,
                                   int &maxi, long long &volgordesMaxi)
{
  if(bord == baseBord){
    maxi = 0;
    volgordesMaxi = 1;
    mini = 0;
    volgordesMini = 1;
  } 
  else{
    int wegTeHalenSteen;
    int magNietZet;
    int preMax = 0;
    int preMin = INT32_MAX;
    int totaalVolgordesMax = 0;
    int totaalVolgordesMin = 0;
    for(int i = 0; i<hoogte; i++){
      for(int j = 0; j<breedte; j++){
        int bitIndex = i*breedte + j;
        wegTeHalenSteen = geefBit(bord, bitIndex);
        magNietZet = geefBit(baseBord, bitIndex);
        if (wegTeHalenSteen && !magNietZet){
          //steen weghalen 
          bord=bord & ~(1<<bitIndex);
         //Recursieve aanroep
            hulpBepaalMiniMaxiScoreRec(mini, volgordesMini, maxi, volgordesMaxi);
            int scoreBijTerugPlaatsen = ScorePlusBijZet(bord, make_pair(i,j));
            if(maxi + scoreBijTerugPlaatsen > preMax){
              totaalVolgordesMax = volgordesMaxi;
              preMax = maxi + scoreBijTerugPlaatsen;
            }
            else if(maxi + scoreBijTerugPlaatsen == preMax){
              totaalVolgordesMax += volgordesMaxi;
            }

            if(mini + scoreBijTerugPlaatsen < preMin){
              totaalVolgordesMin = volgordesMini;
              preMin = mini + scoreBijTerugPlaatsen;
            }
            else if(mini + scoreBijTerugPlaatsen == preMin){
              totaalVolgordesMin += volgordesMini;
            }
          bord=bord|(1<<bitIndex);   
        }
      }
    }
    maxi = preMax;
    volgordesMaxi = totaalVolgordesMax;
    mini = preMin;
    volgordesMini = totaalVolgordesMin;
  }
  return true;
}  // bepaalMiniMaxiScoreRec


//*************************************************************************

pair<int,int> Azul::binaryNaarZet(int binZet){
  int bitIndex = 0;
  for(int i = 0; i<breedte*hoogte; i++){
    if((binZet>>i)%2 == 1){
      bitIndex = i;
      break;
    }
  }
  int zetI = bitIndex/breedte;
  int zetJ = bitIndex % breedte;
  return make_pair(zetI, zetJ);
}



bool Azul::hulpTD(vector<int> &maxiMemo, vector<long long> &volgordesMaxiMemo, vector<int> &miniMemo, vector<long long> &volgordesMiniMemo)
{
  if(bord == baseBord){
    maxiMemo[bord] = 0;
    volgordesMaxiMemo[bord] = 1;
    miniMemo[bord] = 0;
    volgordesMiniMemo[bord] = 1;
    return true;
  } 
  else{
    if(aantalTegels(bord) == aantalTegels(baseBord)){
      maxiMemo[bord] = 0;
      volgordesMaxiMemo[bord] = 0;
      miniMemo[bord] = 0;
      volgordesMiniMemo[bord] = 0;
      return false;
    }
    int preMax = 0;
    long long totaalVolgordesMaxi = 0;
    int preMin = INT32_MAX;
    long long totaalVolgordesMini = 0;
    int wegTeHalenSteen;
    
    for(int i = 0; i<hoogte; i++){
      for(int j = 0; j<breedte; j++){
        int bitIndex = i*breedte + j;
        wegTeHalenSteen = geefBit(bord, bitIndex);
        if (wegTeHalenSteen){
          
          //steen weghalen 
          bord=bord & ~(1<<bitIndex);
          //Recursieve aanroep
          if(volgordesMaxiMemo[bord] == -1)
            hulpTD(maxiMemo, volgordesMaxiMemo, miniMemo, volgordesMiniMemo);
          if(volgordesMaxiMemo[bord] > 0)
          {
            int scoreBijTerugPlaatsen = ScorePlusBijZet(bord, make_pair(i,j));
          
            if(maxiMemo[bord] + scoreBijTerugPlaatsen > preMax){
              totaalVolgordesMaxi = volgordesMaxiMemo[bord];
              preMax = maxiMemo[bord] + scoreBijTerugPlaatsen;
            }
            else if(maxiMemo[bord] + scoreBijTerugPlaatsen == preMax){
              totaalVolgordesMaxi += volgordesMaxiMemo[bord];
            }

            if(miniMemo[bord] + scoreBijTerugPlaatsen < preMin){
              totaalVolgordesMini = volgordesMiniMemo[bord];
              preMin = miniMemo[bord] + scoreBijTerugPlaatsen;
            }
            else if(miniMemo[bord] + scoreBijTerugPlaatsen == preMin){
              totaalVolgordesMini += volgordesMiniMemo[bord];
            }

          }
          
          bord=bord|(1<<bitIndex);   
        }
      }
    }
    maxiMemo[bord] = preMax;
    volgordesMaxiMemo[bord] = totaalVolgordesMaxi;
    miniMemo[bord] = preMin;
    volgordesMiniMemo[bord] = totaalVolgordesMini;
  }
  return true;
}



bool Azul::bepaalMiniMaxiScoreTD (int &mini, long long &volgordesMini,
                                  int &maxi, long long &volgordesMaxi)
{
  int grens = (1<<(breedte*hoogte))-1; //vol bord
  cout << grens;
  vector<int> miniMemo(grens+1, -1);
  vector<int> maxiMemo(grens+1, -1);
  vector<long long> maxiVolgordesMemo(grens+1, -1);
  vector<long long> miniVolgordesMemo(grens+1, -1);

  bord = grens;
  hulpTD(maxiMemo, maxiVolgordesMemo, miniMemo, miniVolgordesMemo);
  bord = baseBord;
  maxi = maxiMemo[grens];
  volgordesMaxi = maxiVolgordesMemo[grens];
  mini = miniMemo[grens];
  volgordesMini = miniVolgordesMemo[grens];
  return true;
}  // bepaalMiniMaxiScoreTD

//*************************************************************************

bool Azul::bepaalMiniMaxiScoreBU (int &mini, long long &volgordesMini,
                                  int &maxi, long long &volgordesMaxi,
                                  vector< pair<int,int> > &zettenReeksMini,
                                  vector< pair<int,int> > &zettenReeksMaxi)
{
  int grens = (1<<(breedte*hoogte))-1;
  vector<int> miniMemo(grens+1, INT32_MAX);
  vector<int> maxiMemo(grens+1, -1);
  vector<long long> maxiVolgordesMemo(grens+1, 0);
  vector<long long> miniVolgordesMemo(grens+1, 0);
  
  miniMemo[bord] = 0;
  maxiMemo[bord] = 0;
  miniVolgordesMemo[bord] = 1;
  maxiVolgordesMemo[bord] = 1;

  for(bord = bord+1; bord<=grens; bord++)
  {
    pair<int,int> miniZet;
    pair<int,int> maxiZet;
    vector<int> deelOplossingen = getDeelOplossingen(bord);
    for(size_t j = 0; j<deelOplossingen.size(); j++){
      if(maxiMemo[deelOplossingen[j]] == -1)
        continue;
      int scoreBijTerugPlaatsen = ScorePlusBijZet(deelOplossingen[j], binaryNaarZet(bord-deelOplossingen[j]));
      if(maxiMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen > maxiMemo[bord]){
        maxiZet = binaryNaarZet(bord-deelOplossingen[j]);
        maxiMemo[bord] = maxiMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen;
        maxiVolgordesMemo[bord] = maxiVolgordesMemo[deelOplossingen[j]];
      }
      else if(maxiMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen == maxiMemo[bord]){
        maxiVolgordesMemo[bord] += maxiVolgordesMemo[deelOplossingen[j]];
      }

      if(miniMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen < miniMemo[bord]){
        miniZet = binaryNaarZet(bord-deelOplossingen[j]);
        miniMemo[bord] = miniMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen;
        miniVolgordesMemo[bord] = miniVolgordesMemo[deelOplossingen[j]];
      }
      else if(miniMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen == miniMemo[bord]){
        miniVolgordesMemo[bord] += miniVolgordesMemo[deelOplossingen[j]];
      }
    }
    zettenReeksMaxi.push_back(maxiZet);
    zettenReeksMini.push_back(miniZet);
  }
  maxi = maxiMemo[grens];
  volgordesMaxi = maxiVolgordesMemo[grens];
  mini = miniMemo[grens];
  volgordesMini = miniVolgordesMemo[grens];

  bord = baseBord;
  return true;

}  // bepaalMiniMaxiScoreBU

//*************************************************************************

void Azul::drukAfZettenReeksen (vector<pair <int,int> > &zettenReeksMini,
                                vector<pair <int,int> > &zettenReeksMaxi)
{
  // TODO: implementeer deze memberfunctie
  for(size_t i = 0; i < zettenReeksMaxi.size(); i++){
    cout << "("<<zettenReeksMaxi[i].first << ", " << zettenReeksMaxi[i].second << ") " << ScorePlusBijZet(bord, zettenReeksMaxi[i]) << endl;
    doeZet(zettenReeksMaxi[i].first, zettenReeksMaxi[i].second); 
  }
  bord = baseBord;
  for(size_t i = 0; i < zettenReeksMini.size(); i++){

  }
  bord = baseBord;
}  // drukAfZettenReeksen

//*************************************************************************

// TODO: implementatie van uw eigen private memberfuncties

