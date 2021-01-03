function outarray = apply_convolve(inarray, filterbank, biasvals)
    in = double(inarray);
    filter_bank = double(filterbank);
    v_bias = double(biasvals);
    
    
    d2 = size(filter_bank,4);
    [n,m,d1] = size(in);
    out = zeros(n,m,d2);
    
    for l=1:d2
        sum = double(zeros(n,m,1));
        for k=1:d1
            t = imfilter(in(:,:,k),filter_bank(:,:,k,l),0,'conv','same');
            sum = sum + t;
        end
        out(:,:,l)=sum + v_bias(l);
    end
    outarray = out;
end




