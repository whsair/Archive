function showGraphs(outarray)
    for i = 1:10
        image = outarray(:,:,i);
        % normalize images between 0 and 1
        mini = min(min(image));
        maxi = max(max(image));
        graph = (image - mini)/(maxi-mini);
        % show the figure
        figure();
        colormap(gray);
        imagesc(graph);
    end
end