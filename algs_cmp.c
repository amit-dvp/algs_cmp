/*

This is the C++ code based on Q21 in C.L.R.S. 
Run it in the internet compiler as C++ code.
Author : A 


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

*/


# define _POSIX_C_SOURCE 199309L 

# include <limits.h>    /* to use max int value INT_MAX as sentinel */
# include <stdlib.h>    /* NULL definition, rand and srand functions */
# include <stdio.h>     /* for printf */
# include <math.h>      /* for floor , log2 */
# include <time.h>      /* for time seed for random number, clock_t */
# include <sys/types.h> /* clockid_t for Linux */

# define nSEC_IN_1_uSEC = 1000 /* 1000 nanoseconds in 1 uSec*/

# define DEBUG 0  /* set to 0 to disable debug macros */

# define NS_TO_MS 1000000     /* nsec in 1 msec */
# define NS_TO_US 1000        /* nsec in 1 usec*/
# define NS_TO_SEC 1000000000 /* nsec in sec */

# define AVERAGE_SAMPLE_SIZE 10 /* sample size for calculation time average */

/* macro for debug code wrapping to disable all debug macros simultaneously */
# if DEBUG
    # define debug_only_code(text) text
# else
    # define debug_only_code(text)     
# endif    


/* struct definition */

/* define struct to hold comparisons and swaps of 
   modified merge insertion sort
*/
typedef struct comps_and_swaps {

    unsigned long long int comps_num;
    unsigned long long int swaps_num;
} t_comps_and_swaps;


/* global variable for random seed*/
int rand_seed;



/* functions prototypes */

/*
   Function merge:
        Part of merge sort algorithm.
        Merges already sorted subarrays arr[p..q] (left subarray) 
        and arr[q+1..r] (right subarray) within array arr.
        Merge part worst case time: T(n)=teta(n) 

        The function returns number of comparisons.
*/
unsigned long long int merge (int * arr, int p, int q, int r);

/* 
  Dynamic allocation of memory for array. 
*/
int * arr_alloc (int arrSize);
/* Print array */
void print_arr (int * arr, int arrSize);
/*
    Regular merge sort algorithm 
        Merge part needs additional arrays to store merged data
        Merge part worst case time: T(n)=teta(n)
        There are log(n) stages of merge
        Total worst case time :T(n) = teta(n*log(n))
        Total worst case time (additional way) : T(n)=2T(n/2) + teta(n) => T(n)=teta(n*log(n))
        Returns number of comparisons.
*/
unsigned long long int merge_sort(int * arr, int p, int r);

/*
    descending merge sort (from mx to min) algorithm 
        Merge part needs additional arrays to store merged data
        Merge part worst case time: T(n)=teta(n)
        There are log(n) stages of merge
        Total worst case time :T(n) = teta(n*log(n))
        Total worst case time (additional way) : T(n)=2T(n/2) + teta(n) => T(n)=teta(n*log(n))
        Returns number of comparisons.
*/
int descending_merge_sort(int * arr, int p, int r);

/*
   Function merge_descending:
        Part of descending merge sort (from max to min) algorithm.
        Merges already sorted subarrays arr[p..q] (left subarray) 
        and arr[q+1..r] (right subarray) within array arr.
        Merge part worst case time: T(n)=teta(n) 

        The function returns number of comparisons.
*/
int merge_descending (int * arr, int p, int q, int r);


