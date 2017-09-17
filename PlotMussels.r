# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# Plotting script for the Opencl implementation of the                        #
# self-organization model of Van de Koppel et al 2005                         #
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

# First setup of the model
remove(list=ls()) # Remove all variables from memory
on=1;off=0;

# Change this to the folder where the code resides
setwd('~/Simulations/OpenCL/clMussels/clMussels')

require(fields)

# Settings for the plotting
Movie=off 
Wait=off

# --- Settings for this specific visualization --------------------------------
TitleText="Mussel density"
LabelText=expression(Dry~weight~(g~m^-2))
MovieFileName="Mussels_1P.mp4"

# The color palette used for the mussel panel
mussel.palette= colorRampPalette(c('#b2aa9e','#f4e8ed','#a3a3b5','#707a8e','#334447','#000000'))
UsedPalette=mussel.palette(255)

D2_Max = 1200  # Maximal biomass for the image plot & bar

# --- Graphics settings -------------------------------------------------------
WinWidth = 960
WinHeight = 720
DPI=144

# --- Opening the data file ---------------------------------------------------
FileName = "Output.dat"
cat(paste("Data file date :",file.info(FileName )$mtime ),"\n")
FID = file(FileName, "rb")

# Loading the simulation variables from the file
NX =        readBin(FID, integer(), n = 1, endian = "little");
NY =        readBin(FID, integer(), n = 1, endian = "little");
Length =    readBin(FID, numeric(), n = 1, size=4, endian = "little")
NumFrames = readBin(FID, integer(), n = 1, endian = "little");
EndTime =   readBin(FID, integer(), n = 1, endian = "little")

# Opening a window if not making a movie
if (Movie==off) 
  quartz(width=WinWidth/DPI, height=WinHeight/DPI, dpi=DPI)

for (jj in 0:(NumFrames-1)){  # Here the time loop starts 
  
   # Loading the matrices for one timestep
   D1 = matrix(nrow=NY, ncol=NX, readBin(FID, numeric(), size=4, n = NX*NY, endian = "little"));
   D2 = matrix(nrow=NY, ncol=NX, readBin(FID, numeric(), size=4, n = NX*NY, endian = "little"));
   
   # Opening a JPEG if making a movie
   if (Movie==on)
     png(filename = sprintf("Images/Rplot%03d.png",jj),
          width = WinWidth, height = WinHeight, 
          units = "px", pointsize = 24,
          bg = "white", res = NA,
          type = "quartz")  
   
   # --- Producing an image from the data arrays ------------------------------
   
   # Setting tight margins for the plot
   par(mar=c(2, 3, 2, 5) + 0.1)
   
   image.plot(pmin(D2,D2_Max), zlim=c(0,D2_Max), xaxt="n", yaxt="n",
              col = UsedPalette,asp=1, bty="n", useRaster=TRUE,
              legend.shrink = 0.99, legend.width = 2,
              legend.args=list(text=LabelText,
                               cex=0.8, line=0.5))  
   title(TitleText)   
   
   mtext(text=sprintf("Time : %1.0f of %1.0f Days", (jj+1)/NumFrames*EndTime/24, EndTime/24), 
         side=1, adj=0.5, line=0.5, cex=1)   
   
   # Adding the scale bar and text
   axis(side=1, at=c(0.8,1), line=0.5, labels = c(0,trunc(Length/5+0.5)), 
        cex.axis=0.8, tck = -0.015, mgp=c(3, .25, 0))
   
   mtext(text="Scale (m)", side=1, adj=1.2, line=0.5, cex=0.8)
   
   # CLose the image if making a movie, otherwise flush the graph
   if (Movie==on) dev.off() else { 
      dev.flush()
      dev.hold()
   }
   if (Wait==on){
      cat ("Press [enter] to continue, [q] to quit")
      line <- readline()
      if (line=='q'){ stop() }
   } 
}

close(FID)

# --- Here a movie is made, if Movie is on ------------------------------------

if (Movie==on) { 
  
  InFiles=paste(getwd(),"/Images/Rplot%03d.png", sep="")
  OutFile=MovieFileName;
  
  print(paste(" building :", OutFile))
  
  CmdLine=sprintf("ffmpeg -y -r 30 -i %s -c:v libx264 -pix_fmt yuv420p -b:v 2000k %s", InFiles, OutFile)
  cmd = system(CmdLine)
  
  # if (cmd==0) try(system(paste("open ", paste(getwd(),"Mussels_PDE.mp4"))))
} 

system('say All ready')