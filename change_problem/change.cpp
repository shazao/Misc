/* Change problem, using Greedy Algorithm
 * : Given change, find the optimal change scheme*/

#include <iostream>
#include <iomanip>
#include <cmath>

int main()
{
  int moneys[] = {1000, 500, 200, 100, 50, 20, 10, 5, 2, 1}; // Unit: 10 cents(1 Mao)
  int money_count[sizeof(moneys)/sizeof(moneys[0])] = {0};
  std::cout << "Please input your change(unit: RMB Yuan):" << std::endl;
  int change;
  float chg;
  std::cin >> chg;
  change = roundf(chg*10);
  
  // Algorithm goes here.
  while(change >= 1){
    for(int i=0; i<sizeof(moneys)/sizeof(moneys[0]); ++i){
      if(change >= moneys[i]){      // Note: '>='
        change -= moneys[i];
        money_count[i]++;
        break;
      }
    }
  }

  // Print it.
  std::cout << "When 100, 50, 20, 10, 5, 2, 1, 0.5, 0.2, 0.1 Yuan are avaible, " << std::endl;
  std::cout << "the change scheme is(Unit: RMB Yuan):" << std::endl;
  std::cout << std::left;     // Left-justify
  for(int i=0; i<sizeof(moneys)/sizeof(moneys[0]); ++i)
    if(money_count[i])
      std::cout << std::setw(4) << moneys[i]/10.0 << ": " << money_count[i] << std::endl;

  return 0;
}
