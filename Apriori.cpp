#include "Apriori.h"
#include <string>
#include <iostream>
#include <sstream>

void Apriori::findFrequent(){
  inputFile.open(fileName);

  string line;
  int curItem;

  //do 1 pass over all items to get total number of items for more
  //space efficiency
  while(inputFile >> curItem){
    //since items already converted to int
    //we can just find the item with the highest number to get total num items
    if(curItem > totalItems){
      totalItems = curItem;
    }
  }
  inputFile.close();

  totalItems++; // item numbers start at 0 so increase by 1 to get total items
  items = new int[totalItems];

  //initialize all element counts to 0 to start with
  for(int i =0; i < totalItems; i++){
    items[i] = 0;
  }

  inputFile.open(fileName);

  //pass 1
  //go through the file like by line
  while(getline(inputFile, line)){
    totalBuckets++; // get the number of lines(buckets)
    stringstream ss(line);
    // split line by spaces to get each item number
    while(ss >> curItem){
      // at the index corresponding to the item number incream value by1
      // this keeps track of the number of times that item occurs
      items[curItem] +=1;
    }
  }

  inputFile.close();


  //in between pass
  //supThresh = ((double)threshold/100)*totalBuckets;
  supThresh = 3;

  for(int i =0; i < totalItems; i++){
    if(items[i] >= supThresh){
      numFrequent++;
    }
  }

  cout << numFrequent << endl;

  // itemsPuring = new int[numFrequent + 1];  // item indexes will start at 1 instead of 0
  // int temp = 1;
  // for(int i =0; i < totalItems;i++){
  //   if(items[i] >= supThresh){
  //     // save the item count into an array containing all frequent items
  //     //at next available index
  //     itemsPuring[temp] = items[i];
  //
  //     // remember what the original item number was
  //     // key = original Item number
  //     // value = new item number(index in itemsPuring array)
  //     renumbering[i] = temp;
  //     temp++;
  //   }
  // }
  //
  // //delete the array containg counts for all items to free up memory
  // delete[] items;
  //
  //
  // // lower triangular matrix can have a total of this many
  // // non zero elements
  // totalNumPairs = ((1+numFrequent)*numFrequent)/2;
  // itemPairs = new int[totalNumPairs];
  //
  // //set count of each pair to 0 to start with
  // for(int i =0; i < totalNumPairs; i++){
  //   itemPairs[i] = 0;
  // }
  //
  // //pass 2
  // inputFile.open(fileName);
  // // go through each bucket
  // while(getline(inputFile, line)){
  //   stringstream ss(line);
  //   //get all frequent items in bucket
  //   while(ss >> curItem){
  //
  //     // if the map containg the original numbers of all frequentItems contains
  //     // this number
  //     if(renumbering.count(curItem)){
  //       if(itemsPuring[renumbering.at(curItem)] >= supThresh){
  //         //insert the new number of the frequent item for this bucket into the
  //         //frequentItems set to find frequent pairs for this bucket
  //         frequentItems.insert(renumbering.at(curItem));
  //       }
  //     }
  //   }
  //
  //   for(auto i : frequentItems){
  //     for(auto j: frequentItems){
  //       if(i != j){
  //         if(i < j){
  //           itemPairs[triangularMatrix(i,j)] +=1;
  //         }
  //       }
  //     }
  //   }
  //
  // }
  // inputFile.close();
  //
  // printPairs();
  //
  // delete[] itemsPuring;
  // delete[] itemPairs;
}

void Apriori::printPairs(){
  int r =1;
  int c = 1;
  int increment = 1;

  int origI = -1;
  int origJ = -1;
  for(int i = 1; i < totalNumPairs; i++){
    if(itemPairs[i] >= supThresh){
      for(auto original : renumbering){
        if(original.second == c)
          origI = original.first;
        if(original.second == increment)
          origJ = original.first;
      }
      cout << "{" << origI << ","  << origJ << "} -> frequency = " << itemPairs[i]  << endl;
    }
    if(increment == r){
      increment = 1;
      r += 1;
      c +=1;
    }
    else{
      increment++;
    }
  }
}



int Apriori::triangularMatrix(int i, int j){
  int k = (i-1)*(numFrequent - (i/2)) + j - i;
  return k;
}

int main(int argc, char const *argv[]){
  Apriori apriori;
  apriori.findFrequent();
  return 0;
}
