#include "Settings_and_Parameters.h"

////////////////////////////////////////////////////////////////////////////////
// Laplacation operator definition, to calculate diffusive fluxes
////////////////////////////////////////////////////////////////////////////////

float d2_dxy2(__global float* pop, int row, int column)
{
    float retval;
    float dx = dX;
    float dy = dY;
    
    int current = row * Grid_Width + column;
    int left    = row * Grid_Width + column-1;
    int right   = row * Grid_Width + column+1;
    int top     = (row-1) * Grid_Width + column;
    int bottom  = (row+1) * Grid_Width + column;
    
    retval = ( ( pop[left] + pop[right]  - 2*pop[current] ) /dx/dx +
               ( pop[top]  + pop[bottom] - 2*pop[current] ) /dy/dy );
    
    return retval;
}

////////////////////////////////////////////////////////////////////////////////
// Gradient operator definition, to calculate advective fluxes
////////////////////////////////////////////////////////////////////////////////


float d_dy(__global float* pop, int row, int column)
{
	float retval;
	float dy = dY;
	
	int current=row * Grid_Width + column;
	int top=(row-1) * Grid_Width + column;
	
	retval =  (( pop[current] - pop[top] )/dy );
    
	return retval;
}

////////////////////////////////////////////////////////////////////////////////
// Simulation kernel
////////////////////////////////////////////////////////////////////////////////

__kernel void SimulationKernel (__global float* A, __global float* M)
{
    
	float Consumption;
	
    size_t current  = get_global_id(0);
    int    row      = floor((float)current/(float)Grid_Width);
    int    column   = current%Grid_Width;
	
	if (row > 0 && row < Grid_Width-1)
    {
		float Consumption = cc * A[current] * M[current];
        
		A[current]=A[current]+(ff*(Aup-A[current]) - Consumption/hh - V*d_dy(A, row, column))*dT;
		M[current]=M[current]+EX*(ee*Consumption - dM*M[current]*kM/(kM+M[current]) + D*d2_dxy2(M, row, column))*dT;
    }
    
    //barrier(CLK_LOCAL_MEM_FENCE);
    
	// HANDLE Boundaries
	if(row==0)
		//do copy of first row = second last row
    {
        A[current]=A[(Grid_Height-2)*Grid_Width+column];
        M[current]=M[(Grid_Height-2)*Grid_Width+column];
    }
    
	if(row==Grid_Height-1)
		//do copy of last row = second row
    {
        A[current]=A[1*Grid_Width+column];
        M[current]=M[1*Grid_Width+column];
    }
	
} // End Aridlandskernel