/*
    Insertion sort function.
        Sorts in place.
        Total worst case time: T(n)=teta(n^2)
        Returns number of swaps
*/
unsigned long long int insertion_sort (int * arr, int arrSize);
/*

 Modified merge sort.
    - Divides array recirsievely in regular way up to subarray size less or equal to K.
    - Modification : if the size is equal or less than k, performs INSERTION SORT to sort the 
        desired part of the array.
    - After it performes merge recursievely in regular way. 
    - Merge part still needs additional arrays to store merged data
    Total worst case time : 
        - Sort n/k subarrays with insertion sort : T(n)=teta(n*k)
        - Each merge runs teta(n) in worst case
        - There are log(n/k) stages in merge of n/k subarrays
        - Total merge time of n/k subarrays:  T(n) = teta(n*log(n/k))
        - Total worst case time of modified merge sort : T(n)=teta(n*k + n*log(n/k)) 
*/
t_comps_and_swaps merge_insertion_sort(int * arr, int p, int r, int k);
/* 
  Fill two identical arrays arr and arr1 with random data
*/
void fill_rand_arr (int * arr, int arrSize, int rand_max_value);
/*
  Fill array with rand numbers in ascending (rising) order with merge sort.
*/
void fill_rand_arr_asc_order (int * arr, int arrSize, int rand_max_value);
/*
  Fill array with rand numbers in descending (from max to min) order with merge sort.
*/
void fill_rand_arr_descending_order (int * arr, int arrSize, int rand_max_value);
/*
    Calculate elapsed time in ns
*/
double elapsed_time_ns (struct timespec start, struct timespec end);
/* 
  Returns average time of merge sort for average_sample_size equal arrays, 
  Also returns number of comparisons (using pointer int * number_of_comparisons) 
*/
double merge_sort_average_time_calc (int * arr, int arrSize, int average_sample_size, unsigned long long int * number_of_comparisons);
/* 
  Returns average time of updated merge insertion sort 
  for sample size named "average_sample_size",
  The time measurement for average time calculation is done several 
  times using exact copy of the same array.
  Also returns number of comparisons using pointer int * number_of_comparisons 
*/
double merge_insertion_sort_average_time_calc (int * arr, int arrSize, int k_val, int average_sample_size, t_comps_and_swaps * number_of_comps_and_swaps);
/*
  copy arr to arr1 (arr and arr1 are arrays of the same size)
*/
void copy_arr (int * arr, int * arr1, int arrSize);
/*
Calculate times of merge sort vs. modified merge insertion sort 
for given array. 
*/
void compare_algorithms_times(int * arr, int arrSize);
/*
    Generate array with many duplicates: 
        values form 1 to log(n)
*/
void gen_arr_with_many_duplicates (int * arr, int arrSize);
/* Quick sort recursive algorithm
   for k-sorted array prepare. 
*/
void quick_sort_k_sorted (int * arr, int p, int r, int k);
/*
    Partition part of quick sort    
    (basic algorithm) 
*/
int partition (int * arr, int p, int r);
/* Quick sort recursive algorithm */
void quick_sort (int * arr, int p, int r);
/* exchange two integer values */
void exchange (int * a, int * b);


int main() {
    int arrSize;
    int arrDataRange = 850000;
    int * arr; /* two arrays for comparison of sort time with two two algorithms*/
    printf("enter array size : ");
    scanf("%d",&arrSize);
    printf("Enter K value for K-sorted array : ");
    int k_sorted_val ; /* k value for K-sorted array */
    scanf("%d",&k_sorted_val);   
    /* allocate dynamic memory for 2 arrays : 
       arr will be stored aside, arr1 will be copied from arr 
       before every check to allow check with the same data
       for better comparison
    */
    arr  = arr_alloc (arrSize);
    

    
    /* ---------------- CHECK WITH RANDOM ARRAY ------------------------------------ */
    
    printf("\n\n------------- CHECK WITH RANDOM ARRAY ------------------------\n");
    printf("Array size : n=%d\n", arrSize);
    /* set range of random numbers : */ 
    arrDataRange = arrSize*5; /* set data range within the array */ 
    /* fill array with random data */
    fill_rand_arr(arr,arrSize,arrDataRange);
    compare_algorithms_times(arr,arrSize);

    /* ---------------- CHECK WITH RANDOM ARRAY IN ASCENDING (RISING ORDER ) ---------- */
    printf("\n\n------ CHECK WITH RANDOM ARRAY IN ASCENDING ORDER ------------\n");

    printf("Array size : n=%d\n", arrSize);
    /* set range of random numbers : */ 
    arrDataRange = arrSize*5; /* set data range within the array */ 
    fill_rand_arr_asc_order(arr, arrSize,arrDataRange);
    /* copy data of arr to arr1 */
    compare_algorithms_times(arr,arrSize);


    /* ---------------- CHECK WITH RANDOM ARRAY IN DESCENDING (FROM MAX to MIN ORDER ) ---------- */
    printf("\n\n------ CHECK WITH RANDOM ARRAY IN DESCENDING ORDER -----------\n");
    printf("Array size : n=%d\n", arrSize);
    /* set range of random numbers : */ 
    arrDataRange = arrSize*5; /* set data range within the array */ 
    fill_rand_arr_descending_order(arr, arrSize,arrDataRange);
    compare_algorithms_times(arr,arrSize);


    /* ---------------- START CHECK WITH RANDOM ARRAY IN DESCENDING (FROM MAX to MIN ORDER ) ---------- */
    printf("\n\n------ CHECK WITH RANDOM ARRAY WITH MANY DUPLICATES  ---------\n");
    printf("Array size : n=%d\n", arrSize);
    /* set range of random numbers : */ 
    arrDataRange = arrSize*5; /* set data range within the array */ 
    gen_arr_with_many_duplicates(arr, arrSize);
    compare_algorithms_times(arr,arrSize);
    
    /* ---------------- START CHECK WITH K-SORTED ARRAY ---------- */
    printf("\n\n------ CHECK WITH %d-SORTED RANDOM ARRAY  ---------\n",k_sorted_val);
    printf("Array size : n=%d\n", arrSize);
    printf("K value for K-sorted array: %d\n", k_sorted_val);

    fill_rand_arr(arr,arrSize,arrSize*6);
    
    printf("\nGenerate %d-sorted array: \n", k_sorted_val);
    quick_sort_k_sorted(arr,0,arrSize-1,k_sorted_val);
    if (arrSize<=100) print_arr(arr,arrSize);
    compare_algorithms_times(arr,arrSize);   

    free(arr);
    arr = NULL;

}

