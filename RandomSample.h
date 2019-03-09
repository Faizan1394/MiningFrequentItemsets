#ifndef RANDOMSAMPLE_H
#define RANDOMSAMPLE_H

#include <set>
#include <map>
#include <fstream>

#define threshold 3      // threshold percent
#define percentOfData 30 // percent of data to use as sample data

using namespace std;
class RandomSample{

public:
  RandomSample();
private:

  set<string> sampleData;
  map<int,int> items;
  map<string,int> itemPairs;

  int numRandBuckets = 0;
  int supThresh = 0;
  int totalBuckets = 0;


  string fileName = "retail.data";
  ifstream inputFile;
  void printPairs();

  void getRandomNumbers(set<int> *ranNumbers);
  void findFrequent();
  void getSampleData();
  void printFrequentPairs();

};

#endif
