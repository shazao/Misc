/* Tower of Hanoi problem, using Recursive Algorithm */

#include <iostream>

// Move n disks from Rod From to Rod To.
unsigned int Hanoi(unsigned int n, char from, char to)  
{
    unsigned int count = 0;   // Total moves count.
    if(n == 1){
        std::cout << from << "->" << to << std::endl;  ++count;
        return count;
    }else{
        count += Hanoi(n-1, from, 'C'-from+'C'-to+'A'-1); // The third rod: 'A'+'B'+'C'-from-to.
        std::cout << from << "->" << to << std::endl;  ++count;
        count += Hanoi(n-1, 'C'-from+'C'-to+'A'-1, from);
        return count;
    }
}

int main()
{
    std::cout << "Please input the number of disks in the Tower of Hanoi problem:" << std::endl;
    unsigned int n;
    std::cin >> n;
    std::cout << "The sequence of moving the disks from Rod A:" << std::endl;
    std::cout << "Total " << Hanoi(n, 'A', 'C') << " moves." << std::endl;

    return 0;
}
