%The above function will find the epipolar lines and output them as column vectors.

%cam1 & cam2 are the camera parameters provided to you (i.e., vue2 and vue4). cam1 & cam2 are a struct.
%cam1PixelCoords & cam2PixelCoords are the 2D pixel coordinates of the 3D points that are got by projecting 3D world coordinates to the 2D. 
%cam1PixelCoords & cam2PixelCoords are a matrix of size 3xN, where N represents the number of points input to the function.
%EpipolarLines1, EpipolarLines2 are epipolar lines for cam1 and cam2. 
%Lines are represented as 0 = ax + by + c. So the output EpipolarLines are column vectors [a, b, c]'. 

%You can send one point at a time or all the 12 joints image coordinates at a time so that you get all lines in one go
%(this is highly recommended as it used vectorized format and is faster), the design choice is still yours. 
%Ensure that your function works for a variable-length input.
% i.e, if one world point is sent into the function, it should output 1 epipolar line for each camera and
%if 12 points are sent, it should output 12 lines for each camera. 

%Since there are multiple ways to implement it, you can use whichever parameters you require to find the epipolar line.
%But you will have to accept it as a parameter and you can choose to not use it in your function implementation.
%The functions definition does not change.
function [EpipolarLines1, EpipolarLines2] = findEpipolarLines(worldCoord3DPoints, cam1, cam1PixelCoords, cam2, cam2PixelCoords)
    [~, n] = size(cam2PixelCoords);
    
    cam1_coords = [cam2.position]';
    cam2_coords = [cam1.position]';
    cam1_epipole = project3DTo2D(cam2, cam2_coords);
    cam2_epipole = project3DTo2D(cam1, cam1_coords);
    
    for i = 1:1:n
        cam1_slope = (cam2PixelCoords(2,i)-cam1_epipole(2))/(cam2PixelCoords(1,i)-cam1_epipole(1));
        cam2_slope = (cam1PixelCoords(2,i)-cam2_epipole(2))/(cam1PixelCoords(1,i)-cam2_epipole(1));
        %y = kx + b => b = -kx + y => [k, -1, -kx + y] = [a, b, c]
        EpipolarLines1(:,i) = [cam1_slope; -1;-(cam1_slope*cam2PixelCoords(1,i))+cam2PixelCoords(2,i)];
        EpipolarLines2(:,i) = [cam2_slope; -1;-(cam2_slope*cam1PixelCoords(1,i))+cam1PixelCoords(2,i)];
    end
end