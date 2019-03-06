#ifndef APRIORI_H
#define APRIORI_H

#include <set>
#include <map>
#include <fstream>
#define threshold 1

using namespace std;
class Apriori{

public:
  void findFrequent();

private:
  int *items = nullptr;
  int* itemsPuring = nullptr;
  int *itemPairs = nullptr;

  map<int,int> renumbering;
  set<int> frequentItems;

  int totalNumPairs = 0;

  int supThresh = 0;
  int totalItems = 0;
  int totalBuckets = 0;
  int numFrequent = 0;

  string fileName = "test.data";
  ifstream inputFile;


  int triangularMatrix(int i, int j);
  void printPairs();

};

#endif
