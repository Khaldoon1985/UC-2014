//Udacity HW 6
//Poisson Blending

/* Background
   ==========

   The goal for this assignment is to take one image (the source) and
   paste it into another image (the destination) attempting to match the
   two images so that the pasting is non-obvious. This is
   known as a "seamless clone".

   The basic ideas are as follows:

   1) Figure out the interior and border of the source image
   2) Use the values of the border pixels in the destination image 
      as boundary conditions for solving a Poisson equation that tells
      us how to blend the images.
   
      No pixels from the destination except pixels on the border
      are used to compute the match.

   Solving the Poisson Equation
   ============================

   There are multiple ways to solve this equation - we choose an iterative
   method - specifically the Jacobi method. Iterative methods start with
   a guess of the solution and then iterate to try and improve the guess
   until it stops changing.  If the problem was well-suited for the method
   then it will stop and where it stops will be the solution.

   The Jacobi method is the simplest iterative method and converges slowly - 
   that is we need a lot of iterations to get to the answer, but it is the
   easiest method to write.

   Jacobi Iterations
   =================

   Our initial guess is going to be the source image itself.  This is a pretty
   good guess for what the blended image will look like and it means that
   we won't have to do as many iterations compared to if we had started far
   from the final solution.

   ImageGuess_prev (Floating point)
   ImageGuess_next (Floating point)

   DestinationImg
   SourceImg

   Follow these steps to implement one iteration:

   1) For every pixel p in the interior, compute two sums over the four neighboring pixels:
      Sum1: If the neighbor is in the interior then += ImageGuess_prev[neighbor]
             else if the neighbor in on the border then += DestinationImg[neighbor]

      Sum2: += SourceImg[p] - SourceImg[neighbor]   (for all four neighbors)

   2) Calculate the new pixel value:
      float newVal= (Sum1 + Sum2) / 4.f  <------ Notice that the result is FLOATING POINT
      ImageGuess_next[p] = min(255, max(0, newVal)); //clamp to [0, 255]


    In this assignment we will do 800 iterations.
   */


#include "stdio.h"
#include "utils.h"
#include <thrust/host_vector.h>

__global__ void cal_mask( bool* d_mask, uchar4* d_source , int N){

    int n = threadIdx.x + blockDim.x * blockIdx.x;

    if (n >= N) return;
    
    if (d_source[n].x == 255 && d_source[n].y == 255 && d_source[n].z == 255) {
        d_mask[n] = 0; //outside the mask
    }
    
    else {
        d_mask[n] = 1; // inside the mask
    }
}

__global__ void isInside( int* d_inside, bool* d_mask , int N , int col_N){

    int n = threadIdx.x + blockDim.x * blockIdx.x;

    if (n >= N) {
        return;
    }

    if (d_mask[n] == true) {
    
        // is in mask, assume there are pixel around it.(never out of bound)
    
        if (d_mask[n-1] == true && d_mask[n+1] ==true && d_mask[n-col_N] == true && d_mask[n+col_N]== true) {
            // it's inside mask
            d_inside[n] = 1;
        }
        else {
            d_inside[n] = 0;
        }
    }
    else {
        d_inside[n] = 8;
    }
}

__global__ void separateChannels(float* red, float* blue, float* green, uchar4* d_source, int N) {

    int n = threadIdx.x + blockDim.x * blockIdx.x;

    if (n >= N) {
        return;
    }

    red[n]  = d_source[n].x;
    blue[n] = d_source[n].y;
    green[n]= d_source[n].z;
}

__global__ void jacobi( float* d_out, float* d_in , float* d_source, float* d_dest, int* d_inside, bool* d_mask , int N, int col) {
    
    int n = threadIdx.x + blockDim.x * blockIdx.x;
    
    if (n >= N) {
        return;
    }

    if (d_mask[n] == 1) {
        if (d_inside[n] ==1 ) {
            float sum1 = 0.0f;
            float neighbor = 4.0f;
            float sum2 = 0.0f;

            if (d_inside[n-1] == 1) {
                sum1 += d_in[n-1];
                sum2 += d_source[n] - d_source[n-1];
            }
            
            else{
                // n-1 on the border
                sum1 += d_dest[n-1];
                sum2 += d_source[n] - d_source[n-1];
            }

            if (d_inside[n+1] == 1) {
                sum1 += d_in[n+1];
                sum2 += d_source[n] - d_source[n+1];
            }
            else{
                sum1 +=d_dest[n+1];
                sum2 += d_source[n] - d_source[n+1];
            }


            if (d_inside[n+col] == 1) {
                sum1 += d_in[n+col];
                sum2 += d_source[n] - d_source[n+col];
            }
            else{
                sum1 +=d_dest[n+col];
                sum2 += d_source[n] - d_source[n+col];
            }


            if (d_inside[n-col] == 1) {
                sum1 += d_in[n-col];
                sum2 += d_source[n] - d_source[n-col];
            }
           
            else{
                sum1 +=d_dest[n-col];
                sum2 += d_source[n] - d_source[n-col];
            }
            
            float newVal = (sum1 + sum2) / neighbor ;
            d_out[n] = min(255.f,max(0.f,newVal));

    
        }
    }
}

