/* Packing problem, using Greedy Algorithm
 * : Put n object into m boxes, 0<V(i)<=V(box), minimize m*/

#include <iostream>

int main()
{
	int object_volumes[] = {60, 45, 35, 20, 20, 20};  // Ordered.
	int boxes[sizeof(object_volumes)/sizeof(object_volumes[0])] = {0};
	int box_volume = 100;

  // Algorithm begins.
	for(int i=0; i<sizeof(object_volumes)/sizeof(object_volumes[0]); ++i){
		for(int j=0; j<sizeof(boxes)/sizeof(boxes[0]); ++j){
			if(boxes[j]+object_volumes[i]>box_volume)
				continue;
			else{
				boxes[j] += object_volumes[i];
				break;
			}
		}
	}

  // Print it.
	for(int j=0; j<sizeof(boxes)/sizeof(boxes[0]); ++j){
		if(boxes[j] != 0)
			std::cout << j << ": " << boxes[j] << std::endl;
		else
			break;
	}

	return 0;
}
