function Plot = kPlot(cm)
    %clear sum;
    accuracy = sum(diag(cm))/sum(sum(cm));
    kp = zeros(1,10);
    kp(1) = accuracy;
    kp(10) = 1.00;
    for i = 2:9
        n = 0;
        for j = 1:10
            n = n + sum(maxk(cm(:,j), i));
        end
        kp(i) = n/sum(sum(cm));
    end
    %general x-axis for the plot
    x = [1,2,3,4,5,6,7,8,9,10];
    %plot the graph
    figure;
    %graph the plot
    Plot = plot(x,kp,'-o','MarkerIndices',1:1:length(kp));
    title('top-k classes');
    xlabel('k value');
    ylabel('percentage');
    disp(Plot);
end