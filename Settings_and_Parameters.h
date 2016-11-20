//
//  clMussels.cpp
//
//  Created by Johan Van de Koppel on 03-09-14.
//  Copyright (c) 2014 Johan Van de Koppel. All rights reserved.
//

// Compiler directives
#define ON              1
#define OFF             0

#define Print_All_Devices OFF

#define Device_No       1   // 0: CPU; 1: Intel 4000; 2: Nvidia GT 650M
#define ProgressBarWidth 45

#define WorkGroupSize   16
#define DomainSize      2048

// Thread block size
#define Block_Size_X	(WorkGroupSize)
#define Block_Size_Y	(WorkGroupSize)

// Number of blox
/* I define the Block_Number_ensions of the matrix as product of two numbers
Makes it easier to keep them a multiple of something (16, 32) when using CUDA*/
#define Block_Number_X	(DomainSize/WorkGroupSize)
#define Block_Number_Y	(DomainSize/WorkGroupSize)

// Matrix Block_Number_ensions
// (chosen as multiples of the thread block size for simplicity)
#define Grid_Width  (Block_Size_X * Block_Number_X)			// Matrix A width
#define Grid_Height (Block_Size_Y * Block_Number_Y)			// Matrix A height
#define Grid_Size (Grid_Width*Grid_Height)                  // Grid Size

// DIVIDE_INTO(x/y) for integers, used to determine # of blocks/warps etc.
#define DIVIDE_INTO(x,y) (((x) + (y) - 1)/(y))

//      Parameters		Original value    Explanation and Units
#define EX	1000.0		//  1            - Speeding constant

#define D	0.0005		//  0.0005       - m2/h  The diffusion constant describing the movement of mussels
#define V	0.1*60*60	//  0.1*60*60    - The advection constant describing the flow of algae with the water

#define Aup	1.1			//  1.00         - g/m3     Algal concentration in upper layer; Oosterschelde data
#define hh	0.10		//  0.10         - m        Height of the lower layer; defined
#define ff	100			//  100          - m3/m3/h  eXchange rate with upper layer; Guestimated
#define	cc	0.1			//  0.1          - g/g/h    Maximal consumption rate of the mussels; Riisgard 2001
#define	ee	0.200		//  0.2          - g/g      Trophic efficiency of mussels; Lamie
#define	dM	0.02		//  0.02         - g/g/h    Density dependent mortality rate of the mussels; Calibrated
#define	kM	150			//  150          - g/m2     Effect of density on mortality; Guestimated

#define dX	0.05
#define dY	0.05

#define Time      0             // 0      - Start time of the simulation
#define dT        0.0001        // 0.0001 - The timestep of the simulation
#define RealTime  (24*180)      // 24*180 - hours     The time at which the simulation ends
#define EndTime   (float)RealTime/(float)EX  // The simulation time, with EX acceleration
#define NumFrames 300           // Number of times during the simulation that the data is stored
#define	MAX_STORE (NumFrames+1) //

// Name definitions
#define MUSSELS	101
#define ALGAE	102


