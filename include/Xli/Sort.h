#ifndef __XLI_SORT_H__
#define __XLI_SORT_H__

#include <Xli/Utils.h>

namespace Xli
{
    /**
        \addtogroup XliCoreUtils
        @{
    */

    template <typename T, typename TComparator> 
    void ShellSort(T* data, int left, int right)
    {
        const float s = 2.8f;

        int size = right - left;
        int increment = size / 2;

        while (increment > 0)
        {
            for (int i = left + increment; i < size; i++)
            {
                int j = i;
                T temp = data[i];

                while ((j >= increment) && TComparator::Compare(temp, data[j - increment]))
                {
                    Swap(data[j], data[j - increment]);
                    j -= increment;
                }

                data[j] = temp;
            }

            if (increment < s && increment > 1)
                increment = 1;
            else 
                increment = (int)((float)increment / s);
        }
    }

    template <typename T, typename TComparator> 
    void Sort(T* data, int left, int right)
    {
        if (right - left > 280)
        {
            int rightIndex = right - 1;
            int pivotIndex = left;

            T pivotValue = data[pivotIndex];
            Swap(data[pivotIndex], data[rightIndex]);

            for (int i = left; i < rightIndex; i++)
            {
                if (TComparator::Compare(data[i], pivotValue))
                {
                    Swap(data[i], data[pivotIndex]);
                    pivotIndex++;
                }
            }

            Swap(data[pivotIndex], data[rightIndex]);

            Sort<T, TComparator>(data, left, pivotIndex);
            Sort<T, TComparator>(data, pivotIndex + 1, right);
        }
        else
        {
            ShellSort<T, TComparator>(data, left, right);
        }
    }

    /** @} */
}

#endif
