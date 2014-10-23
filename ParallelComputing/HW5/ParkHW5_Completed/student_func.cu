/* Udacity HW5
   Histogramming for Speed

   The goal of this assignment is compute a histogram
   as fast as possible.  We have simplified the problem as much as
   possible to allow you to focus solely on the histogramming algorithm.

   The input values that you need to histogram are already the exact
   bins that need to be updated.  This is unlike in HW3 where you needed
   to compute the range of the data and then do:
   bin = (val - valMin) / valRange to determine the bin.

   Here the bin is just:
   bin = val

   so the serial histogram calculation looks like:
   for (i = 0; i < numElems; ++i)
     histo[val[i]]++;

   That's it!  Your job is to make it run as fast as possible!

   The values are normally distributed - you may take
   advantage of this fact in your implementation.

*/


#include "utils.h"
#include "reference.cpp"


__global__
void yourHisto(const unsigned int* const vals, //INPUT
               unsigned int* const histo,      //OUPUT
               int numVals)
{
  //TODO fill in this kernel to calculate the histogram
  //as quickly as possible

  //Although we provide only one kernel skeleton,
  //feel free to use more if it will help you
  //write faster code
   /*int id_x = blockIdx.x * blockDim.x + threadIdx.x ;
   int id_y = blockIdx.y * blockDim.y + threadIdx.y ;
   int absoulteId = id_y*numVals + id_x ;
     
   atomicAdd (&histo[vals[absoulteId]] , 1 );*/
    
   int id = blockIdx.x * blockDim.x + threadIdx.x ;
   atomicAdd (&histo[vals[id]] , 1 );


}

void computeHistogram(const unsigned int* const d_vals, //INPUT
                      unsigned int* const d_histo,      //OUTPUT
                      const unsigned int numBins,
                      const unsigned int numElems)
{
  //TODO Launch the yourHisto kernel

  //if you want to use/launch more than one kernel,
  //feel free

    //test algorithm
  /*int Nby2_ = numElems/2 ; 

  dim3 blocks (16,16);
  dim3 grid(Nby2_ / 16 , Nby2_/16) ; */

  //yourHisto <<<grid , blocks>>> (d_vals ,  d_histo, Nby2_ ) ;
    yourHisto <<<numElems/1024 , 1024>>> (d_vals ,  d_histo, numElems);

  /*delete[] h_vals;
  delete[] h_histo;
  delete[] your_histo;*/
}
