/*
* RandomSample
* Similar too Apriori except its run for randomly selected sample data
* percentOfData = 1 <- in header file, change this to select a different percentage of data to use as sample data (1%-100%)
*/

#include "RandomSample.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include<time.h>


RandomSample::RandomSample(){
  getSampleData();
  findFrequent();
  printFrequentPairs();
}

/**
 * Go through the file and get a random sample of data(buckets)
 * store that sample data into memory
 *
 * need to do 2 passes over the file containing the items:
 * 1st pass to get total number of itemsets
 *      generate specified number of random numbers bettween 0 and the total bucketss
 * 2nd pass pick the bucket if it appears in the list of random numbers
 *
 */
void RandomSample::getSampleData(){

  string line;

  //1st pass
  //get total number of data(buckets)
  inputFile.open(fileName);
  while(getline(inputFile, line)){
    totalBuckets++; // get the number of lines(buckets)
  }

  numRandBuckets = ((double)percentOfData/100)*totalBuckets;  // number of random buckets to read

  inputFile.close();

  set<int> ranNumbers;

  // generate a set of random numbers that are automaticly stored in order into the set ranNumbres
  // these numbers represent the bucket selected for sample data
  getRandomNumbers(&ranNumbers);


  inputFile.open(fileName);
  int curBucket = 0;              //bucket currently being read
  while(getline(inputFile, line)){
    if(ranNumbers.count(curBucket) == 1){       // if the bucket currently being read is one of the randomly selected buckets
      sampleData.insert(line);                  // add it to sample data
    }
    curBucket++;
  }

  inputFile.close();

}

/**
 * generate a set of random numbers between 0 and the total number of buckets in dataset
 * @param ranNumbers refference to the set to store the unique numbers in
 */
void RandomSample::getRandomNumbers(set<int> *ranNumbers){
  srand(time(NULL));
  bool unique = false;
  int num = rand()%totalBuckets;  // generate a random number between 0 and total number of buckets
  ranNumbers->insert(num);         // add the first random number to the list of selected random numbers
  for(int i = 1; i < numRandBuckets; i++){
    num = rand()%totalBuckets;
    while(ranNumbers->count(num) == 1){          // keep generating numbers till a unique random number is generated
      num = rand()%totalBuckets;
    }
    ranNumbers->insert(num);                 // add the unique random number to the list
  }
}


/**
 * Apriori for sample data
 */
void RandomSample::findFrequent(){

  //pass 1
  int curItem;
  string line;
  for(auto i : sampleData){
    stringstream ss(i);
    // split line by spaces to get each item number
    while(ss >> curItem){
      if(items.count(curItem) == 1){   // if item already exists in the map
        items[curItem] +=1;            // incese its count by 1
      }else{
        items[curItem] = 0;
      }
    }
  }

  //in between pass
  double scaleThreshold = ((double)percentOfData/100) * threshold;          // reduce threshold to match amount of data selected
  supThresh = ((double)scaleThreshold/100)*totalBuckets;      // amount of buckets item must appear in to meet threshold

  // go through all items
  // and remove those that dont meet support threshold
  map<int,int>::iterator it;
  for(auto it = items.begin(); it != items.end();){
    if(it->second < supThresh){                   // item count doesnt meet threshold
      it = items.erase(it);                              // remove the element
    }else{
      ++it;
    }
  }

  //pass 2
  set<int> fItemsBucket;  // will hold the items that appear frequently in a bucket
  curItem = 0;
  for(auto i : sampleData){
    stringstream ss(i);
    //get all frequent items in bucket
    while(ss >> curItem){
      // if the map containg the frequent items
      //  contains this item
      if(items.count(curItem) == 1){
        fItemsBucket.insert(curItem);
      }
    }

    //double for loop to generate all pairs of frequent items
    for(auto i : fItemsBucket){
      for(auto j: fItemsBucket){
        if(i != j){  //cant have a pair made up of the same item
          if(i < j){
            string key = to_string(i) + "," + to_string(j);
            if(itemPairs.count(key) == 1){ // if this item pair alrady in item pairs map
              itemPairs[key] +=1;          // increase its count by 1
            }else{
              itemPairs[key] = 1;          // else add it to the map
            }
          }
        }
      }
    }
     // once you are done finding all pottential frequent pairs in a bucket clear it
     // so the next bucket can use it
    fItemsBucket.clear();
  }
}

void RandomSample::printFrequentPairs(){
  int totalFrequentPairs = 0;

  map<int,int>::iterator it;
  for(auto it = itemPairs.begin(); it != itemPairs.end();++it){
    if(it->second >= supThresh){                   // item pair count meets threshold
      cout << "{" << it->first << "}  -> frequency = " << it->second << endl;
      totalFrequentPairs++;
    }
  }
  cout << "total number of frequent pairs = " << totalFrequentPairs << endl;
}

int main(int argc, char const *argv[]){
  RandomSample RandomSample;
  return 0;
}
