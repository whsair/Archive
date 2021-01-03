% Debugging check for CNN layers
function debugging(filteredImage, D)
    load 'debuggingTest.mat'
    
    
    if isequal(filteredImage, layerResults{D})
        fprintf('%i passes\n',D);
    else
        fprintf('%i ERROR\n',D);
    end
    
    
end