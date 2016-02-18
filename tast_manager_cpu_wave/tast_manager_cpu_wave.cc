#include "windows.h"
#include <cmath>
#include <iostream>

void mySolution() {
  const float pi = 3.141592653;
  const int N = 3000;
  const int relative_frequency = 4;

  double ft[N];      // Function table of one period.
  for (int i=0; i<N; ++i) {
    double radian = 2.0f * pi * i / N;
    ft[i] = (3.5 + sin(radian)) / (4 - sin(radian));
    std::cout << ' ' << ft[i];
  }

  int i = 0;
  while (1) {
    i %= N;
    for (int j=0; j<8000000 * ft[i]; ++j) ;     // Intel T5870 @ 2GHz, 1.25 ns per loop.
    Sleep(10);
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
  int idx = 0;
  if (argc != 1) idx = argv[1][0] - '0';
  if (idx) beautyOfProgrammingSolution();
  else mySolution();
  return 0;
}

