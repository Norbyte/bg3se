////////////////////////////////////////////////////////////////////////////////////////////////////
// NoesisGUI - http://www.noesisengine.com
// Copyright (c) 2013 Noesis Technologies S.L. All Rights Reserved.
////////////////////////////////////////////////////////////////////////////////////////////////////


namespace Noesis
{

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class Compare>
inline bool IsSorted(T* first, T* last, Compare comp)
{
    if(first != last)
    {
        T* current = first;

        for(++current; current != last; first = current, ++current)
        {
            if(comp(*current, *first))
            {
                NS_ASSERT(!comp(*first, *current));
                return false;
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline bool IsSorted(T* first, T* last)
{
    return IsSorted(first, last, [](const T& a, const T& b) { return a < b; });
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class Compare>
inline void InsertionSort(T* first, T* last, Compare comp)
{
    if(first != last)
    {
        T *iCurrent, *iNext, *iSorted = first;

        for(++iSorted; iSorted != last; ++iSorted)
        {
            const T temp(*iSorted);

            iNext = iCurrent = iSorted;

            for(--iCurrent; (iNext != first) && comp(temp, *iCurrent); --iNext, --iCurrent)
            {
                NS_ASSERT(!comp(*iCurrent, temp));
                *iNext = *iCurrent;
            }

            *iNext = temp;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void InsertionSort(T* first, T* last)
{
    InsertionSort(first, last, [](const T& a, const T& b) { return a < b; });
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class Compare>
inline T* GetPartition(T* first, T* last, T pivot, Compare comp)
{
    // Note that pivot is copied on purpose because the collection is being modified

    for(; ; ++first)
    {
        while (comp(*first, pivot))
        {
            NS_ASSERT(!comp(pivot, *first));
            ++first;
        }

        --last;

        while (comp(pivot, *last))
        {
            NS_ASSERT(!comp(*last, pivot));
            --last;
        }

        if (first >= last)
        {
            return first;
        }

        T tmp = *first;
        *first = *last;
        *last = tmp;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class Compare>
inline const T& Median(const T& a, const T& b, const T& c, Compare comp)
{
    return comp(a, b) ? (comp(b, c) ? b : comp(a, c) ? c : a) : comp(a, c) ? a : comp(b, c) ? c : b;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class Compare>
inline void Sort(T* first, T* last, Compare comp)
{
    // This is a hybrid of Quicksort algorithm with Insertion sort and using Tail Call optimization
    while (first < last)
    {
        if (last - first < 20)
        {
            InsertionSort(first, last, comp);
            return;
        }
        else
        {
            // Takes the median of the first, middle, and last elements as the pivot
            const T& pivot = Median(*first, *(first + (last - first) / 2), *(last - 1), comp);
            T* p = GetPartition(first, last, pivot, comp);
            Sort(p, last, comp);
            last = p;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T>
inline void Sort(T* first, T* last)
{
    Sort(first, last, [](const T& a, const T& b) { return a < b; });
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class Val, class Compare>
inline T* LowerBound(T* first, T* last, const Val& value, Compare comp)
{
    uint32_t d = uint32_t(last - first);

    while (d > 0)
    {
        T* i = first;
        uint32_t d2 = d >> 1;

        i += d2;

        if (comp(*i, value))
        {
            first = ++i;
            d -= d2 + 1;
        }
        else
        {
            d = d2;
        }
    }

    return first;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
template<class T, class Val>
inline T* LowerBound(T* first, T* last, const Val& value)
{
    return LowerBound(first, last, value, [](const T& a, const T& b) { return a < b; });
}

}
