clear all; clc;
on=1;off=0;

Movie   = off;
PlotAll = off;

TitleFontSize = 24;
BarFontSize   = 18;

FID=fopen('Mussels.dat', 'r');

X=fread(FID,1,'int32');
Y=fread(FID,1,'int32');
Z=fread(FID,1,'int32');
Length=fread(FID,1,'float');
EndTime=fread(FID,1,'int32');

ArrayMemSize=X*Y*Z;

A=zeros(X,Y,'double');
M=zeros(X,Y,'double');

if Movie==on,
    writerObj = VideoWriter('Mussels in the flow.mp4', 'MPEG-4');
    open(writerObj);
end;

% Get Screen dimensions and set Main Window Dimensions
x = get(0,'ScreenSize'); ScreenDim=x(3:4);
if PlotAll==on;
    MainWindowDim=[1280 720];
else
    MainWindowDim=[960 720];
end;

% The graph window is initiated, with specified dimensions.
Figure1=figure('Name','Mussel bed development (Van de Koppel et al 2005', ...
               'Position',[(ScreenDim-MainWindowDim)/2 MainWindowDim], ...
               'Color','white');   

if PlotAll==on,
    subplot('position',[0.03 0.075 0.45 0.85]);
else
    subplot('position',[0.07 0.075 0.9 0.85]);
end;

F1=imagesc(M(1:X,1:Y)',[0 1500]);
title('Mussel density (g/m^2)','FontSize',TitleFontSize);  
colorbar('SouthOutside','FontSize',BarFontSize);
colormap('default'); axis image; axis ij; axis off;

if PlotAll==on,
    subplot('position',[0.52 0.075 0.45 0.85]);    
    F2=imagesc(A(1:X,1:Y)',[0 1.500]);
    title('Algal concentration (mg/l)','FontSize',TitleFontSize);  
    colorbar('SouthOutside','FontSize',BarFontSize);
    colormap('default'); axis image; axis ij; axis off;
end;
    

for i=1:Z-1,  
    
    A=reshape(fread(FID,X*Y,'float32'),X,Y);
    M=reshape(fread(FID,X*Y,'float32'),X,Y);   

    set(F1,'CData',M');
    if PlotAll==on, set(F2,'CData',A');end;
    set(Figure1,'Name',['Timestep ' num2str((i+1)/Z*EndTime/24,'%1.0f') ' of ' num2str(EndTime/24) ' days']); 

    drawnow; 

    if Movie==on,
         frame = getframe(Figure1);
         writeVideo(writerObj,frame);
    end

end

fclose(FID);

if Movie==on,
    close(writerObj);
end;

disp('Done');beep;
