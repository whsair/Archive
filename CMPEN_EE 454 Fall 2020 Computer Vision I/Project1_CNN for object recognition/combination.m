function combination()
    load 'cifar10testdata.mat'
    load 'CNNparameters.mat'
    
    d = size(imageset);
    disp(d);
    
    output = zeros(1,1,10,d(4));
    
    for i=1:2
        fprintf('image %d\n', i);
        outarray = double(imageset(:,:,:,i));
        
        %1 imnormalize 
        outarray = apply_imnormalize(outarray);
        %debugging(outarray,1)
        %2 convolve 
        outarray = apply_convolve(outarray, filterbanks{2}, biasvectors{2});
        %debugging(outarray,2)
        %3 relu 
        outarray = apply_relu(outarray);
        %debugging(outarray,3)
        %4 convolve 
        outarray = apply_convolve(outarray, filterbanks{4}, biasvectors{4});
        %debugging(outarray,4)
        %5 relu
        outarray = apply_relu(outarray);
        %debugging(outarray,5)
        %6 maxpool
        outarray = apply_maxpool(outarray);
        %debugging(outarray,6)
        %7 convolve 
        outarray = apply_convolve(outarray, filterbanks{7}, biasvectors{7});
        %debugging(outarray,7)
        %8 relu 
        outarray = apply_relu(outarray);
        %debugging(outarray,8)
        %9 convolve 
        outarray = apply_convolve(outarray, filterbanks{9}, biasvectors{9});
        %debugging(outarray,9)
        %10 relu 
        outarray = apply_relu(outarray);
        %debugging(outarray,10)
        %11 maxpool 
        outarray = apply_maxpool(outarray);
        %debugging(outarray,11)
        %12 convolve 
        outarray = apply_convolve(outarray, filterbanks{12}, biasvectors{12});
        %debugging(outarray,12)
        %13 relu
        outarray = apply_relu(outarray);
        %debugging(outarray,13)
        %14 convolve 
        outarray = apply_convolve(outarray, filterbanks{14}, biasvectors{14});
        %debugging(outarray,14)
        %15 relu 
        outarray = apply_relu(outarray);
        %debugging(outarray,15)
        %16 maxpool
        outarray = apply_maxpool(outarray);
        %debugging(outarray,16)
        %17 fullconnect 
        outarray = apply_fullconnect(outarray, filterbanks{17}, biasvectors{17});
        %debugging(outarray,17)
        %18 softmax 
        outarray = apply_softmax(outarray);
        %debugging(outarray,18)
        output(1,1,1:10,i) = outarray(:,:);
        
        
    end
    
    %get the confusion table
    cm = confusionMatrix(output, d(4));
    %plot the k classes accuracy
    Plot = kPlot(cm);
    %save confusion matrix
    %clearvars -except result cm plot
    %save('output.mat');
end
         
        
        
    
  