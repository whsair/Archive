%In the original formula, it includes lemda but since we are want
%unit vector as our final result; getting rid of it doesn't affect the
%final results
function u = backwarding(pm, Kmat, Rmat)
    u = Rmat' * (Kmat \ pm);
    u = u/norm(u);
end