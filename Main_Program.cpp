//
//  clMussels main.cpp
//
//  Created by Johan Van de Koppel on 03-09-14.
//  Copyright (c) 2014 Johan Van de Koppel. All rights reserved.
//

#include <stdio.h>
#include <sys/time.h>
#include <iostream>
#include <math.h>

#include "Settings_and_Parameters.h"
#include "Device_Utilities.h"

#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

#define MAX_SOURCE_SIZE (0x100000)

// Forward definitions from functions at the end of this code file
void randomInit(float*, int, int, int);

////////////////////////////////////////////////////////////////////////////////
// Main program code for Mussels
////////////////////////////////////////////////////////////////////////////////

int main()
{
    
    /*----------Constant and variable definition------------------------------*/
    
	unsigned int Grid_Memory = sizeof(float) * Grid_Size;
	unsigned int size_storegrid = Grid_Width * Grid_Height * MAX_STORE;
	unsigned int mem_size_storegrid = sizeof(float) * size_storegrid;
    
    /*----------Defining and allocating memeory on host-----------------------*/
    
    // Defining and allocating the memory blocks for P, W, and O on the host (h)
    float* h_A = (float *)malloc(Grid_Width*Grid_Height*sizeof(float));
	float* h_M = (float *)malloc(Grid_Width*Grid_Height*sizeof(float));
	
    // Defining and allocating storage blocks for P, W, and O on the host (h)
    float* h_store_popA=(float*) malloc(mem_size_storegrid);
	float* h_store_popM=(float*) malloc(mem_size_storegrid);
    
    /*----------Initializing the host arrays----------------------------------*/
    
    srand(50); // Seeding the random number generator
    
	randomInit(h_A, Grid_Width, Grid_Height, ALGAE);
	randomInit(h_M, Grid_Width, Grid_Height, MUSSELS);
    
    /*----------Printing info to the screen ----------------------------------*/

	//system("clear");
    printf("\n");
	printf(" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
	printf(" * Mussel bed Patterns                                   * \n");
	printf(" * OpenCL implementation : Johan van de Koppel, 2014     * \n");
	printf(" * Following a model by Van de Koppel et al 2005         * \n");
	printf(" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n");
    
	printf(" Current grid dimensions: %d x %d cells\n\n",
           Grid_Width, Grid_Height);
    
    /*----------Setting up the device and the kernel--------------------------*/
    
    cl_device_id* devices;
    cl_int err;
    
    cl_context context = CreateGPUcontext(devices);
    
    // Print the name of the device that is used
    printf(" Implementing PDE on device %d: ", Device_No);
    print_device_info(devices, (int)Device_No);
    printf("\n");
    
    // Create a command queue on the device
    cl_command_queue command_queue = clCreateCommandQueue(context, devices[Device_No], 0, &err);
    
    /* Create Buffer Objects */
	cl_mem d_A = clCreateBuffer(context, CL_MEM_READ_WRITE, Grid_Memory, NULL, &err);
	cl_mem d_M = clCreateBuffer(context, CL_MEM_READ_WRITE, Grid_Memory, NULL, &err);
    
	/* Copy input data to the memory buffer */
	err = clEnqueueWriteBuffer(command_queue, d_A, CL_TRUE, 0, Grid_Width*Grid_Height*sizeof(float), h_A, 0, NULL, NULL);
	err = clEnqueueWriteBuffer(command_queue, d_M, CL_TRUE, 0, Grid_Width*Grid_Height*sizeof(float), h_M, 0, NULL, NULL);

    /*----------Building the PDE kernel---------------------------------------*/
    
    cl_program program = BuildKernelFile("Computing_Kernel.cl", context, &devices[Device_No], &err);
    if (err!=0)  printf(" > Compile Program Error number: %d \n\n", err);
    
    /* Create OpenCL kernel */
    cl_kernel kernel = clCreateKernel(program, "MusselsKernel", &err);
    if (err!=0) printf(" > Create Kernel Error number: %d \n\n", err);
    
	/* Set OpenCL kernel arguments */
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_A);
	err = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&d_M);
    
    /*----------Pre-simulation settings---------------------------------------*/

    /* create and start timer */
    struct timeval Time_Measured;
    gettimeofday(&Time_Measured, NULL);
    double Time_Begin=Time_Measured.tv_sec+(Time_Measured.tv_usec/1000000.0);

    /* Progress bar initiation */
    int RealBarWidth=std::min((int)NumFrames,(int)ProgressBarWidth);
    int BarCounter=0;
    float BarThresholds[RealBarWidth];
    for (int i=0;i<RealBarWidth;i++) {BarThresholds[i] = (float)(i+1)/RealBarWidth*NumFrames;};
    
    /* Print the reference bar */
    printf(" Progress: [");
    for (int i=0;i<RealBarWidth;i++) { printf("-"); }
    printf("]\n");
    fprintf(stderr, "           >");
    
    float EndTime_EX =(float)EndTime/(float)EX;
    
    /*----------Kernel parameterization---------------------------------------*/
    
    #if SetGrid2D==ON
        size_t global_item_size[] = {Grid_Width, Grid_Height};
        size_t local_item_size[] = {Block_Size_X, Block_Size_Y};
    #else
        size_t global_item_size[] = {Grid_Width*Grid_Height};
        size_t local_item_size[] = {Block_Size_X*Block_Size_Y};
    #endif

	for (int Counter=0;Counter<NumFrames;Counter++)
    {
        for (int Runtime=0;Runtime<floor(EndTime_EX/NumFrames/dT);Runtime++)
        {
            /* Execute OpenCL kernel as data parallel */
            err = clEnqueueNDRangeKernel(command_queue, kernel, (cl_uint)(SetGrid2D+1), NULL,
                                         global_item_size, local_item_size, 0, NULL, NULL);

            if (err!=0) { printf(" > Kernel Error number: %d \n\n", err); exit(-10);}
        }
        
        /* Transfer result to host */
        err = clEnqueueReadBuffer(command_queue, d_A, CL_TRUE, 0, Grid_Width*Grid_Height*sizeof(float), h_A, 0, NULL, NULL);
        err = clEnqueueReadBuffer(command_queue, d_M, CL_TRUE, 0, Grid_Width*Grid_Height*sizeof(float), h_M, 0, NULL, NULL);

        //printf("Read Buffer Error: %d\n\n", err);
        
        //Store values at this frame.
        memcpy(h_store_popA+(Counter*Grid_Size),h_A,Grid_Memory);
        memcpy(h_store_popM+(Counter*Grid_Size),h_M,Grid_Memory);
        
        // Progress the progress bar if time
        if ((float)(Counter+1)>=BarThresholds[BarCounter]) {
            fprintf(stderr,"*");
            BarCounter = BarCounter+1;}
            
    }
    
    printf("<\n\n");
    
    /*---------------------Report on time spending----------------------------*/
    gettimeofday(&Time_Measured, NULL);
    double Time_End=Time_Measured.tv_sec+(Time_Measured.tv_usec/1000000.0);
	printf(" Processing time: %4.3f (s) \n", Time_End-Time_Begin);
    
    /*---------------------Write to file now----------------------------------*/
    
    // The location of the code is obtain from the __FILE__ macro
    const std::string SourcePath (__FILE__);
    const std::string PathName = SourcePath.substr (0,SourcePath.find_last_of("/")+1);
    const std::string DataPath = PathName + "Mussels.dat";
    
	FILE * fp=fopen(DataPath.c_str(),"wb");

    int width_matrix = Grid_Width;
    int height_matrix = Grid_Height;
    int NumStored = NumFrames;
    float Length = dX*(float)Grid_Width;
    int EndTimeVal = EndTime;

	// Storing parameters
	fwrite(&width_matrix,sizeof(int),1,fp);
	fwrite(&height_matrix,sizeof(int),1,fp);
	fwrite(&NumStored,sizeof(int),1,fp);
    fwrite(&Length,sizeof(float),1,fp);
	fwrite(&EndTimeVal,sizeof(int),1,fp);
	
	for(int store_i=0;store_i<NumFrames;store_i++)
    {
		fwrite(&h_store_popA[store_i*Grid_Size],sizeof(float),Grid_Size,fp);
		fwrite(&h_store_popM[store_i*Grid_Size],sizeof(float),Grid_Size,fp);
    }
	
	printf("\r Simulation results saved! \n\n");
    
	fclose(fp);
    
	/*---------------------Clean up memory------------------------------------*/
	
    // Freeing host space
    free(h_A);
	free(h_M);
    
	free(h_store_popA);
	free(h_store_popM);
 
	// Freeing kernel and block space
	err = clFlush(command_queue);
	err = clFinish(command_queue);
	err = clReleaseKernel(kernel);
	err = clReleaseProgram(program);
	err = clReleaseMemObject(d_A);
	err = clReleaseMemObject(d_M);
	err = clReleaseCommandQueue(command_queue);
	err = clReleaseContext(context);
    free(devices);
    
    #if defined(__APPLE__) && defined(__MACH__)
        //system("say Simulation finished");
    #endif

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// Allocates a matrix with random float entries
////////////////////////////////////////////////////////////////////////////////

void randomInit (float* data, int x_siz, int y_siz, int type)
{
    int i,j;
    for(i=0;i<y_siz;i++)
    {
        for(j=0;j<x_siz;j++)
 		{
 			//assigning the first row last row and
			//first column last column as zeroes
 			
			if(i==0||i==y_siz-1||j==0||j==x_siz-1)
				data[i*y_siz+j]=0.5f;
			else
            {
				//for every other element find the correct initial
				//value using the conditions below
				if(type==MUSSELS)
                {
					//printf(" %4.5f ",(rand() / (float)RAND_MAX));
					if((rand() / (float)RAND_MAX)<0.10f)
                        data[i*y_siz+j] = (float)775.0f;
                    else
                        data[i*y_siz+j] = (float)25.0f;
					
                }
				else if(type==ALGAE)
					data[i*y_siz+j]=0.6f;
            }
		}
    }
} // End randomInit
