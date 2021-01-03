% Calculate perpendicularline u3 from two camera rays
function u3 = getU_3(cameraRay2, cameraRay4)
    u3 = cross(cameraRay2,cameraRay4) / norm(cross(cameraRay2,cameraRay4));
end