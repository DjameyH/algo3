// Implementatie van klasse Azul

#include <iostream>
#include <fstream>
#include "standaard.h"
#include "azul.h"
using namespace std;

//*************************************************************************

Azul::Azul ()
{

}  // default constructor

//*************************************************************************

Azul::Azul (int nwHoogte, int nwBreedte)
{
  hoogte = nwHoogte;
  breedte = nwBreedte;
  bord = 0;
  baseBord=0;
}  // constructor met parameters

//*************************************************************************

Azul::~Azul ()
{

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

vector<int> Azul::getDeelOplossingen(int bord) {
    vector<int> deelOplossingen;

    for(int i = 0 ; i<breedte*hoogte; i++){
        if(geefBit(bord, i) && (bord&~(1<<i)) >= baseBord)
          deelOplossingen.push_back(bord&~(1<<i));
    }
    return deelOplossingen;
}

void Azul::hulpBepaalMiniMaxiScoreRec (int &mini, long long &volgordesMini,
                                   int &maxi, long long &volgordesMaxi)
{
  if(bord == baseBord){
    maxi = 0;
    volgordesMaxi = 1;
    mini = 0;
    volgordesMini = 1;
  } 
  else{
    int preMax = 0, preMin = INT32_MAX;
    long long totaalVolgordesMax = 0, totaalVolgordesMin = 0;
    for(int i = 0; i<hoogte; i++){
      for(int j = 0; j<breedte; j++){
        int bitIndex = i*breedte + j;
        int wegTeHalenSteen = geefBit(bord, bitIndex);
        int magNietZet = geefBit(baseBord, bitIndex);
        if (wegTeHalenSteen && !magNietZet){
          bord=bord & ~(1<<bitIndex); //steen weghalen 
          hulpBepaalMiniMaxiScoreRec(mini, volgordesMini, maxi, volgordesMaxi);//Recursieve aanroep
          int scoreBijTerugPlaatsen = ScorePlusBijZet(bord, make_pair(i,j));
          if(maxi + scoreBijTerugPlaatsen > preMax){
            totaalVolgordesMax = volgordesMaxi;
            preMax = maxi + scoreBijTerugPlaatsen;
          }
          else if(maxi + scoreBijTerugPlaatsen == preMax){ totaalVolgordesMax += volgordesMaxi; }
          if(mini + scoreBijTerugPlaatsen < preMin){
            totaalVolgordesMin = volgordesMini;
            preMin = mini + scoreBijTerugPlaatsen;
          }
          else if(mini + scoreBijTerugPlaatsen == preMin){ totaalVolgordesMin += volgordesMini; }
          bord=bord|(1<<bitIndex);   
        }
      }
    }
    maxi = preMax, volgordesMaxi = totaalVolgordesMax, mini = preMin, volgordesMini = totaalVolgordesMin;
  }
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



void Azul::hulpTD(vector<int> &maxiMemo, vector<long long> &volgordesMaxiMemo, vector<int> &miniMemo, vector<long long> &volgordesMiniMemo)
{
  if(bord == baseBord)
    maxiMemo[bord] = 0, miniMemo[bord] = 0, volgordesMaxiMemo[bord] = 1, volgordesMiniMemo[bord] = 1;
  else{
    int preMax = 0, preMin = INT32_MAX;
    long long totaalVolgordesMaxi = 0,totaalVolgordesMini = 0;
    for(int i = 0; i<hoogte; i++){
      for(int j = 0; j<breedte; j++){
        int bitIndex = i*breedte + j;
        int wegTeHalenSteen = geefBit(bord, bitIndex);
        int magNietZet = geefBit(baseBord, bitIndex);
        if (wegTeHalenSteen && !magNietZet){
          bord=bord & ~(1<<bitIndex); //steen weghalen 
          if(volgordesMaxiMemo[bord] == -1)
            hulpTD(maxiMemo, volgordesMaxiMemo, miniMemo, volgordesMiniMemo); //Recursieve aanroep
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
          bord=bord|(1<<bitIndex); //steen terugzetten
        }
      }
    }
    maxiMemo[bord] = preMax, volgordesMaxiMemo[bord] = totaalVolgordesMaxi, miniMemo[bord] = preMin, volgordesMiniMemo[bord] = totaalVolgordesMini;
  }
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
  zettenReeksMaxi.clear();
  zettenReeksMini.clear();
  int grens = (1<<(breedte*hoogte))-1;
  vector<int> miniMemo(grens+1, INT32_MAX);
  vector<int> maxiMemo(grens+1, -1);
  vector<long long> maxiVolgordesMemo(grens+1, 0);
  vector<long long> miniVolgordesMemo(grens+1, 0);
  vector<int> gebruikteDeeloplossingenMaxi(grens+1, 0);
  vector<int> gebruikteDeeloplossingenMini(grens+1, 0);
  miniMemo[bord] = 0, maxiMemo[bord] = 0, miniVolgordesMemo[bord] = 1, maxiVolgordesMemo[bord] = 1;
  for(bord = bord+1; bord<=grens; bord++)
  {
    vector<int> deelOplossingen = getDeelOplossingen(bord);
    int gebruikteDeeloplossingMaxi = 0;
    int gebruikteDeeloplossingMini = 0;
    for(size_t j = 0; j<deelOplossingen.size(); j++){
      if(maxiMemo[deelOplossingen[j]] == -1)
        continue;
      int scoreBijTerugPlaatsen = ScorePlusBijZet(deelOplossingen[j], binaryNaarZet(bord-deelOplossingen[j]));
      if(maxiMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen > maxiMemo[bord]){
        gebruikteDeeloplossingMaxi = deelOplossingen[j];
        maxiMemo[bord] = maxiMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen;
        maxiVolgordesMemo[bord] = maxiVolgordesMemo[deelOplossingen[j]];
      }
      else if(maxiMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen == maxiMemo[bord]){
        maxiVolgordesMemo[bord] += maxiVolgordesMemo[deelOplossingen[j]];
      }
      if(miniMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen < miniMemo[bord]){
        gebruikteDeeloplossingMini = deelOplossingen[j];
        miniMemo[bord] = miniMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen;
        miniVolgordesMemo[bord] = miniVolgordesMemo[deelOplossingen[j]];
      }
      else if(miniMemo[deelOplossingen[j]] + scoreBijTerugPlaatsen == miniMemo[bord]){
        miniVolgordesMemo[bord] += miniVolgordesMemo[deelOplossingen[j]];
      }
    }
    gebruikteDeeloplossingenMaxi[bord]=gebruikteDeeloplossingMaxi;
    gebruikteDeeloplossingenMini[bord]=gebruikteDeeloplossingMini;
  }
  maxi = maxiMemo[grens];
  volgordesMaxi = maxiVolgordesMemo[grens];
  mini = miniMemo[grens];
  volgordesMini = miniVolgordesMemo[grens];
  int i = grens;
  while(i != baseBord){
    zettenReeksMaxi.insert(zettenReeksMaxi.begin(), binaryNaarZet(i-gebruikteDeeloplossingenMaxi[i]));
    i = gebruikteDeeloplossingenMaxi[i];
  }
  i=grens;
  while(i != baseBord){
    zettenReeksMini.insert(zettenReeksMini.begin(), binaryNaarZet(i-gebruikteDeeloplossingenMini[i]));
    i = gebruikteDeeloplossingenMini[i];
  }
  bord = baseBord;
  return true;
}  // bepaalMiniMaxiScoreBU

//*************************************************************************

void Azul::drukAfZettenReeksen (vector<pair <int,int> > &zettenReeksMini,
                                vector<pair <int,int> > &zettenReeksMaxi)
{
  cout << "Maxi zettenreeks" << endl;
  for(size_t i = 0; i < zettenReeksMaxi.size(); i++){
    cout << "("<<zettenReeksMaxi[i].first << ", " << zettenReeksMaxi[i].second << ") " << ScorePlusBijZet(bord, zettenReeksMaxi[i]) << endl;
    doeZet(zettenReeksMaxi[i].first, zettenReeksMaxi[i].second); 
  }
  while(zetten.size() != 0){
    unDoeZet();
  }
  bord = baseBord;
  cout << endl << endl;
  cout << "Mini zettenreeks" << endl;
  for(size_t i = 0; i < zettenReeksMini.size(); i++){
    cout << "("<<zettenReeksMini[i].first << ", " << zettenReeksMini[i].second << ") " << ScorePlusBijZet(bord, zettenReeksMini[i]) << endl;
    doeZet(zettenReeksMini[i].first, zettenReeksMini[i].second); 
  }
  while(zetten.size() != 0){
    unDoeZet();
  }
  bord = baseBord;
}  // drukAfZettenReeksen

//*************************************************************************

// TODO: implementatie van uw eigen private memberfuncties

