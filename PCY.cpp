#include "PCY.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>

void PCY::findFrequent(){
  inputFile.open(fileName);

  string line;
  int curItem;

  //go over all items to get total number of items for more
  //space efficiency
  while(getline(inputFile,line)){
    totalBuckets++;
    stringstream ss(line);
    //since items already converted to int
    //we can just find the item with the highest number to get total num items

    while(ss >> curItem){
      if(curItem > totalItems){
        totalItems = curItem;
      }
    }
  }
  inputFile.close();
  totalItems++; // item numbers start at 0 so increase by 1 to get total items
  items = new int[totalItems]; // used to hold all items

   supThresh = ((double)threshold/100)*totalBuckets;
   //supThresh = 3;

  itemPairs = new int[totalItems]; // represents hash table for each pair

  //initialize all element counts to 0 to start with
  for(int i = 0; i < totalItems; i++){
    items[i] = 0;
    itemPairs[i] = 0;
  }
  inputFile.open(fileName);



  //***pass 1***
  //go through the file like by line
  while(getline(inputFile, line)){
    stringstream ss(line);
    // split line by spaces to get each item number
    while(ss >> curItem){
      // at the index corresponding to the item number incream value by1
      // this keeps track of the number of times that item occurs
      items[curItem] +=1;
      itemsInBucket.insert(curItem);  // using this to temp. hold all items in the bucket instead of just frequent
    }

      //find pairs in bucket
      for(auto i : itemsInBucket){
        for(auto j: itemsInBucket){
          if(i != j){  //cant have a pair made up of the same item
            if(i < j){ //order doesnt matter (2,3) is the same as (3,2) so dont repeat
              int h = hashFunction(i,j);  // get hash value for pair
              itemPairs[h] += 1;          // increment the value at bucket(index) in array with this hash value by 1
            }
          }
        }
      }
      itemsInBucket.clear();
  }

    inputFile.close();

  //in between pass
  //bool vectors in c++ are made in such a way that
  //each element occupies a single bit
  vector<bool> vb;
  for(int i =0; i < totalItems; i++){
    if(itemPairs[i] >= supThresh){
      vb.push_back(true);
    }else{
      vb.push_back(false);
    }
  }


  //***pass 2***

  inputFile.open(fileName);
  //go through the file like by line
  while(getline(inputFile, line)){
    stringstream ss(line);
    // split line by spaces to get each item number
    while(ss >> curItem){
      itemsInBucket.insert(curItem);  // hold all items in the bucket
    }

      //find pairs in bucket
      for(auto i : itemsInBucket){
        for(auto j: itemsInBucket){
          if(i != j){  //cant have a pair made up of the same item
            if(i < j){ //order doesnt matter (2,3) is the same as (3,2) so dont repeat
              int h = hashFunction(i,j);  // get hash value for pair
              if(vb[h] == true){
                if(items[i] >= supThresh  && items[j] >= supThresh){
                  string pair = to_string(i) + "," + to_string(j);
                  //cout << pair << endl;
                  if(frequentPairs.count(pair)){
                    frequentPairs[pair] += 1;
                  }else{
                    frequentPairs[pair] = 1;
                  }
                }
              }
            }
          }
        }
      }
      itemsInBucket.clear();
  }
    inputFile.close();

    int totalFrequentPairs = 0;
  for(auto i : frequentPairs){
    if(i.second >= supThresh){
      cout << "{" << i.first << "}" << "-> frequency = " << i.second   << endl;
      totalFrequentPairs++;
    }
  }
cout << "total number of frequent pairs = " << totalFrequentPairs << endl;


delete[] itemPairs;
delete[] items;
}


//used to print the pairs of items that meet
// the support threshold
void PCY::printPairs(){
}

int PCY::hashFunction(int i, int j){
  int h = (i+j)%totalItems;
  return h;
}

int main(int argc, char const *argv[]){
  PCY pcy;
  pcy.findFrequent();
  return 0;
}
