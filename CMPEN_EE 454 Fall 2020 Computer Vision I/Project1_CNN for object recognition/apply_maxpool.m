function outarray = apply_maxpool(inarray)
    %Out(i; j;k) = max(f In(r;c;k) j (2i􀀀1) < r < 2i and (2 j􀀀1) < c < 2 j g)
    %reference: https://www.mathworks.com/matlabcentral/answers/409032-how-do-i-compute-the-maxpool-of-a-image-let-us-say-stride-of-2-2-on-a-mxn-matrix
    input = inarray;
    [N,M,D] = size(input);
    
    n = N/2;
    m = M/2;
    
    out = ones(n, m, D);
    
    for i = 1:D
        
        cc = input(:,:,i);
        
        a11 = cc(1:2:end,1:2:end);
        a12 = cc(1:2:end,2:2:end);
        a21 = cc(2:2:end,1:2:end);
        a22 = cc(2:2:end,2:2:end);
        
        
        
        out(:,:,i) = max(max(a11,a12),max(a21,a22));
    end
    outarray = out;
end


