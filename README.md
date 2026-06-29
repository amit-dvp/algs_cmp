The program helps to compare two algorithms: Regular Merge Sort vs. Modified Merge Sort.

    1. Regular Merge Sort: 
        recursive division of array into 
        two subarrays every time, up to subarrays for trivial
        the sort, and merge each two already sorted subarrays 
        into one sorted subarray, up to sorting the whole array.
        Features of merge sort algorithm: 
            Merge part needs additional arrays to store merged data
            Merge part worst case time: T(n)=teta(n)
            There are log(n) stages of merge
            Total worst case time :T(n) = teta(n*log(n))
            Total worst case time (additional way) : T(n)=2T(n/2) + teta(n) => T(n)=teta(n*log(n))

    2. Modified Merge Sort 
        also divides the array recursievely 
        into subarrays, but stops when the size of subarrays 
        is less or equal to parameter k,  
        at this stage it uses Insertion Sort to sort each subarray, 
        and after it performs regular merge recuresively to get whole sorted array.
        
        It divides the given array into subarrays of about size k , 
        the number of such subarrays is about n/k ,
        It performs Insertion Sort for each subarray of size k.
        When size of each subarray is k=1 the subarray becomes trivial for sorting, 
        and actually the performance of modified merge sort is equal to regular merge sort.   
        Features of modofied merge sort algorithm:
            Merge part still needs additional arrays to store merged data
            But it has less merges, because they are stoped when subarrays size
            reaches k value. 
            Total worst case time : 
            - Sort n/k subarrays with insertion sort : T(n)=teta(n*k)
            - Each merge runs teta(n) in worst case
            - There are log(n/k) stages in merge of n/k subarrays
            - Total merge time of n/k subarrays:  T(n) = teta(n*log(n/k))
            - Total worst case time of modified merge sort : T(n)=teta(n*k + n*log(n/k))  

  According to modified algorithm implementation, we expect the following behaviour :
            
            when the size of subarrays is k = 1: 
                the array will be actually divided to minimal parts 
                without (with trivial) sort, so no Insertion Sort will be actually done and this 
                case is identical to Merge Sort case.  
            
            when the size of subarrays is k = n (where n is whole array's size):
                only Insertion Sort will be formed on entire array, 
                without any divide and merge.

            when the size of subarrays is k >= n/2 the algorithm firstly will start with 
                array of size n,
                because n > k , after it algorithm will divide array into 2 parts
                each about n/2 and because n/2 <= k it will always perform 
                Insertion Sort for two parts.
                So all cases of k >=n/2 will return the same results.
                
            when the size of subarrays is 1<k<n/2 the algorith will spend one part of time 
               in performing swaps of Insertion Sort and another part of the time 
               in comparisons of merges.
               As k is bigger , less merges will be done and more Insertion sorts 
               will be performed. So we expect for some optimum this two time parts.

Also, the program will run merge sort and modified merge sort (with insertion sort) on the 
same arrays of several types : 
    
    - random array
    
    - array arranged in ascending (rising) order, from min element to max element.
        We will generate it using merge sort of random array.
    
    - array arranged in descending order, from max element to min element. 
        We will generate it using merge sort of random array,
        but with merge arranged in descending order.
            
    - array with many duplicates : random numbers generation from 1 to log(n)
    
    
    - K-sorterd array. 
       According to Copilot AI: a k-sorted array is one where each element is at most 
       k positions away from its correct position in a fully sorted array. 
       We will prepare K - sorted array using quick sort algorithm with stop recursion condition 
       when each subarray length will be less or equal to k. 
       This will garantee that all the elements between pivot values that are in correct position
       will be in almost correct position up to k places.    


The program attempts to measure exact elapsed time in ns resolution, 
using POSIX(Linux) system commands, so it can not be pure ANSI standard program. 
The attempt was done to get at least GNU standard program. 

The elapsed time is calculated by performing average of several measurements, 
(average sample size can be changed , it is by default 10), because the operation system
inserts fluctuations in running time from one run to another run.

Also we count number of comparisons and number of swaps (in case of regular merge sort - 
comparisons only), to allow check if elapsed time measurements are consistent. 

We check following values of k, depending on n :

       1. Always present times for k = 1 and k = n.
       2. Present times for k>=2 and k<n/2 with resolution of k : k = k * 2, starting from k = 2.
       3. For k => n/2 and k<n try to present 3 values (we expect the same result) , if it is possible (use step k_val_step_2).     
