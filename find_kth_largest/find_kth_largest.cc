#include <iostream>
#include <vector>
#include <cstdlib>
#include <cassert>

int partition(std::vector<int> & iv, int l, int r) {
  int pivot = l + rand()%(r-l);
  std::swap(iv[l], iv[pivot]);
  int larger_idx = l;
  for (int i = l + 1; i < r; ++i)
    if (iv[i] > iv[l]) {
      ++ larger_idx;
      std::swap(iv[larger_idx], iv[i]);
    }
  std::swap(iv[l], iv[larger_idx]);
  return larger_idx;
}

int findKthLargest(std::vector<int> & iv, int l, int r, int k) {
  assert(k <= r - l);
  int pivot = partition(iv, l, r);
  int n_larger = pivot - l;
  if (n_larger + 1 == k)
    return iv[pivot];
  else if (n_larger + 1 > k)
    return findKthLargest(iv, l, pivot, k);
  else
    return findKthLargest(iv, pivot+1, r, k - (n_larger + 1));
}

int main() {
  // Get an array.
  std::cout << "Please input the array: ";
  std::vector<int> iv;
  int i = 0;
  while (std::cin >> i)
    iv.push_back(i);
  std::cout << "The array you input is: ";
  for (auto itr=iv.begin(); itr!=iv.end(); ++itr)
    std::cout << ' ' << *itr;
  std::cout << std::endl;

  for (size_t i = 0; i < iv.size(); ++i)
    std::cout << "The " << i + 1 << "-th largest: " << findKthLargest(iv, 0, iv.size(), i+1) << std::endl;

  return 0;
}
