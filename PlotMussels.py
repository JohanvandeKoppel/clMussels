#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jun  7 08:19:25 2017

@author: johank
"""

on=1;off=0;

# %% Loading required modules
import numpy as np
import matplotlib.pyplot as plt
import os, time
from matplotlib.colors import LinearSegmentedColormap

# %% Parameter settings
Movie=off

# %% Setting up the plot 
WinWidth = 1280
WinHeight = 720
DPI=144

MainTitleText='Musselbed pattern formation model'
TitleText_P1='Mussels M'
TitleText_P2='Algae A'
LabelText_P1 = '$g m^{-2}$'
LabelText_P2 = '$g m^{-3}$'

CodeFolder = '/Users/johank/Simulations/OpenCL/clMussels/clMussels'
MovieFileName = 'Mussels_2P.mp4'

# %% Colormap definitions
MusselColors = [(0.70, 0.67, 0.62), (0.96, 0.91, 0.93), (0.64, 0.64, 0.71),\
                (0.44, 0.48, 0.56), (0.20, 0.27, 0.28), (0.0 , 0.0 , 0.0)]  
ColorMap_P1 = LinearSegmentedColormap.from_list('AridVeg', MusselColors, N=100)

AlgaeColors = [(0.84, 1.0, 1.0), (0.25, 0.8, 0.5)] 
ColorMap_P2 = LinearSegmentedColormap.from_list('Water', AlgaeColors, N=100)

D2_Max = 1200  # Maximal biomass for the image plot & bar

# %% Importing the data header
FileName = "Output.dat"

f = open(FileName, 'rb') 
        
fTime = os.fstat(f.fileno()).st_mtime
print("\n Opening file %s" % FileName)
print(" last modified: %s\n" % time.ctime(fTime))

NX        = np.asscalar(np.fromfile(file=f, dtype= np.int32, count=1, sep=""))
NY        = np.asscalar(np.fromfile(file=f, dtype= np.int32, count=1, sep=""))
Length    = np.asscalar(np.fromfile(file=f, dtype= np.float32, count=1, sep=""))
NumFrames = np.asscalar(np.fromfile(file=f, dtype= np.int32, count=1, sep=""))
EndTime   = np.asscalar(np.fromfile(file=f, dtype= np.int32, count=1, sep="")) 

D1 = np.zeros((NX, NY))
D2 = np.zeros((NX, NY))

# %%Setting up the two panels for earch array

fig, (ax) = plt.subplots(1, 2, figsize=(WinWidth/DPI, WinHeight/DPI), dpi=DPI)
fig.canvas.set_window_title(MainTitleText)
 
mngr = plt.get_current_fig_manager()
#mngr.window.setGeometry(0,25,WinWidth, WinHeight)
plt.tight_layout(pad=3.0, w_pad=1.5, h_pad=1.0 )

text=fig.suptitle("Time: %1.0f of %1.0f " % (0, EndTime), 
            y=0.07, fontsize=12);

# Panel 1: 
im1=ax[0].imshow(D2, cmap=ColorMap_P1, clim=(0, D2_Max), extent=[0,Length,0,Length]);
ax[0].set_title(TitleText_P1, fontsize=15)
cb1=fig.colorbar(im1, ax=ax[0], fraction=0.046, pad=0.04)
cb1.ax.set_title(LabelText_P1)
ax[0].set_xticks([]) 

ax[0].set_ylabel('Scale (meters)')

# Panel 2: 
im2=ax[1].imshow(D1, cmap=ColorMap_P2, clim=(0, 1));
ax[1].set_title(TitleText_P2, fontsize=15)
cb2=fig.colorbar(im2, ax=ax[1], fraction=0.046, pad=0.04)
cb2.ax.set_title(LabelText_P2)
ax[1].set_xticks([]) 
ax[1].set_yticks([])  

# %% Plotting of the modelling results

for jj in range(NumFrames):  # Here the time loop starts 
   
        D1 = np.reshape(np.fromfile(file=f, dtype= np.float32, count=NX*NY, sep=""),(NX,NY))
        D2 = np.reshape(np.fromfile(file=f, dtype= np.float32, count=NX*NY, sep=""),(NX,NY))
        
        im1.set_data(D2)
        im2.set_data(D1)
        plt.draw()
        plt.pause(1e-10)
        text.set_text("Time: %1.0f of %1.0f" % ((jj+1)/NumFrames*EndTime/24, EndTime/24))
        ii=0;jj=jj+1
        if (Movie==on):plt.savefig("Images/Rplot%03d.png" % jj)

f.close() 

print("Finished!") 


# %% Saving the file
if (Movie==on): 
    os.chdir(CodeFolder)
    InFile = 'Images/Rplot%03d.png'
    OutFile = MovieFileName
    CommandLine='/usr/local/bin/ffmpeg -y -r 30 -i %s -c:v libx264 -pix_fmt yuv420p -b:v 2000k %s' % (InFile, OutFile)
    os.system(CommandLine)  
    
