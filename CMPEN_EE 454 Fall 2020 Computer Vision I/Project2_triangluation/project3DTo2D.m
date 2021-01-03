function projected2DPoints  = project3DTo2D(cam, worldCoord3DPoints)
    %compute to get [x'; y'; z']
    if size(worldCoord3DPoints,1) == 3
        worldCoord3DPoints = [worldCoord3DPoints;ones(1,size(worldCoord3DPoints,2))];
    end
    %compute for x = x'/z', y = y'/z' for each camera
    %x = CameraPoint(1) ./ CameraPoint(3);
    %y = CameraPoint(2) ./ CameraPoint(3);
    %z = CameraPoint(3) ./ CameraPoint(3);
    CameraPoint = cam.Kmat * cam.Pmat * worldCoord3DPoints;
    projected2DPoints = CameraPoint ./ CameraPoint(3,:);
end