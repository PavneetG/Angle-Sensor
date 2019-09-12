

if ~isempty(instrfind)%find port
     fclose(instrfind);
      delete(instrfind);
end

clc;
s = serial('COM3');     %choose COM port
set(s, 'Baudrate', 19200);   %sets the Baudrate 
s.Terminator = 'CR';      %define terminator 
fopen(s);   
i = 0;
lHandle = line(nan, nan);

while (1)    
   % figure(1);
   % hold on;
    i = i+10e-2;        %increment x-axis
    x = get(lHandle, 'XData');  % x-axis 
    y = get(lHandle, 'YData');  % y-axis 
    
    x = [x i];      % store X axis value
    output = fscanf(s);  
    output = str2double(output); 
    y = [y output];  % store data in y-axis
    
    set(lHandle, 'XData', x, 'YData', y);   %create plot
    drawnow;    %show plot

end

fclose(s);  %close the port
delete(s);  
clear s;