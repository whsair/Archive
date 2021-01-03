%calculate to get the location of P point
function p = getP(cameraRay2,cameraRay4,a,b,position2,position4)
    p1 = position2 + a * cameraRay2;
    p2 = position4 + b * cameraRay4;
    p = (p1 + p2)/2.0;
end