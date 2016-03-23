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
// Heavily optimize the Prime Number search.
// With -O0 is pretty slow because of set implementation
// With -O3, despite managing the prime list, it start getting faster

#include <iostream>
#include <vector>
#include <set>
#include <cmath>

using namespace std;

void getRotations(int i, vector<int>& r) {
  const int rad = 10;
  if (i >= rad) {
    vector<int> p;
    while (i > 0) {
      p.push_back(i % rad);
      i /= rad;
    }

    for (int i = 1; i < p.size(); ++i) {
      int multi = 1;
      int r2 = 0;

      for (int j = i; j < i + p.size(); ++j) {
        r2 += p[j % p.size()] * multi;
        multi *= rad;
      }

      r.push_back(r2);
    }
  }
}

bool isPrime(int p, set<int> *primes, int *max_t) {
  bool prime = true;
  int stop = sqrt(p);
  if (p < 2)
    return false;
  // Not very useful to optimize for the case of 2
  else if (primes->count(p) != 0)
    return true;
  // Optimize for number that are divisible by 2
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
  if (n > 2) {
    primes.insert(2);
    primes_circular.push_back(2);
    max_t = 2;
  }

  for (int pc = 3; pc <= n; ++++pc) {
    vector<int> r;
    if (isPrime(pc, &primes, &max_t)) {
      getRotations(pc, r);
      bool cp = true;
      for (int i = 0; i < r.size(); ++i) {
        if (!isPrime(r[i], &primes, &max_t)) {
          cp = false;
          break;
        }
      }

      if (cp) {
        primes_circular.push_back(pc);
      }
    }
  }

  cout << primes_circular.size() << endl;
  // for (auto test: primes_circular)
  //   cout << test << " ";
  // cout <<endl;

  return 0;
}
