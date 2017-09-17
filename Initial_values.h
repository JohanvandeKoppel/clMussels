//
//  Initial_values.cpp
//  clMussels
//
//  Created by Johan Van de Koppel on 09/09/15.
//  Copyright (c) 2015 Johan Van de Koppel. All rights reserved.
//

#include <stdio.h>

////////////////////////////////////////////////////////////////////////////////
// Allocates a matrix with random float entries
////////////////////////////////////////////////////////////////////////////////

void Initialize (float* data, size_t x_siz, size_t y_siz, int type)
{
    for(size_t i=0;i<y_siz;i++)
    {
        for(size_t j=0;j<x_siz;j++)
        {
            //for every other element find the correct initial
            //value using the conditions below
            if(type==MUSSELS)
              { data[i*y_siz+j] = ((rand() / (float)RAND_MAX)<0.1f)? 1200.0: 100.0; }
            else if(type==ALGAE)
              { data[i*y_siz+j] = 0.6; }

        }
    }
} // End Initialize

////////////////////////////////////////////////////////////////////////////////
// Prints the model name and additional info
////////////////////////////////////////////////////////////////////////////////

void Print_Label()
{
    //system("clear");
    printf("\n");
    printf(" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n");
    printf(" * Mussel bed Patterns                                   * \n");
    printf(" * OpenCL implementation : Johan van de Koppel, 2014     * \n");
    printf(" * Following a model by Van de Koppel et al 2005         * \n");
    printf(" * * * * * * * * * * * * * * * * * * * * * * * * * * * * * \n\n");
    
} // Print_Label
