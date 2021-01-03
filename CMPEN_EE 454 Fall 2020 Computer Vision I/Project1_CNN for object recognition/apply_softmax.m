function outarray = apply_softmax(inarray)
    in = inarray;
    d = size(in);
    out = zeros(1,1,d(3));
    a = max(in(1,1,:));
    t = sum(exp(in(1,1,:)-a));
    
    
    for k=1:d(3)
        out(1,1,k) = exp(in(1,1,k) - a)/t;
    end
    outarray = out;
end
    