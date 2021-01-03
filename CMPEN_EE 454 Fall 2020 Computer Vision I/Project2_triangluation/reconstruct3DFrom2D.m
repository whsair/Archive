function recovered3DPoints = reconstruct3DFrom2D(cam1, cam1PixelCoords, cam2, cam2PixelCoords)
    recovered3DPoints = zeros(3,12);
    for j = 1:12
        %get camera rays
        cam_r2 = backwarding(cam1PixelCoords(:,j), cam1.Kmat, cam1.Rmat);
        cam_r4 = backwarding(cam2PixelCoords(:,j), cam2.Kmat, cam2.Rmat);
        %get p (2.4)
        u3 = getU_3(cam_r2, cam_r4);
        [i,d,b] = triangula(cam_r2,cam_r4,u3,(cam1.position(1:3))',(cam2.position(1:3))');
        p = getP(cam_r2,cam_r4,i,b,(cam1.position(1:3))',(cam2.position(1:3))');
        recovered3DPoints(:,j) = p; 
    end
end