/*
    Calculate elapsed time in ns
*/
double elapsed_time_ns (struct timespec start, struct timespec end){ 
    double seconds = end.tv_sec - start.tv_sec;
    double nanoseconds = end.tv_nsec - start.tv_nsec;
    if (nanoseconds < 0) {
        seconds -= 1;
        nanoseconds += 1000000000;
    }

    return seconds * 1000000000 + nanoseconds; 
}


/* 
  Returns average time of merge sort for average_sample_size equal arrays, 
  Also returns number of comparisons (using pointer int * number_of_comparisons) 
*/
double merge_sort_average_time_calc (int * arr, int arrSize, int average_sample_size, unsigned long long int * number_of_comparisons){
    int i ; /* index */
    double  mergeSortElapsed = 0.0, sumTime = 0.0;
    int * arr1;
    unsigned long long int num_of_comp;  /* number_of_comparisons */
    /* allocate array for data copy */
    arr1 = arr_alloc (arrSize);
    /* POSIX (Linux) clock measure struct */
    struct timespec start, end;
    for (i=0;i<average_sample_size;i++){
        /* copy data of arr to arr1 */
        copy_arr(arr,arr1,arrSize);
        /* measure elapsed time for merge_sort algorithm */
        clock_gettime(CLOCK_MONOTONIC, &start);
        /* perform merge sort */
        num_of_comp = merge_sort(arr1,0,arrSize-1);
        clock_gettime(CLOCK_MONOTONIC, &end);
        mergeSortElapsed = elapsed_time_ns(start, end); 
        sumTime += mergeSortElapsed; 
        debug_only_code(printf("Time taken by regular merge_sort: %.2f nsec\n", mergeSortElapsed);); 
        
    }
    * number_of_comparisons = num_of_comp; /* return last number of comparisons */ 
    free(arr1);
    arr1 = NULL;
    /* return average elapsed time */
    return sumTime/average_sample_size;
}    



