function demoe1()
    load 'cifar10testdata.mat'
    load 'CNNparameters.mat'
    
    outarray = double(imread('airplane5.png')); 
    %1 imnormalize 
    outarray = apply_imnormalize(outarray);
    
        %2 convolve 
        outarray = apply_convolve(outarray, filterbanks{2}, biasvectors{2});
        
        %3 relu 
        outarray = apply_relu(outarray);
        
        %4 convolve 
        outarray = apply_convolve(outarray, filterbanks{4}, biasvectors{4});
        
        %5 relu
        outarray = apply_relu(outarray);
       
        %6 maxpool
        outarray = apply_maxpool(outarray);
        
        %7 convolve 
        outarray = apply_convolve(outarray, filterbanks{7}, biasvectors{7});
        
        %8 relu 
        outarray = apply_relu(outarray);
        
        %9 convolve 
        outarray = apply_convolve(outarray, filterbanks{9}, biasvectors{9});
        
        %10 relu 
        outarray = apply_relu(outarray);
        
        %11 maxpool 
        outarray = apply_maxpool(outarray);
        
        %12 convolve 
        outarray = apply_convolve(outarray, filterbanks{12}, biasvectors{12});
        
        %13 relu
        outarray = apply_relu(outarray);
        
        %14 convolve 
        outarray = apply_convolve(outarray, filterbanks{14}, biasvectors{14});
        
        %15 relu 
        outarray = apply_relu(outarray);
        
        %16 maxpool
        outarray = apply_maxpool(outarray);
        
        %17 fullconnect 
        outarray = apply_fullconnect(outarray, filterbanks{17}, biasvectors{17});
        
        %18 softmax 
        outarray = apply_softmax(outarray);
        display(outarray);
        

end