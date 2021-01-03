function round_debugging(filteredImage, D)
    load 'debuggingTest.mat'
    
    
    
    if isequal(round(filteredImage,8), round(layerResults{D},8))
        fprintf('%i passes\n',D);
    else
        fprintf('%i ERROR\n',D);
    end
    
    
end