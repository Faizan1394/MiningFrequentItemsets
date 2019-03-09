#ifndef PCY_H
#define PCY_H

#include <set>
#include <map>
#include <fstream>
#define threshold 30

using namespace std;
class PCY{

public:
  void findFrequent();

private:
  int *items = nullptr;
  int *itemPairs = nullptr;

  map<string,int> frequentPairs;

  set<int> itemsInBucket;

  int totalNumPairs = 0;

  int supThresh = 0;
  int totalItems = 0;
  int totalBuckets = 0;

  string fileName = "netflix.data";
  ifstream inputFile;


  int triangularMatrix(int i, int j);
  int hashFunction(int i, int j);
  void printPairs();

};

#endif
