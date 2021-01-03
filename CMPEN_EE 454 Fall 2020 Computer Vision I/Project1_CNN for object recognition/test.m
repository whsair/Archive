%loading this file defines imageset, trueclass, and classlabels
load 'cifar10testdata.mat'
%some sample code to read and display one image from each class
for classindex = 1:10
    %get indices of all images of that class
    inds = find(trueclass==classindex);
    %take first one
    imrgb = imageset(:,:,:,inds(1));
    %display it along with ground truth text label
    figure;
    imagesc(imrgb);
    truesize(gcf,[64 64]);
    title(sprintf('%s',classlabels{classindex}));
end






