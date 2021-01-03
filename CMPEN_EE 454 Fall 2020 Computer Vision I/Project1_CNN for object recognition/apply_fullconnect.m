function outarray = apply_fullconnect(inarray, filterbank, biasvals)
    in = inarray;
    filter_bank = filterbank;
    v_bias = biasvals;
    
   
    out = zeros(1,1,10);
    [n,m,d1,d2] = size(filter_bank);
    
    for l=1:d2
        t = 0;
        for i=1:n
            for j=1:m
                for k=1:d1
                    t = t + imfilter(in(i,j,k),filter_bank(i,j,k,l),'conv','same');
                end
            end
        end
   
        out(1,1,l) = t + v_bias(l);
    end
    outarray = out;
end 
 
        
        
    
    
    
    