__global__ void storeBack( uchar4* d_dest, float* red, float* blue, float* green, int* d_inside , bool* d_mask, int N){

    int n = threadIdx.x + blockDim.x * blockIdx.x;

    if (n >= N) {
        return;
    }


    if (d_mask[n] == 1) {
    
        if (d_inside[n] == 1) {
        
            d_dest[n].x = char(red[n]  );
            d_dest[n].y = char(blue[n] );
            d_dest[n].z = char(green[n]);
        }
    }
}

void your_blend(
                //Input
                const uchar4* const h_sourceImg, const size_t numRowsSource, const size_t numColsSource, const uchar4* const h_destImg,
                //Output
                uchar4* const h_blendedImg) {

    printf("row: %lu, col: %lu\n", numRowsSource, numColsSource);

    int N = numRowsSource * numColsSource;
    bool* d_mask;
    int* d_inside;
    uchar4* d_source;
    uchar4* d_dest;
    float* d_source_red;
    float* d_source_blue;
    float* d_source_green;

    float *d_dest_red;
    float *d_dest_blue;
    float *d_dest_green;

    float *red;
    float *blue;
    float *green;

    float *red2;
    float *blue2;
    float *green2;

    checkCudaErrors(cudaMalloc((void **)&d_mask   , N * sizeof(bool  ) )  );
    checkCudaErrors(cudaMalloc((void **)&d_inside , N * sizeof(int   ) )  );
    checkCudaErrors(cudaMalloc((void **)&d_source , N * sizeof(uchar4) )  );
    checkCudaErrors(cudaMalloc((void **)&d_dest   , N * sizeof(uchar4) )  );
    checkCudaErrors(cudaMalloc((void **)&d_source_red    , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&d_source_blue   , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&d_source_green  , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&d_dest_red    , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&d_dest_blue   , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&d_dest_green  , N * sizeof(float ) )  );

    checkCudaErrors(cudaMalloc((void **)&red     , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&blue    , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&green   , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&red2     , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&blue2    , N * sizeof(float ) )  );
    checkCudaErrors(cudaMalloc((void **)&green2   , N * sizeof(float ) )  );

    checkCudaErrors(cudaMemcpy(d_source,h_sourceImg,N* sizeof(uchar4), cudaMemcpyHostToDevice));
    checkCudaErrors(cudaMemcpy(d_dest  ,h_destImg  ,N* sizeof(uchar4), cudaMemcpyHostToDevice));

    cal_mask<<<N/1024 + 1 , 1024 >>>(d_mask, d_source, N);

    cudaDeviceSynchronize();
    
    isInside<<<N/1024 + 1 , 1024 >>>(d_inside, d_mask, N, numColsSource);

    separateChannels<<<N/1024 + 1, 1024>>>(d_source_red, d_source_blue, d_source_green, d_source, N);
    separateChannels<<<N/1024 + 1, 1024>>>(d_dest_red, d_dest_blue, d_dest_green, d_dest, N);
    separateChannels<<<N/1024 + 1, 1024>>>(red, blue, green, d_source, N);

    cudaDeviceSynchronize();
    
    for (int i =0; i < 400; i++) {
        jacobi<<<N/1024 + 1, 1024>>>(red2, red, d_source_red, d_dest_red, d_inside, d_mask, N, numColsSource);
        jacobi<<<N/1024 + 1, 1024>>>(blue2, blue, d_source_blue, d_dest_blue,d_inside, d_mask, N, numColsSource);
        jacobi<<<N/1024 + 1, 1024>>>(green2, green, d_source_green, d_dest_green,d_inside, d_mask, N, numColsSource);
    
        cudaDeviceSynchronize();
    
        jacobi<<<N/1024 + 1, 1024>>>(red, red2, d_source_red, d_dest_red, d_inside, d_mask, N, numColsSource);
        jacobi<<<N/1024 + 1, 1024>>>(blue, blue2, d_source_blue, d_dest_blue, d_inside, d_mask, N, numColsSource);
        jacobi<<<N/1024 + 1, 1024>>>(green, green2, d_source_green, d_dest_green, d_inside, d_mask, N, numColsSource);
    
        cudaDeviceSynchronize();
    }



    storeBack<<<N/1024 + 1, 1024 >>>(d_dest, red, blue, green, d_inside, d_mask , N);

    cudaDeviceSynchronize();
    
    checkCudaErrors(cudaMemcpy(h_blendedImg,d_dest,N* sizeof(uchar4), cudaMemcpyDeviceToHost));

    checkCudaErrors(cudaGetLastError());
}