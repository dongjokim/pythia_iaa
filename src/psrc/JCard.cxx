//====================================
//last modified FK 6.NOV 2009
//====================================

#include "JCard.h"

//ClassImp(JCard)


JCard::JCard(char *filename){   //constructor
 
  incard.open(filename,ios::in);
 
  if(!incard){
    cout<<"ERROR: Config file <"<<filename<<"> not found!"<<endl;
    exit(1);
  }

  strcpy(cardname, filename);//needed in Print()
 
  //   i n i t i a l i z a t i o n

  nentry = 0; //initialize nr of lines
 
 
  //----  r e a d   t h e   c a r d ----
 
  ReadInputCard();//read config file fill Tvectors
}



unsigned int JCard::GetTVectorIndex(TString keyword){
  //returns index of a TVector according to its position in map 
  map< TString, unsigned int >::iterator iter = MapKeyWordToTVector.begin();
  iter = MapKeyWordToTVector.find(keyword);
  if(iter != MapKeyWordToTVector.end()){
    return (unsigned int) iter->second;
  }else{
    cout << "ERROR: \""<<keyword.Data()<<"\" must be defined  "<< endl;
    exit(1);
  }
}

TString JCard::GetKeyWord(corrType ctype){
   TString kw;
 
   switch(ctype){
     case kTriggType:  kw = "TriggPtBorders"; break;
     case kAssocType:  kw = "AssocPtBorders"; break;
     case kLongType:   kw = "KlongBorders"; break;
     case kCentrType:  kw = "CentBinBorders"; break;
     case kMassType:   kw = "PairInvariantMassBins"; break;
     case kEtaGapType: kw = "EtaGapThresholds"; break;
     case kZVertType:  kw = "zVertBins"; break;
     default : cout<<"ERROR: kNoType on input to JCard::GetKeyWord"<<endl; exit(1); break;
   }
  return kw; 
}


int JCard::GetN(TString keyword){
  //returns size of TVector
  unsigned int index = GetTVectorIndex(keyword);
  return (int) fValuesVector[index].GetNrows();
}



int JCard::GetN(corrType ctype){
  //returns size of TVector
  return GetN(GetKeyWord(ctype));
}



float JCard::Get(TString keyword, int VectorComponent){
  //returns VectorComponent Component of  fValuesVecto`uor TVector for given keyword
  unsigned int index = GetTVectorIndex(keyword);
  if(0<=VectorComponent && VectorComponent<GetN(keyword)){
    return fValuesVector[index][VectorComponent+1];
  }else{
    cout<<"ERROR: fValuesVector index out of range "<<keyword.Data()<<endl;
    exit(1);
  }
}



float JCard::Get(corrType ctype, int VectorComponent){
  //returns VectorComponent Component of  fValuesVecto`uor TVector for given keyword
  return Get(GetKeyWord(ctype), VectorComponent);
}



void JCard::ReadInputCard(){
 
  char buffer[kMaxDimBuffer];

  while(!incard.eof()){ //loop over the input card
   
    incard.getline(buffer,kMaxDimBuffer); //read a line

    if(nentry > 1000){//is the file reasonably long?
      cout<<"Maximum number of 1000 lines reached in JCard.C"<<endl;
      exit(1);
    }
 
    TString tstr(buffer); //convert the line in the buffer to TString

    if( tstr.BeginsWith("#") ) continue;//skipp comments
    tstr.ReplaceAll("\t"," ");//get rid of tabelators
 
   //remove comment in line
    Ssiz_t startOFcomment = tstr.First('#');
    if(startOFcomment>0){
      tstr.Remove(startOFcomment,tstr.Length() - startOFcomment);
    }
   
    //remove white spaces from the begining
    if(tstr.BeginsWith(" ")){
      Ssiz_t startOFkeyword = 0;
      while(1){
        TString s = tstr[startOFkeyword];
        if(s.CompareTo(" ")) break;
        startOFkeyword++;
      }
      tstr.Replace(0,startOFkeyword,"",0);
    }

    //separate inputs 
    TObjArray *lineContents = tstr.Tokenize(" ");

    if(lineContents->GetEntriesFast() < 1) continue;//skipp empty lines
    
    //----- Read a keyword -----
    TString entryname = ((TObjString*)(lineContents->At(0)))->String(); //read a key word

    if(lineContents->GetEntriesFast() == 1){
      cout<<"WARNING: single keyword "<<entryname.Data()<<" on line"<<endl;
    }else{


      //----- Read parameters -----
      vector< float > items;//auxiliary vector
 
      for(int i=1; i<lineContents->GetEntriesFast(); i++){ //loop over the numbers 
        TString token = ((TObjString*)(lineContents->At(i)))->String();//read a string

        if(token.IsFloat()){
          items.push_back(token.Atof());//if string is float number store it to vector
        }else{
  	  cout<<"ERROR: char "<<token.Data()<<" among numbers"<<endl;
	  exit(1);
        }
      }//end of the for loop
 

      //Fill TVectors and Map 
      KeyWordVector.push_back( entryname.Data() );//put the new keyword at the end of the array
 
      fValuesVector.push_back( TVector( 1, items.size(), &items[0]) );//store TVector to array

      MapKeyWordToTVector.insert(pair<TString, unsigned int>(entryname.Data(),KeyWordVector.size()-1)); 
    }//else

    lineContents->~TObjArray();//remove array from heap
    nentry++;
  }//while eof

  return;
}



void JCard::Print(){
  cout<<endl<<"======== "<<cardname<<" ========="<<endl;
  for(unsigned int i=0; i<fValuesVector.size();i++){
    cout<<setw(15)<<KeyWordVector[i];//print keyword
    cout<<" (dim ="<<fValuesVector[i].GetNrows()<<") ";//print size of TVector
    for(int j=1; j<=fValuesVector[i].GetNrows(); j++){
      cout<<fValuesVector[i][j]<<" ";//TVector components
    }
    cout<<endl;
  }
}



void JCard::WriteCard(TFile *file){
  cout<<endl<<"====== Writing into file ========="<<endl;

  file->mkdir("JCard");//directory to store input parameters
  file->cd("JCard");
  for(unsigned int i=0;i<fValuesVector.size();i++){ 
    fValuesVector[i].Write(KeyWordVector[i]);
  }
}



int JCard::GetBin(corrType ctype, float val){

  if(ctype == kNoType) return 0;
 
  int iBin=-1;

  for(int i=0; i<(GetN(ctype)-1); i++)
    if(Get(ctype,i)<=val && val<Get(ctype,i+1)) iBin=i;
 
  return iBin;
}