/* 
  Returns average time of updated merge insertion sort 
  for sample size named "average_sample_size",
  The time measurement for average time calculation is done several 
  times using exact copy of the same array.
  Also returns number of comparisons using pointer int * number_of_comparisons 
*/
double merge_insertion_sort_average_time_calc (int * arr, int arrSize, int k_val, int average_sample_size, t_comps_and_swaps * number_of_comps_and_swaps){
    int i ; /* index */
    double  mergeSortElapsed, sumTime = 0, averageTime;
    int * arr1;
    t_comps_and_swaps num_of_comp_swap;  /* number_of_comparisons */
    /* allocate array for data copy */
    arr1 = arr_alloc (arrSize);
    /* POSIX (Linux) clock measure struct */
    struct timespec start, end;
    for (i=0;i<average_sample_size;i++){
        /* copy data of arr to arr1 */
        copy_arr(arr,arr1,arrSize);
        /* measure elapsed time for merge_insertion_sort algorithm */
        clock_gettime(CLOCK_MONOTONIC, &start);
        /* perform merge sort */
        num_of_comp_swap = merge_insertion_sort(arr1,0,arrSize-1,k_val);
        clock_gettime(CLOCK_MONOTONIC, &end);
        mergeSortElapsed = elapsed_time_ns(start, end); 
        sumTime += mergeSortElapsed; 
        debug_only_code(printf("Time taken by modified insertion - merge_sort: %.2f nsec\n", mergeSortElapsed);); 
        
    }
    * number_of_comps_and_swaps = num_of_comp_swap; /* return last number of comparisons */ 
    
   
    averageTime = sumTime/average_sample_size;
    
    if (averageTime/NS_TO_SEC >= 1 ){

        printf("\twhen k = %d , average time: %.1f sec\n"
            "\t             num of comparisons: %llu \n"
            "\t             num_of_swaps: %llu \n", 
            k_val,
            averageTime/NS_TO_SEC,
            num_of_comp_swap.comps_num,
            num_of_comp_swap.swaps_num
        );  

    } else if (averageTime/NS_TO_MS >= 1 && averageTime/NS_TO_SEC < 1){
    
        printf("\twhen k = %d , average time: %.1f msec\n"
            "\t             num of comparisons: %llu \n"
            "\t             num_of_swaps: %llu \n", 
            k_val,
            averageTime/NS_TO_MS,
            num_of_comp_swap.comps_num,
            num_of_comp_swap.swaps_num
        );     
    } else if (averageTime/NS_TO_MS < 1 && averageTime/NS_TO_US >=1){
        printf("\twhen k = %d , average time: %.1f usec\n"
            "\t             num of comparisons: %llu \n"
            "\t             num_of_swaps: %llu \n", 
            k_val,
            averageTime/NS_TO_US,
            num_of_comp_swap.comps_num,
            num_of_comp_swap.swaps_num
        );  


    } else {

        printf("\twhen k = %d , average time: %.1f nsec\n"
            "\t             num of comparisons: %lld \n"
            "\t             num_of_swaps: %lld \n", 
            k_val,
            averageTime,
            num_of_comp_swap.comps_num,
            num_of_comp_swap.swaps_num
        );  

    }



    
    free(arr1);
    arr1 = NULL;
    /* return average elapsed time */
    return sumTime/average_sample_size;
}  




/*
    Print array
*/
void print_arr (int * arr, int arrSize){

    int i;
    
    if (arrSize == 0)
        return;
    printf("%d", arr[0]);
    for (i=1; i<arrSize; i++){
        printf (", %d", arr[i]);
    }
    printf("\n");
}


/*
  Allocate dynamic memory for array.
*/
int * arr_alloc (int arrSize){

    int * p_arr = NULL;

    p_arr = (int *)malloc(sizeof(int)*arrSize);
    if (p_arr == NULL) {
        printf("Failure of dynamic array allocation. \nExitting\n");
        exit(EXIT_FAILURE);
    }

    return p_arr;

}


/*
   Function merge:
        Merges already sorted subarrays arr[p..q] (left subarray) 
        and arr[q+1..r] (right subarray) within array arr.
        Merge part worst case time: T(n)=teta(n) 

        The function returns number of comparisons.
*/
unsigned long long int merge (int * arr, int p, int q, int r){

    int leftSubArrSize;
    int rightSubArrSize;
    int * leftArr;                  /* pointer to left arr */ 
    int * rightArr;                 /* pointer to right arr */ 
    int i;                          /* index */
    int j;                          /* index */
    int k;                          /* index */
    unsigned long long int num_of_comparisons = 0;     /* number of comparisons */

    leftSubArrSize  = q-p+1; /* arr[p..q] is left subarray)    */
    rightSubArrSize = r-q;   /* arr[q+1..r] is right subarray) */
    
    /* allocate left and right arrays dynamically: */
    leftArr  = arr_alloc(leftSubArrSize+1);  /* last element for sentinel*/ 
    rightArr = arr_alloc(rightSubArrSize+1);

    /* copy arr[p..q] into LeftArr[0..SizeOfLeftSubArray-1]*/
    for (i=0;i<leftSubArrSize;i++){
        leftArr[i]=arr[p+i];
    }
    leftArr[leftSubArrSize] = INT_MAX; /* set sentinel at array's last element */
    /* copy arr[p..q] into LeftArr[0..SizeOfLeftSubArray-1]*/
    for (i=0;i<rightSubArrSize;i++){
        rightArr[i]=arr[q+1+i];
    }
    rightArr[rightSubArrSize] = INT_MAX; /* set sentinel at array's last element */

    debug_only_code(printf("Merge p=%d, q=%d r=%d \n",p,q,r););
    debug_only_code(printf(" Before: \n "););
    debug_only_code(print_arr(arr+p,r-p+1);); 
    debug_only_code(printf(" Left subarray: \n "););
    debug_only_code(print_arr(leftArr,leftSubArrSize+1););
    debug_only_code(printf(" Right subarray: \n "););
    debug_only_code(print_arr(rightArr,rightSubArrSize+1););

    
    i=j=0;
    /* perform merge : */
    for (k=p;k<=r;k++){
        /* increment number of comparisons */
        if (leftArr[i]<=rightArr[j]){
            arr[k]=leftArr[i];
            i++;
        } else {
            arr[k]=rightArr[j];
            j++;
        }
    }
    debug_only_code(printf(" Merged array: \n "););    
    debug_only_code(print_arr(arr+p,r-p+1);); 
    /* calculate number of comparisons */
    num_of_comparisons = i+j;
    debug_only_code(printf("num_of_comparisons = %llu\n", num_of_comparisons););
    /* free memory for left and right arrays */
    free(leftArr);
    leftArr = NULL;
    free(rightArr);
    rightArr = NULL;

    return num_of_comparisons;

}


