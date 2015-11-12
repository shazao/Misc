#include <iostream>
#include <cstdio>
#include "..\Profiler.h"

int x[8];
int solution_count;

// Can ith queen be placed at Column j?
bool canPlace(int i, int j) {
  for (int idx=0; idx<i; ++idx)
    if (x[idx]==j || x[idx]-j==idx-i || x[idx]-j==i-idx)
      return false;
  return true;
}

// The iterative method shows the "backtracking" clearly by "-- idx".
void eightQueens() {
  int idx = 0;
  while (idx >= 0) {
    if (x[idx] == 8) {
      x[idx] = 0;
      -- idx;
      if (idx >= 0)
        ++ x[idx];
      continue;
    }
    while (x[idx]<7 && !canPlace(idx, x[idx]))
      ++ x[idx];
    if (x[idx]==7 && !canPlace(idx, x[idx])) {
      x[idx] = 0;
      -- idx;
      ++ x[idx];
    } else if (idx == 7) {
      ++ solution_count;
      std::cout << "Solution " << solution_count << ": ";
      for (int i=0; i<8; ++i)
        std::cout << ' ' << x[i];
      std::cout << std::endl;
      //return;
      ++ x[idx];
    } else {
      ++ idx;
    }
  }
}

// A more neat iterative solution.
bool place(int x[], int k)//考察皇后k放置在x[k]列是否发生冲突
{
    int i;
    for(i=1;i<k;i++)
        if(x[k]==x[i] || k-i==x[k]-x[i] || k-i==x[i]-x[k])
            return false;
    return true;
}

void queen(int n)
{
    int x[100];
    int i,k;
    for(i=1;i<=n;i++)
        x[i]=0;
    k=1;
    while(k>=1)
    {
        x[k]=x[k]+1;   //在下一列放置第k个皇后
        while(x[k]<=n&&!place(x, k))
            x[k]=x[k]+1;//搜索下一列
        if(x[k]<=n&&k==n)//得到一个输出
        {
            ++ solution_count;
            std::cout << "Solution " << solution_count << ": ";
            for(i=1;i<=n;i++)
                printf("%d ",x[i]);
            printf("\n");
        //return;//若return则只求出其中一种解，若不return则可以继续回溯，求出全部的可能的解
        }
        else if(x[k]<=n&&k<n)
            k=k+1;//放置下一个皇后
        else
        {
            x[k]=0;//重置x[k],回溯
            k=k-1;
        }
    }
}

void printSolution() {
  std::cout << "Solution " << solution_count << ": ";
  for (int i=0; i<8; ++i)
    std::cout << ' ' << x[i];
  std::cout << std::endl;
}

// The recursive method shows the "backtracking" implicitly by return to the caller after the for loop.
void eightQueensRecursively(int i) {
  if (i == 8)
    return;
  for (int j=0; j<8; ++j) {
    x[i] = j;
    if (canPlace(i, j)) {
      if (i == 7) {
        ++ solution_count;
        printSolution();
      } else
        eightQueensRecursively(i+1);
    }
  }
}

int main() {

  {
    Profiler perf;
    eightQueens();
  }

  x[8] = { 0 };
  solution_count = 0;

  {
    Profiler perf;
    queen(8);
  }

  x[8] = { 0 };
  solution_count = 0;

  {
    Profiler perf;
    eightQueensRecursively(0);
  }

  return 0;
}
