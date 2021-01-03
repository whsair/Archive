function cfmx = confusionMatrix(results,length)
    load cifar10testdata.mat
    
    % Define final confusion matrix
    cfmx = zeros(10,10);

    for imageIndex = 1:length
        im = results(:,:,1:10,imageIndex);
        cfmx(im == max(im),trueclass(imageIndex)) = cfmx(im == max(im),trueclass(imageIndex))+1;
    end
    clearvars -except result cm plot
    save('output.mat');
    display(cfmx)
end