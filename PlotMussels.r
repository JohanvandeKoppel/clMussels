# First setup of the model
remove(list=ls()) # Remove all variables from memory

on=1;off=0;
setwd('/Simulations/OpenCL/clMussels/clMussels')

require(fields)

FileName = "Output.dat"
cat(paste("Data file date :",file.info(FileName )$mtime ),"\n")
FID = file(FileName, "rb")

Movie=off
Wait=off
WinWidth = 960
WinHeight = 720

NX =        readBin(FID, integer(), n = 1, endian = "little");
NY =        readBin(FID, integer(), n = 1, endian = "little");
Length =    readBin(FID, numeric(), size=4, n = 1, endian = "little")
NumFrames = readBin(FID, integer(), n = 1, endian = "little");
EndTime =   readBin(FID, integer(), n = 1, endian = "little")

DPI=144

UsedPalette=topo.colors(255)

if (Movie==off) 
  quartz(width=WinWidth/DPI, height=WinHeight/DPI, dpi=DPI)

for (jj in 0:(NumFrames-1)){  # Here the time loop starts 
  
   if (Movie==on)
      jpeg(filename = sprintf("Images/Rplot%03d.jpeg",jj),
           width = WinWidth, height = WinHeight, 
           units = "px", pointsize = 24,
           quality = 100,
           bg = "white", res = NA,
           type = "quartz")  
   
   Data_A = matrix(nrow=NY, ncol=NX, readBin(FID, numeric(), size=4, n = NX*NY, endian = "little"));
   Data_M = matrix(nrow=NY, ncol=NX, readBin(FID, numeric(), size=4, n = NX*NY, endian = "little"));
   
   par(mar=c(2, 3, 2, 5) + 0.1)
   
   image.plot(Data_M[,2:(NX-1)], zlim=c(0,1200), xaxt="n", yaxt="n",
              col = UsedPalette,asp=1, bty="n", useRaster=TRUE,
              legend.shrink = 0.99, legend.width = 2,
              legend.args=list(text=expression(Dry~weight~(g~m^-2)),
                               cex=0.8, line=0.5))  
   title("Mussel density")   
   
#   mtext(text=paste("Time : ",sprintf("%1.0f",jj),
#                    "of" ,sprintf("%1.0f",NumFrames), "Frames"), 
#      side=1, adj=0.5, line=0.5, cex=1)

   mtext(text=sprintf("Time : %1.0f of %1.0f Days", (jj+1)/NumFrames*EndTime/24, EndTime/24), 
         side=1, adj=0.5, line=0.5, cex=1)   
   
   # Adding the scale bar and text
   axis(side=1, at=c(0.8,1), line=0.5, labels = c(0,trunc(Length/5+0.5)), 
        cex.axis=0.8, tck = -0.015, mgp=c(3, .25, 0))
   
   mtext(text="Scale (m)", side=1, adj=1.2, line=0.5, cex=0.8)
   
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

if (Movie==on) { 
  
  InFiles=paste(getwd(),"/Images/Rplot%03d.jpeg", sep="")
  OutFile="Mussels.mp4"
  
  print(paste(" building :", OutFile))
  
  CmdLine=sprintf("ffmpeg -y -r 30 -i %s -c:v libx264 -pix_fmt yuv420p -b:v 2000k %s", InFiles, OutFile)
  cmd = system(CmdLine)
  
  # if (cmd==0) try(system(paste("open ", paste(getwd(),"Mussels_PDE.mp4"))))
} 

system('say All ready')