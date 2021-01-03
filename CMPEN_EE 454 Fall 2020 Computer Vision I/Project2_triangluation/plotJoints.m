function plotJoints(vid2Frame, pixel)
    figure();
    image(vid2Frame);
    hold on;
    plot(pixel(1,:),pixel(2,:), '.');
    hold off;
end