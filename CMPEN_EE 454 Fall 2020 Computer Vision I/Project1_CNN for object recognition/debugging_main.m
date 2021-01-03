%loading this file defines imrgb and layerResults

load 'debuggingTest.mat'
load 'cifar10testdata.mat'
load 'CNNparameters.mat'
%sample code to show image and access expected results
figure; imagesc(imrgb); truesize(gcf,[64 64]);

%input image
outarray = imrgb;

%1 imnormalize 
outarray = apply_imnormalize(outarray);
debugging(outarray,1);
%2 convolve
outarray = layerResults{1};
outarray = apply_convolve(outarray, filterbanks{2}, biasvectors{2});
round_debugging(outarray,2);

%3 relu 
outarray = layerResults{2};
outarray = apply_relu(outarray);
debugging(outarray,3);
%4 convolve 
outarray = layerResults{3};
outarray = apply_convolve(outarray, filterbanks{4}, biasvectors{4});
round_debugging(outarray,4);
%5 relu
outarray = layerResults{4};
outarray = apply_relu(outarray);
debugging(outarray,5);
%6 maxpool
outarray = layerResults{5};
outarray = apply_maxpool(outarray);
debugging(outarray,6);
%7 convolve 
outarray = layerResults{6};
outarray = apply_convolve(outarray, filterbanks{7}, biasvectors{7});
round_debugging(outarray,7);
%8 relu 
outarray = layerResults{7};
outarray = apply_relu(outarray);
debugging(outarray,8);
%9 convolve 
outarray = layerResults{8};
outarray = apply_convolve(outarray, filterbanks{9}, biasvectors{9});
round_debugging(outarray,9);
%10 relu 
outarray = layerResults{9};
outarray = apply_relu(outarray);
debugging(outarray,10);
%11 maxpool 
outarray = layerResults{10};
outarray = apply_maxpool(outarray);
debugging(outarray,11);
%12 convolve 
outarray = layerResults{11};
outarray = apply_convolve(outarray, filterbanks{12}, biasvectors{12});
round_debugging(outarray,12);
%13 relu
outarray = layerResults{12};
outarray = apply_relu(outarray);
debugging(outarray,13);
%14 convolve 
outarray = layerResults{13};
outarray = apply_convolve(outarray, filterbanks{14}, biasvectors{14});
round_debugging(outarray,14);
%15 relu 
outarray = layerResults{14};
outarray = apply_relu(outarray);
debugging(outarray,15);
%16 maxpool
outarray = layerResults{15};
outarray = apply_maxpool(outarray);
debugging(outarray,16);
%17 fullconnect 
outarray = layerResults{16};
outarray = apply_fullconnect(outarray, filterbanks{17}, biasvectors{17});
debugging(outarray,17);
%18 softmax 
outarray = layerResults{17};
outarray = apply_softmax(outarray);
debugging(outarray,18);