/*
    Regular merge sort algorithm 
        Merge part needs additional arrays to store merged data
        Merge part worst case time: T(n)=teta(n)
        There are log(n) stages of merge
        Total worst case time :T(n) = teta(n*log(n))
        Total worst case time (additional way) : T(n)=2T(n/2) + teta(n) => T(n)=teta(n*log(n))
        Returns number of comparisons.
*/
unsigned long long int merge_sort(int * arr, int p, int r){
    int q;
    unsigned long long int num_of_comparisons = 0;     /* total number of comparisons */
    unsigned long long int num_of_comparisons_left = 0;
    unsigned long long int num_of_comparisons_right = 0;
    if (p < r){
        
        /* divide the array into 2 subarray parts */ 
        q=(int)floor((p+r)/2);
        debug_only_code(printf("p=%d,q=%d,r=%d\n",p,q,r););
        /* recursive sort of both subarray parts separetly */
        num_of_comparisons_left = merge_sort(arr,p,q);
        num_of_comparisons_right = merge_sort(arr,q+1,r);
        /* merge two subarrays into one sorted subarray */
        /* and increment number of comparisons */
        num_of_comparisons +=  num_of_comparisons_left + 
                               num_of_comparisons_right +
                               + merge(arr,p,q,r);
        
    }
    
    return num_of_comparisons;

}


/*
    descending merge sort (from mx to min) algorithm 
        Merge part needs additional arrays to store merged data
        Merge part worst case time: T(n)=teta(n)
        There are log(n) stages of merge
        Total worst case time :T(n) = teta(n*log(n))
        Total worst case time (additional way) : T(n)=2T(n/2) + teta(n) => T(n)=teta(n*log(n))
        Returns number of comparisons.
*/
int descending_merge_sort(int * arr, int p, int r){
    int q;
    int num_of_comparisons = 0;     /* total number of comparisons */
    int num_of_comparisons_left = 0;
    int num_of_comparisons_right = 0;
    if (p < r){
        
        /* divide the array into 2 subarray parts */ 
        q=(int)floor((p+r)/2);
        debug_only_code(printf("p=%d,q=%d,r=%d\n",p,q,r););
        /* recursive sort of both subarray parts separetly */
        num_of_comparisons_left = descending_merge_sort(arr,p,q);
        num_of_comparisons_right = descending_merge_sort(arr,q+1,r);
        /* merge two subarrays into one sorted subarray */
        /* and increment number of comparisons */
        num_of_comparisons +=  num_of_comparisons_left + 
                               num_of_comparisons_right +
                               + merge_descending(arr,p,q,r);
        
    }
    
    return num_of_comparisons;

}


