%get Triangulation form from camera rays,perpendicularline and positions 
function [a,d,b] = triangula(cameraRay2,cameraRay4,perpendicularline,position2,position4)
    matrix = [cameraRay2  perpendicularline -cameraRay4];
    vector = matrix\(position4 - position2);
    a = vector(1);
    d = vector(2);
    b = vector(3);
end