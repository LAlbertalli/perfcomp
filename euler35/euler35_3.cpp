//
// (C) Aleksey V Malyshev
// aleksey.malyshev@mail.ru
//
// C++ solution for Project Euler problem #35
// 
// Type to compile:
// 
// -- no optimization:
// g++ euler35.cpp -o euler35
// 
// -- full optimization:
// g++ euler35.cpp -o euler35 -O3
// 
// To run type:
// time ./euler35 < euler35_in.txt
//

// Modified by Luca Albertalli
// l.albertalli@gmail.com
//
// Same as euler35_2.cpp but optimize the rotations:
// - Use a statically allocated vector instead of vector<int>
// - Optimize checks
// - Optimize isPrime for the new usage pattern

#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <stdlib.h>

using namespace std;

class Rotators{
  public:
    Rotators(unsigned int max);
    ~Rotators();
    int getRotations(int i);

    static const unsigned int RADIX = 10;
    int *rotations;
  private:
    unsigned int max_len;
    int *support;
};

Rotators::Rotators(unsigned int max){
  max_len = 0;
  while (max>0){
    max_len++;
    max /= RADIX;
  }
  if (max_len==0){
    rotations = NULL;
    support = NULL;
  }
  else{
    rotations = (int*)malloc(sizeof(int)*max_len);
    support = (int*)malloc(sizeof(int)*max_len);
  }
}

Rotators::~Rotators(){
  if (rotations!=NULL)
    free(rotations);
  if (support!=NULL)
    free(support);
}

int Rotators::getRotations(int i) {
  int cnt = 0;
  int skpcnt = 0;
  rotations[0] = i;
  if (i >= RADIX) {
    while (i > 0) {
      support[cnt] = i % RADIX;
      i /= RADIX;
      cnt++;
    }

    for (int i = 1; i<cnt; ++i){
      int multi = 1;
      int r2 = 0;
      bool skp = false;
      for (int j = i; j< i+cnt; ++j){
        r2 += support[j%cnt] * multi;
        multi *= RADIX;
      }
      // Skip the return value if already inserted
      if (r2 < rotations[0])
        return -1;
      for (int j = 0; j < i; j++)
        if (rotations[j] == r2){
          skpcnt++;
          skp = true;
          break;
        }
      if (not skp)
        rotations[i] = r2;
    }
    return cnt - skpcnt;
  }
  else
    return 1;
}

bool isPrime(int p, set<int> *primes, int *max_t) {
  bool prime = true;
  int stop = sqrt(p);
  if (p < 2)
    return false;
  // Due to the optimization in generating the rotations, we no more need to optimize with a set lookup.
  // Technically speaking it still useful because of the "look ahead" due to rotations, but it is minor
  // Note: We still use set because they are ordered.
  //else if (primes->count(p) != 0)
  //  return true;

  // Optimize for number that are divisible by 2
  else if(p==2)
    return true;
  else if ((p & 1) == 0)
    return false;
  else{
    if (*max_t < stop) {
    // In theory is useless, but optimizer should take care of it
      for (int i = (*max_t)+1; i<= stop; i++)
        isPrime(i, primes,&i);
      *max_t = stop;
    }
    for (auto test: *primes)
    {
      // Optimize for number that are divisible by 2
      if (test == 2)
        continue;
      if (test > stop)
        break;
      if (p%test == 0){
        prime = false;
        break;
      }
    }
  }
  if (prime){
    primes->insert(p);
  }
  return prime;
}

int main() {
  int n;
  cin >> n;

  vector<int> primes_circular;
  set<int> primes;
  int max_t = 0;
  Rotators r = Rotators(n);

  if (n > 2) {
    primes.insert(2);
    primes_circular.push_back(2);
    max_t = 2;
  }

  // Divisible by two is already optimized here!!!
  for (int pc = 3; pc <= n; ++++pc) {
    if (isPrime(pc, &primes, &max_t)) {
      int cnt = r.getRotations(pc);
      if (cnt > 0){
        bool cp = true;
        for (int i = 1; i < cnt; ++i) {
          if (!isPrime(r.rotations[i], &primes, &max_t)) {
            cp = false;
            break;
          }
        }

        if (cp) {
          for (int i = 0; i< cnt; ++i)
            primes_circular.push_back(r.rotations[i]);
        }
      }
    }
  }

  // for (auto test: primes_circular)
  //   cout << test << " ";
  // cout <<endl;
  cout << primes_circular.size() << endl;

  return 0;
}