/*
   Function merge_descending:
        Part of descending merge sort (from max to min) algorithm.
        Merges already sorted subarrays arr[p..q] (left subarray) 
        and arr[q+1..r] (right subarray) within array arr.
        Merge part worst case time: T(n)=teta(n) 

        The function returns number of comparisons.
*/
int merge_descending (int * arr, int p, int q, int r){

    int leftSubArrSize;
    int rightSubArrSize;
    int * leftArr;                  /* pointer to left arr */ 
    int * rightArr;                 /* pointer to right arr */ 
    int i;                          /* index */
    int j;                          /* index */
    int k;                          /* index */
    int num_of_comparisons = 0;     /* number of comparisons */

    leftSubArrSize  = q-p+1; /* arr[p..q] is left subarray)    */
    rightSubArrSize = r-q;   /* arr[q+1..r] is right subarray) */
    
    /* allocate left and right arrays dynamically: */
    leftArr  = arr_alloc(leftSubArrSize+1);  /* last element for sentinel*/ 
    rightArr = arr_alloc(rightSubArrSize+1);

    /* copy arr[p..q] into LeftArr[0..SizeOfLeftSubArray-1]*/
    for (i=0;i<leftSubArrSize;i++){
        leftArr[i]=arr[p+i];
    }
    leftArr[leftSubArrSize] = INT_MIN; /* set sentinel at array's last element */
    /* copy arr[p..q] into LeftArr[0..SizeOfLeftSubArray-1]*/
    for (i=0;i<rightSubArrSize;i++){
        rightArr[i]=arr[q+1+i];
    }
    rightArr[rightSubArrSize] = INT_MIN; /* set sentinel at array's last element */

    debug_only_code(printf("Merge p=%d, q=%d r=%d \n",p,q,r););
    debug_only_code(printf(" Before: \n "););
    debug_only_code(print_arr(arr+p,r-p+1);); 
    debug_only_code(printf(" Left subarray: \n "););
    debug_only_code(print_arr(leftArr,leftSubArrSize+1););
    debug_only_code(printf(" Right subarray: \n "););
    debug_only_code(print_arr(rightArr,rightSubArrSize+1););

    
    i=j=0;
    /* perform merge : */
    for (k=p;k<=r;k++){
        /* increment number of comparisons */
        if (leftArr[i]>rightArr[j]){
            arr[k]=leftArr[i];
            i++;
        } else {
            arr[k]=rightArr[j];
            j++;
        }
    }
    debug_only_code(printf(" Merged array: \n "););    
    debug_only_code(print_arr(arr+p,r-p+1);); 
    /* calculate number of comparisons */
    num_of_comparisons = i+j;
    debug_only_code(printf("num_of_comparisons = %d\n", num_of_comparisons););
    /* free memory for left and right arrays */
    free(leftArr);
    leftArr = NULL;
    free(rightArr);
    rightArr = NULL;

    return num_of_comparisons;

}


/*
 Modified merge sort.
    - Divides array recirsievely in regular way up to subarray size less or equal to K.
    - Modification : if the size is equal or less than k, performs INSERTION SORT to sort the 
        desired part of the array.
    - After it performes merge recursievely in regular way. 
    - Merge part still needs additional arrays to store merged data
    Total worst case time : 
        - Sort n/k subarrays with insertion sort : T(n)=teta(n*k)
        - Each merge runs teta(n) in worst case
        - There are log(n/k) stages in merge of n/k subarrays
        - Total merge time of n/k subarrays:  T(n) = teta(n*log(n/k))
        - Total worst case time of modified merge sort : T(n)=teta(n*k + n*log(n/k)) 
*/
t_comps_and_swaps merge_insertion_sort(int * arr, int p, int r, int k){
    int q;
    unsigned long long int number_of_comparisons = 0; /* comparisons of merge */
    t_comps_and_swaps comps_and_swaps_left = {0,0}; /* number of comps and swaps in left array */
    t_comps_and_swaps comps_and_swaps_right = {0,0};/* number of comps and swaps in right array */
    unsigned long long int number_of_merge_comparisons = 0;
    unsigned long long int number_of_swaps = 0 ; /* swaps of insertion sort */
    t_comps_and_swaps comps_and_swaps = {0,0};   /* num of comps and swaps in current stage */
    if ( p>r){ 
        comps_and_swaps.swaps_num = 0;
        comps_and_swaps.comps_num = 0;
        return comps_and_swaps;
    }    
    else if ((r - p + 1) <= k) {
        number_of_swaps = insertion_sort (arr + p, r - p + 1);
        debug_only_code(printf("p=%d,r=%d,k=%d\n",p,r,k););
        comps_and_swaps.swaps_num = number_of_swaps;
        comps_and_swaps.comps_num = 0;
        return comps_and_swaps;
    }    
    else {
        /* divide array into 2 subarray parts */    
        q=(int)floor((p+r)/2);
        debug_only_code(printf("p=%d,q=%d,r=%d\n",p,q,r););
        /* perform recursive sort of each subarray part separetly */
        comps_and_swaps_left = merge_insertion_sort(arr,p,q,k);
        comps_and_swaps_right = merge_insertion_sort(arr,q+1,r,k);
        /* merge two subarrays into one sorted subarray */
        /* and calculate total number of comparisons    */
        number_of_merge_comparisons = merge(arr,p,q,r);
        /* total number of comparisons and swaps */
        number_of_comparisons += comps_and_swaps_left.comps_num +
                                 comps_and_swaps_right.comps_num + 
                                 number_of_merge_comparisons;
        number_of_swaps += comps_and_swaps_left.swaps_num+
                           comps_and_swaps_right.swaps_num;                        

        comps_and_swaps.comps_num = number_of_comparisons;
        comps_and_swaps.swaps_num = number_of_swaps;

           
    }  

    return comps_and_swaps;  
    

}


