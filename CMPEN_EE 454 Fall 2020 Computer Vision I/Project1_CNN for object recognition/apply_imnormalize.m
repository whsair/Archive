function outarray = apply_imnormalize(inarray)
    input = double(inarray);
    out = input/255.0 - 0.5;
    outarray = out;
end