//
//  clMussels model settings and parameters
//
//  Created by Johan Van de Koppel on 03-09-14.
//  Copyright (c) 2014 Johan Van de Koppel. All rights reserved.
//

#ifndef SETTINGS_AND_PARAMETERS_H
#define SETTINGS_AND_PARAMETERS_H

// Compiler directives
#define ON              1
#define OFF             0

// --- Definitions for SpatialFunctions.cl -------------------------------------
#define Forward   1
#define Backward  2
#define Central   3

#define DifferenceScheme Backward

// --- GPU Settings ------------------------------------------------------------

#define Device_No         1 // 0:CPU; 1:Integrated GPU; 2:Discrete GPU (for mac)
#define WorkGroupSize     16
#define DomainSize        512
#define Print_All_Devices OFF

// Thread block size
#define Block_Size_X	(WorkGroupSize)
#define Block_Size_Y	(WorkGroupSize)

// Number of blox
// I define the Block_Number of the matrix as product of two numbers
// Makes it easier to keep them a multiple of something (16, 32)
#define Block_Number_X	(DomainSize/WorkGroupSize)
#define Block_Number_Y	(DomainSize/WorkGroupSize)

// Matrix Block_Number_ensions
// (chosen as multiples of the thread block size for simplicity)
#define Grid_Width  (Block_Size_X * Block_Number_X)		// Matrix width
#define Grid_Height (Block_Size_Y * Block_Number_Y)		// Matrix height
#define Grid_Size   (Grid_Width*Grid_Height)            // Total matrix size

// --- Model parameters --------------------------------------------------------

//      Parameters		Original value   Explanation and Units
#define Phi	1000.0		//  1            - Speeding constant

#define D	0.0005		//  0.0005       - m2/h  The diffusion constant describing the movement of mussels
#define V	0.1*60*60	//  0.1*60*60    - The advection constant describing the flow of algae with the water

#define Aup	1.0			//  1.00         - g/m3     Algal concentration in upper layer; Oosterschelde data
#define hh	0.10		//  0.10         - m        Height of the lower layer; defined
#define ff	100			//  100          - m3/m3/h  eXchange rate with upper layer; Guestimated
#define	cc	0.1			//  0.1          - g/g/h    Maximal consumption rate of the mussels; Riisgard 2001
#define	ee	0.200		//  0.2          - g/g      Trophic efficiency of mussels; Lamie
#define	dM	0.02		//  0.02         - g/g/h    Density dependent mortality rate of the mussels; Calibrated
#define	kM	150			//  150          - g/m2     Effect of density on mortality; Guestimated

#define dX	0.1
#define dY	0.1

// --- Simulation parameters ---------------------------------------------------

#define dT        0.00025       // 0.0001 - The timestep of the simulation
#define RealTime  (24*180)      // 24*180 - hours     The time at which the simulation ends
#define EndTime   (float)RealTime/(float)Phi // The simulation time, with EX acceleration
#define NumFrames 300           // Number of times during the simulation that the data is stored
#define	MAX_STORE (NumFrames+1) //
#define RandSeed  30            // an arbitraty number the sets a deterministic series of random starting valus

// --- Progress bar settings ---------------------------------------------------

#define ProgressBarWidth 45

// --- Name definitions --------------------------------------------------------
#define ALGAE	101
#define MUSSELS	102

#endif