/*
    Insertion sort function.
        Sorts in place.
        Total worst case time: T(n)=teta(n^2)
        Returns number of swaps
*/
unsigned long long int insertion_sort (int * arr, int arrSize){

    int j;
    int i;
    int key;
    int num_of_swaps = 0;
    
    /* IMPORTANT : must check i >= 0 before accessing arr[i] !!! */
    for (j=1;j<arrSize;j++){
        key = arr[j];
        /* insert A[j] into the sorted sequence A[0..j-1] */
        i = j-1;
        while((i>=0)&&(arr[i]>key) ){
            
            arr[i+1] = arr[i];
            i--;
            num_of_swaps++;
        }
        arr[i+1]=key;
    }

    return num_of_swaps;

}


/* 
  Fill array arr with random data
*/
void fill_rand_arr (int * arr, int arrSize, int rand_max_value){
    int i;
    
    /* Seed the random number generator with current time */
    rand_seed += 1235;
    srand(rand_seed);
    
    for (i=0;i<arrSize;i++){

        /* fill arrays with random data */
        arr[i]  = rand()%rand_max_value;; 
         
    }
    if (arrSize <=100){
        printf("\nGenerated random array:\n");
        print_arr(arr,arrSize);
        printf("\n");
    }  


}


/*
  Fill array with rand numbers in ascending (rising) order with merge sort.
*/
void fill_rand_arr_asc_order (int * arr, int arrSize, int rand_max_value){
    fill_rand_arr(arr, arrSize, rand_max_value);
    merge_sort(arr,0,arrSize-1);
    /* print array data only if its size is less or equal to 100: */
    if (arrSize <=100){
        printf("\nRandom array sorted in ascending order:\n");
        print_arr(arr,arrSize);
        printf("\n");
    }    
}


/*
  Fill array with rand numbers in descending (from max to min) order with merge sort.
*/
void fill_rand_arr_descending_order (int * arr, int arrSize, int rand_max_value){
    fill_rand_arr(arr, arrSize, rand_max_value);
    descending_merge_sort(arr,0,arrSize-1);
    if (arrSize <=100){
        printf("\nRandom array sorted in descending order:\n");
        print_arr(arr,arrSize);
        printf("\n");
    }    
}

/*
    Generate array with many duplicates: 
        values form 1 to log(n)
*/
void gen_arr_with_many_duplicates (int * arr, int arrSize){
    int rand_max_value = (int)log2(arrSize);
    fill_rand_arr (arr, arrSize, rand_max_value);
    if (arrSize <=100){
        printf("\nRandom array with many duplicates (from 1 to log2(n)) : \n");
        print_arr(arr,arrSize);
        printf("\n");
    }     

}


/*
  copy arr to arr1 (arr and arr1 are arrays of the same size)
*/
void copy_arr (int * arr, int * arr1, int arrSize){
    int i;
    for (i=0;i<arrSize;i++){
        arr1[i]=arr[i];
    }
}



