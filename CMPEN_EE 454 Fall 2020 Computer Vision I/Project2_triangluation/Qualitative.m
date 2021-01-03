function Qualitative(vid2Frame, pixel)
    figure();
    imagesc(vid2Frame)
    hold on;
    %plot each points in the frame
    plot(pixel(1,:),pixel(2,:), '.') 
    %plot lines from point to point
    plot([pixel(1, 1);pixel(1, 2)], [pixel(2, 1); pixel(2, 2)], '-r'); 
    plot([pixel(1, 2);pixel(1, 3)], [pixel(2, 2); pixel(2, 3)], '-r'); 
    plot([pixel(1, 1);pixel(1, 4)], [pixel(2, 1); pixel(2, 4)], '-r'); 
    plot([pixel(1, 4);pixel(1, 5)], [pixel(2, 4); pixel(2, 5)], '-r'); 
    plot([pixel(1, 5);pixel(1, 6)], [pixel(2, 5); pixel(2, 6)], '-r'); 
    plot([pixel(1, 7);pixel(1, 8)], [pixel(2, 7); pixel(2, 8)], '-r'); 
    plot([pixel(1, 8);pixel(1, 9)], [pixel(2, 8); pixel(2, 9)], '-r'); 
    plot([pixel(1, 7);pixel(1, 10)], [pixel(2, 7); pixel(2, 10)], '-r'); 
    plot([pixel(1, 10);pixel(1, 11)], [pixel(2, 10); pixel(2, 11)], '-r'); 
    plot([pixel(1, 11);pixel(1, 12)], [pixel(2, 11); pixel(2, 12)], '-r');
    %plot shoulder mid to hip mid
    plot([(pixel(1, 1) + pixel(1, 4))/2;(pixel(1, 7) + pixel(1, 10))/2],...
        [(pixel(2, 1) + pixel(2, 4))/2; (pixel(2, 7) + pixel(2, 10))/2], '-r'); 

hold off;
end
   
