# clMussels

The repository contains a OpenCL code that runs the model describing spatial self-organization in mussel beds, as described in:

Van de Koppel, J., Rietkerk, M., Dankers, M. & Herman, P.M.J. 2005 Scale-dependent feedback and regular spatial patterns in young mussel beds. The American Naturalist 165:E66-E77.

Running the code requires an OpenCL-compatible graphics card and with OpenCL drivers installed. With a Mac, you should be able to run it without much tinkering from Xcode, which has OpenCL installed natively. The code has been build and tested using a Macbook pro 2012 edition. I have also tested the code using the NVIdia CUDA drivers, which also include OpenCL. I then run it using the cmake package (www.cmake.org) that handles the generation of a suitable makefile on the Mac.

For Windows users:
If you run under Windows, you have to install OpenCL (for AMD/INTEL cards) or CUDA with the OpenCL SDK samples (for Nvidia cards) within Visual Studio, and integrate the code files within a Visual studio project. The simplest way to do that is to copy one of the code samples into a new folder within the samples folder, and use an existing sample project file by replacing the code files. I cannot help you with this as I am a Mac user (I actually switched to Mac to avoid the Visual Studio headache, so be warned).

For Linux users: 
Presuming that have OpenCL installed, unzip the file somewhere, and open a command window in the build folder within the unzipped code folder. Now, run:

cmake ..
clear; make
clear; make run

If you want to delete the build, type 
clear; make destroy

The code itself does not show anything except for progress bar, and saves the data to a binary file. To view the results, you have to use the PlotMussel.r R-project file (using R and RStudio I advice) or PlotMussel.m within Matlab. Note to set the folder settings correctly and to install the “fields” package within R [ install.package(“fields”) ]

Both scripts can make a movie. The R script is best for that, but you have to install the ffmpeg program, which you can find here: https://www.ffmpeg.org/ 

I hope the code it useful for you. Please drop me an email if you like to tell whether it worked for you or not.


