%reference https://www.mathworks.com/matlabcentral/answers/112957-how-to-find-euclidean-distance
function d = euclidean_distance(p1,p2)
    d = sqrt(sum((p1(:) - p2(:)) .^ 2));
end
