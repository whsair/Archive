function plotEpline( EpipolarLines2, pixel2, vid2Frame)
    %let each color for the epipolar line different
    colormap = jet(12);
    figure();
    %Display image for input frame
    image(vid2Frame);
    hold on;
    %plot every joint in the frame
    for i = 1:1:12
        color = colormap(i,:);
        point = pixel2(:,i);
        %Display dot for current joint
        scatter(point(1),point(2),'filled','MarkerEdgeColor',color,'MarkerFaceColor',color);
        hold on
        %Plot epipolar line through all 12 joints in the frame
        epLine = EpipolarLines2(:,i);
        epipolarX = linspace(0, 2000, 100);
        epipolarY = (-epLine(1) * epipolarX - epLine(3))/epLine(2);
        plot(epipolarX,epipolarY, 'LineWidth', 1, 'Color', color);
        hold on
    end
    hold off;
end