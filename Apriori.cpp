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
  supThresh = ((double)threshold/100)*totalBuckets;
  //supThresh = 3;

  //find the total number of frequent items(items that meet support threshold)
  for(int i =0; i < totalItems; i++){
    if(items[i] >= supThresh){
      numFrequent++;
    }
  }

  numFrequent +=1; // item indexes will start at 1 instead of 0
  itemsPuring = new int[numFrequent];
  int temp = 1;

  // take items that meet threshold and renumber them from 1 to m(numFrequent pairs)
  // and than add them too itemsPuring array
  for(int i =0; i < totalItems;i++){
    if(items[i] >= supThresh){
      // save the item count into an array containing all frequent items
      //at next available index
      itemsPuring[temp] = items[i];

      // remember what the original item number was
      // key = original Item number
      // value = new item number(index in itemsPuring array)
      renumbering[i] = temp;
      temp++;
    }
  }

  //delete the array containg counts for all items to free up memory
  delete[] items;


  // lower triangular matrix can have a total of this many
  // non zero elements
  totalNumPairs = ((1+numFrequent)*numFrequent)/2;
  //totalNumPairs +=1;
  //array holding count for pairs of itemsPuring
  //triangularMatrix method used to determine index in array for each pair
  itemPairs = new int[totalNumPairs];

  //set count of each pair to 0 to start with
  for(int i =0; i < totalNumPairs; i++){
    itemPairs[i] = 0;
  }

  //pass 2
  inputFile.open(fileName);

  // go through each bucket
  while(getline(inputFile, line)){
    stringstream ss(line);
    //get all frequent items in bucket
    while(ss >> curItem){
      // if the map containg the original numbers of all frequentItems
      //  contains this number
      if(renumbering.count(curItem) == 1){
        // convert original item number to new item number
        // check to see if this item meets threshold
        //insert the frequent item for this bucket into the
        //frequentItems set to find frequent pairs for this bucket
        frequentItems.insert(renumbering.at(curItem));
      }
    }

    //double for loop to generate all pairs of frequent items
    for(auto i : frequentItems){
      for(auto j: frequentItems){
        if(i != j){  //cant have a pair made up of the same item
          if(i < j){  //lower triangular matrix so i < j
            //increase the pair count for this item by 1
            //get the index item located at in the itemPairs array using the
            // triangularMatrix method
            int k = triangularMatrix(i,j);
            itemPairs[k] +=1;
          }
        }
      }
    }

    frequentItems.clear();

  }
  inputFile.close();

  printPairs();

  delete[] itemsPuring;
  delete[] itemPairs;
}


//used to print the pairs of items that meet
// the support threshold
void Apriori::printPairs(){

 //used to convert array index into triangular matrix
 //in order to figure which 2 items a certain index represents
 //start at row 1 and column 2 in matrix since u cant have an item pair with ur self
  int r = 1;
  int c = r+1;

  //used to convert the item numbers to there original numbers
  int origI = -1;
  int origJ = -1;

  int totalFrequentPairs = 0;
  for(int i = 0; i < totalNumPairs; i++){
    if(itemPairs[i] >= supThresh){
      for(auto original : renumbering){
        if(original.second == c)
          origI = original.first;
        if(original.second == r)
          origJ = original.first;
      }
      cout <<"{" << origJ << ","  << origI << "} -> frequency = " << itemPairs[i]  << endl;
      totalFrequentPairs++;
    }
    if(c == numFrequent){
      r += 1;
      c = r+1;
    }
    else{
      c+=1;
    }

  }
  cout << "total number of frequent pairs = " << totalFrequentPairs << endl;
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
