%Simple Background Subtraction
%{
loop time t
I(t) = next frame;
diff = abs[B – I(t)];
M(t) = threshold(diff,l);

%}

function output = sbs(I,B,l) 
    diff = abs(B - I); %get absolute diff between background and current frame
    diff = threshold(diff, l); %thresholding (using l = threshold)
    output = diff; %return output
    
end