/*
Calculate times of merge sort vs. modified merge insertion sort 
for given array. 
*/
void compare_algorithms_times(int * arr, int arrSize){
    int * arr1; /* copy arr data to arr1 in order to not change arr array */
    double averageMergeSortTime = 0.0;          /* elapsed time for merge sort */ 
    double averageMergeInsertionSortTime = 0.0;     /* elapsed time for merge sort with insertion sort */
    int k_val = 1; /* array partition value */
    int k_val_step_2; /* step of k value change for range k>n/2 and k<n */
   
 
    unsigned long long int num_of_comparisons; /* number of comparisons */
    double k_val_d;
    t_comps_and_swaps num_of_comps_and_swaps;
    arr1 = arr_alloc (arrSize);
    /* copy data of arr to arr1 */
    copy_arr(arr,arr1,arrSize);
    
    const int average_sample_size = AVERAGE_SAMPLE_SIZE;
    averageMergeSortTime = merge_sort_average_time_calc (arr1, arrSize,average_sample_size, &num_of_comparisons);

    if ((averageMergeSortTime / NS_TO_SEC) >= 1 ) {
        printf(" Average merge sort time (for %d mesurements): %.1f sec\n", 
            average_sample_size, averageMergeSortTime/NS_TO_SEC);
    } else if ((averageMergeSortTime / NS_TO_SEC) < 1 && (averageMergeSortTime / NS_TO_MS) >= 1 ){
        printf(" Average merge sort time (for %d mesurements): %.1f msec\n", 
            average_sample_size, averageMergeSortTime/NS_TO_MS);
    } else if ((averageMergeSortTime / NS_TO_MS) < 1 && (averageMergeSortTime / NS_TO_US) >= 1){
        printf(" Average merge sort time (for %d mesurements): %.1f usec\n", 
            average_sample_size, averageMergeSortTime/NS_TO_US);  
    } else {
        printf(" Average merge sort time (for %d mesurements): %.1f nsec\n", 
            average_sample_size, averageMergeSortTime);
    }   


    printf(" Total number of comparisons in merge sort (array size = %d) : %lld\n", arrSize, num_of_comparisons) ;               
    printf("\n\n\n");
    
    /* measure elapsed time of merge sort with insertion sort with different k values 
       choose values of k depending of n :

       1. Always present times for k = 1 and k = n.
       2. For k>2 and k<n : k_val_step_1 = k_val_step_1 * 2.
       3. For k > n/2 and k<n try to present 3 values , if it is possible (use step k_val_step_2).     
       
       calculate step values for k:
    */
    

    if ((arrSize/2)/3 > 1)
        k_val_step_2 = (arrSize/2)/3;
    else  
        k_val_step_2 = 1;         

    printf("Average merge_insertion_sort time (sample size for avrg calc = %d) :\n",average_sample_size);
    /* copy data of arr to arr1 again to get exactly the same array */
    copy_arr(arr,arr1,arrSize); 
    
    k_val = 1;
    averageMergeInsertionSortTime = merge_insertion_sort_average_time_calc (arr1, arrSize,k_val, average_sample_size, &num_of_comps_and_swaps);
    
    k_val_d=2.5;
    /* for (k_val=2; k_val<arrSize/2; k_val_d *= 1.414, k_val=(int)(k_val_d)) */
    for (k_val=2; k_val<arrSize/2; k_val_d *= 2.0, k_val=(int)(k_val_d))
    {
        averageMergeInsertionSortTime = merge_insertion_sort_average_time_calc (arr1, arrSize,k_val, average_sample_size, &num_of_comps_and_swaps);
    }
    for (k_val=arrSize/2; k_val<arrSize; k_val+=k_val_step_2){
        averageMergeInsertionSortTime = merge_insertion_sort_average_time_calc (arr1, arrSize,k_val, average_sample_size, &num_of_comps_and_swaps);
    }   
    k_val=arrSize;
    averageMergeInsertionSortTime = merge_insertion_sort_average_time_calc (arr1, arrSize,k_val, average_sample_size, &num_of_comps_and_swaps);
     

    free(arr1);
    arr1 = NULL; 

}


/* Quick sort recursive algorithm */
void quick_sort (int * arr, int p, int r){
    int q;
    if (p<r){
        /* divide array into two parts 
           ("green" and "red" and return the index of partition )
        */
        q = partition(arr,p,r);
        quick_sort(arr,p,q-1); /* sort left (green) part*/
        quick_sort(arr,q+1,r); /* sort right (red) part */ 
    }
}


/* Quick sort recursive algorithm
   for k-sorted array prepare.    */
void quick_sort_k_sorted (int * arr, int p, int r, int k){
    int q;
    if (r-p+1 >= k){
        /* divide array into two parts 
           ("green" and "red" and return the index of partition )
        */
        q = partition(arr,p,r);
        quick_sort_k_sorted(arr,p,q-1,k); /* sort left (green) part*/
        quick_sort_k_sorted(arr,q+1,r,k); /* sort right (red) part */ 
    }
}


/*
    Partition part of quick sort    
    (basic algorithm) 
*/
int partition (int * arr, int p, int r){

    /* choose pivot value */
    int pivot= arr[r];;
    int i = p-1; /* i is last element of "green" area (<= pivot)*/
    int j;       /* after last element of "red" area (> pivot)  */

     
    for (j=p;j<r;j++){
        if (arr[j]<=pivot){
            i++;
            /* exchange A[i]<->A[j] */
            exchange(arr+i,arr+j);
        }
    } 
    
    debug_only_code( printf("i=%d, j=%d, p=%d, r=%d , pivot = %d\n", i,j,p,r, pivot););
    
    exchange(arr+i+1,arr+r);

    debug_only_code(print_arr(arr,r-p+1););
    
    return i+1;
}



/* exchange two integer values */
void exchange (int * a, int * b){
        int temp;
        temp = *a;
        *a = *b;
        *b = temp;
}

