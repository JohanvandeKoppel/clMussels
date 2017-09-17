#include "Settings_and_Parameters.h"
#include "SpatialFunctions.cl"

////////////////////////////////////////////////////////////////////////////////
// Simulation kernel
////////////////////////////////////////////////////////////////////////////////

__kernel void SimulationKernel (__global float* A, __global float* M)
{
    const size_t current  = get_global_id(0);
    const size_t row      = floor((float)current/(float)Grid_Width);
    const size_t column   = current%Grid_Width;
	
	if(row > 0 && row < Grid_Height-1 && column > 0 && column < Grid_Width-1)
    {
		float Consumption = cc * A[current] * M[current];
        
        float drA = ff*(Aup-A[current]) - Consumption/hh - V*d_dy(A);
        float drM = ee*Consumption - dM*M[current]*kM/(kM+M[current]) + D*d2_dxy2(M);
        
        A[current]=A[current] + drA*dT;
        M[current]=M[current] + drM*dT*Phi;
    }
    
	// HANDLE Boundaries
    else if( row==0 || row==Grid_Height-1)
    {
        PeriodicBoundaries(A);
        PeriodicBoundaries(M);
    }
    else if( column == 0 || column == Grid_Width-1)
    {
        NeumannBoundaries(A);
        NeumannBoundaries(M);
    }
	
} // End Simulationkernel
