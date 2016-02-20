#include "windows.h"
#include <cmath>
#include <iostream>

/* Method 0: Let for loop consume time f, Sleep consume time a(such as 5 ms);
 * Given a CPU utilization cu, then cu = f / (f + a); For example f = a, cu = 50%. 
 * When cu = (b + sin(c)) / d, b > 1, and d > b + 1 > 2, task manage plots sine
 * CPU utilization curve; Then f = a * (b + sin(c)) / (d - b -sin(c)). This 
 * function is a little bizarre, the value b and d should be tuned. */
void mySolution0() {
  const float pi = 3.141592653;
  const int n = 3000;
  const int relative_frequency = 4;

  double ft[n];      // Function table of one period.
  for (int i=0; i<n; ++i) {
    double radian = 2.0 * pi * i / n;
    ft[i] = (3 + sin(radian)) / (3 - sin(radian));
    std::cout << ' ' << ft[i];
  }

  int i = 0;
  while (1) {
    i %= n;
    for (int j=0; j<5000000 * ft[i]; ++j) ;     // About 1 ns per loop.
    Sleep(5);
    i += relative_frequency;
  }
}

/* Method 1: The bizarre function in Method 0 is because of that we make Sleep 
 * time constant. So let busy time conform 1 + sin(x) and idle time conform 
 * 1 - sin(x), then cu = (1 + sin(x)) / 2. To avoid top and bottom fluctuation, 
 * let f = 2 + six(x), a = 2 - six(x), then CPU utilization range will be [1/4, 3/4]. 
 * But the curve tends to be discrete because of the shrinked range. Let f = A + sin(x), 
 * a = B - six(x), range will be [(A-1)/(A+B), (A+1)/(A+B)]; When we want cu 
 * range to be [10%, 100%], then A = 1.22, B = 1. */
void mySolution1() {
  const float pi = 3.141592653;
  const int n = 3000;
  const int relative_frequency = 130;
  const int grain = 100;

  double bt[n], it[n];    // Busy table, idle table.
  for (int i=0; i<n; ++i) {
    double radian = 2.0 * pi * i / n;
    bt[i] = 1.22 + sin(radian);
    it[i] = 1 - sin(radian);
  }

  int i = 0;
  while (1) {
    i %= n;
    auto st = GetTickCount();   // Start time.
    while (1000 * (GetTickCount() - st) < grain * 1000 * bt[i]) ;
    Sleep(grain * it[i]);
    i += relative_frequency;
  }
}

void beautyOfProgrammingSolution() {
  //C++ code to make task manager generate sine graph
  const double SPLIT = 0.01;
  const int COUNT = 200;
  const double PI = 3.14159265;
  const int INTERVAL = 300;

  DWORD busySpan[COUNT]; //array of busy times
  DWORD idleSpan[COUNT]; //array of idle times
  int half = INTERVAL / 2;
  double radian = 0.0;
  for(int i=0;i<COUNT;i++){
    busySpan[i] = (DWORD)(half+(sin(PI*radian)*half));
    idleSpan[i] = INTERVAL - busySpan[i];
    radian += SPLIT;
  }
  DWORD startTime = 0;
  int j = 0;
  while(true){
    j = j%COUNT;
    startTime = GetTickCount();
    while((GetTickCount()-startTime)<=busySpan[j])
      ;
    Sleep(idleSpan[j]);
    j++;
  }
}

int main(int argc, char * argv[]) {
  int idx;    // Left uninitialized.
  if (argc != 1) idx = argv[1][0] - '0';

  if (idx == 0) mySolution0();
  else if (idx == 1) mySolution1();
  else beautyOfProgrammingSolution();

  return 0;
}

