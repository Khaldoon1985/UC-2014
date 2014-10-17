//Udacity HW 4
//Radix Sorting

#include "utils.h"
#include <thrust/host_vector.h>

/* Red Eye Removal
   ===============
   
   For this assignment we are implementing red eye removal.  This is
   accomplished by first creating a score for every pixel that tells us how
   likely it is to be a red eye pixel.  We have already done this for you - you
   are receiving the scores and need to sort them in ascending order so that we
   know which pixels to alter to remove the red eye.

   Note: ascending order == smallest to largest

   Each score is associated with a position, when you sort the scores, you must
   also move the positions accordingly.

   Implementing Parallel Radix Sort with CUDA
   ==========================================

   The basic idea is to construct a histogram on each pass of how many of each
   "digit" there are.   Then we scan this histogram so that we know where to put
   the output of each digit.  For example, the first 1 must come after all the
   0s so we have to know how many 0s there are to be able to start moving 1s
   into the correct position.

   1) Histogram of the number of occurrences of each digit
   2) Exclusive Prefix Sum of Histogram
   3) Determine relative offset of each digit
        For example [0 0 1 1 0 0 1]
                ->  [0 1 0 1 2 3 2]
   4) Combine the results of steps 2 & 3 to determine the final
      output location for each element and move it there

   LSB Radix sort is an out-of-place sort and you will need to ping-pong values
   between the input and output buffers we have provided.  Make sure the final
   sorted results end up in the output buffer!  Hint: You may need to do a copy
   at the end.

 */


__global__
void radixSort(unsigned int* const d_inputVals,
               unsigned int* const d_inputPos,
               unsigned int* const d_outputVals,
               unsigned int* const d_outputPos,
               const size_t numElems,
               unsigned int* const d_histogram,
               unsigned int* const d_scan,
               const unsigned int nBins
               )
{
    const unsigned int x = threadIdx.x;
    const unsigned int y = threadIdx.y;
    const unsigned int tIdx = x + y*blockDim.x;
    if (tIdx >= numElems)
        return;

    unsigned int* d_srcVals = d_inputVals;
    unsigned int* d_srcPos = d_inputPos;
    unsigned int* d_dstVals = d_outputVals;
    unsigned int* d_dstPos = d_outputPos;

    // iterate throught each bit
    for (unsigned int i = 0; i < 8 * sizeof(unsigned int); i += 1u) {
        const unsigned int mask = 1u << i;

        // initialize d_histogram, and d_scan
        if (tIdx < nBins) {
            d_histogram[tIdx] = 0u;
        }
        __syncthreads();

        // evaluate predicate, and compute histogram
        const unsigned int bin = (d_srcVals[tIdx % numElems] & mask) >> i;
        atomicAdd(&(d_histogram[bin % nBins]), 1u);
        __syncthreads();

        // compute the starting location of each bin with exclusive scan
        // This could be simple as
        if (tIdx == 0u) {
            d_scan[0] = 0u;
            d_scan[1] = d_histogram[0];
        }
        __syncthreads();

        // Move element to correct location
        const unsigned int offset = atomicAdd(&d_scan[bin % nBins], 1u);
        d_dstVals[offset % numElems] = d_srcVals[tIdx % numElems];
        d_dstPos[offset % numElems] = d_srcPos[tIdx% numElems];
        __syncthreads();

        // Swap the buffers
        unsigned int * tmp = d_srcVals;
        d_srcVals = d_dstVals;
        d_dstVals = tmp;

        tmp = d_srcPos;
        d_srcPos = d_dstPos;
        d_dstPos = tmp;
        __syncthreads();
    }
}

void your_sort(unsigned int* const d_inputVals,
               unsigned int* const d_inputPos,
               unsigned int* const d_outputVals,
               unsigned int* const d_outputPos,
               const size_t numElems)
{
    // Step 1: Allocate memory
    const unsigned int nBins = 2u;
    unsigned int * d_histogram, * d_scan;
    checkCudaErrors( cudaMalloc((void **)&d_histogram, nBins * sizeof(unsigned int)));
    checkCudaErrors( cudaMalloc((void **)&d_scan, nBins * sizeof(unsigned int)));

    // Step 2: call radix sort
    std::cout << "Number of elements : " << numElems << std::endl;
    const dim3 blockSize(32, 32, 1);
    radixSort<<<1, blockSize>>>(d_inputVals, d_inputPos, d_outputVals, d_outputPos, numElems,
                                       d_histogram, d_scan, nBins);
    cudaDeviceSynchronize(); checkCudaErrors(cudaGetLastError());

    // Step 3: release resources
    checkCudaErrors( cudaFree(d_histogram));
    checkCudaErrors( cudaFree(d_scan)); 
}
