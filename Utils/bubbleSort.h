#pragma once
#include <vector>

template<typename T>
void BubbleSort(std::vector<T>& vec)
{
    for(size_t i = 0; i < vec.size(); ++i)
    {
        bool swapped = false;
        for(size_t j = 0; j < vec.size() - i - 1; ++j)
        {
            if(vec[j] > vec[j+1])
            {
                swapped = true;
                std::swap(vec[j], vec[j+1]);
            }
        }
        if(!swapped) return;
    }
}

// template<typename T>
// void InsertionSort(std::vector<T>& vec);

// template<typename T>
// void CoctailSort(std::vector<T>& vec);

// template<typename T>
// void QuickSort(std::vector<T>& vec);

// template<typename T>
// void CountingSort(std::vector<T>& vec);

// template<typename T>
// void SelectionSort(std::vector<T>& vec);
