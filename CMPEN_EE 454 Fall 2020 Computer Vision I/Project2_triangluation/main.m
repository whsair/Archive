%Group members: Zerui Li, Hongshuo Wang, Songyang Mao, Jiafu Chen
profile on;
load('Subject4-Session3-Take4_mocapJoints.mat');
load 'vue2CalibInfo.mat' vue2
load 'vue4CalibInfo.mat' vue4

%initialization of VideoReader for the vue video
filenamevue2mp4 = 'Subject4-Session3-24form-Full-Take4-Vue2.mp4';
vue2video = VideoReader(filenamevue2mp4);
filenamevue4mp4 = 'Subject4-Session3-24form-Full-Take4-Vue4.mp4';
vue4video = VideoReader(filenamevue4mp4);

firstIte = 1000;
lastIte = 1000;
iterations = 0;
%create matrix for storge joints data for evaluation    
evaluationData = zeros(12, (lastIte - firstIte + 1));

for mocapFnum = firstIte: lastIte
    iterations = iterations + 1;

    %Reading the 3D joint data
    x = mocapJoints(mocapFnum,:,1); 
    y = mocapJoints(mocapFnum,:,2); 
    z = mocapJoints(mocapFnum,:,3);  
    conf = mocapJoints(mocapFnum,:,4);  

    %read in the video for any mocap frame mocapFnum
    vue2video.CurrentTime = (mocapFnum-1)*(50/100)/vue2video.FrameRate;
    vid2Frame = readFrame(vue2video);
    vue4video.CurrentTime = (mocapFnum-1)*(50/100)/vue4video.FrameRate;
    vid4Frame = readFrame(vue4video);

    %create matrix for the 3D coordinates for each joints
    pixel2 = zeros(3,12);
    pixel4 = zeros(3,12);

    %Compute 2D pixel coordinates with Homogeneous Coordinates (2.3)
    %as x = x'/z', y = y'/z' 
    for i = 1:12
        cameraPoint = [x(i); y(i); z(i); 1];
        %compute to get [x'; y'; z']
        pixel2(:,i) = project3DTo2D(vue2, cameraPoint);
        pixel4(:,i) = project3DTo2D(vue4, cameraPoint);
    end
    
    %Plot joint points for Observation report
    plotJoints(vid2Frame, pixel2);
    plotJoints(vid4Frame, pixel4);
    
    %Qualitative analysis
    Qualitative(vid2Frame, pixel2);
    Qualitative(vid4Frame, pixel4);
    
    %Find Epipolar Lines
    [EpipolarLines1, EpipolarLines2] = findEpipolarLines(cameraPoint,...
        vue2, pixel2, vue4, pixel4);
    %Plot epipolar lines on frame
    plotEpline(EpipolarLines2, pixel2, vid2Frame);
    plotEpline(EpipolarLines1, pixel4, vid4Frame);
    
    % triangulation back into of 3d scene points (2.4)
    % p is final output point 
    P = reconstruct3DFrom2D(vue2, pixel2, vue4, pixel4);
    disp('Reconstruct 3D points for joints 1-12 are:');
    disp(P);
    
    %Measure error between tranglated and original 3d points (2.5)
    %get joint information for specific mocapFnum from mocapJionts
    joints = mocapJoints(mocapFnum,:,1:3);
    joint = zeros(3,12);
    for i = 1:3
        joint(i,:) = joints(:,:,i);
    end
    %D(i,j) is error of each points
    D = zeros(12,1);
    for i = 1:12
        %disp(joint(i,:));
        %disp(P(:,i));
        D(i,1) = euclidean_distance(joint(:,i), P(:,i));      
    end
    disp('Measure error between original and reconstruct 3D points for this Frame:');
    disp(D(:,1));

    %save the data of D to evaluationData for evaluatoin        
    evaluationData(:,iterations)= D;
end

%Evaluaion
%Evaluation Matrix for each of the twelve joint pairs
quantitativeEach = zeros(12,5);
for i = 1:12
    quantitativeEach(i,1) = mean(evaluationData(i,:));
    quantitativeEach(i,2) = std(evaluationData(i,:));
    quantitativeEach(i,3) = min(evaluationData(i,:));
    quantitativeEach(i,4) = median(evaluationData(i,:));
    quantitativeEach(i,5) = max(evaluationData(i,:));
end
disp('Evaluation Matrix for For each of the twelve joint pairs');
disp('     Mean      Std       Min      Median     Max');
disp(quantitativeEach);

%Evaluation Matrix for all of the twelve joint pairs
quantitativeAll = zeros(1,5);
quantitativeAll(1) = mean(evaluationData(:));
quantitativeAll(2) = std(evaluationData(:));
quantitativeAll(3) = min(evaluationData(:));
quantitativeAll(4) = median(evaluationData(:));
quantitativeAll(5) = max(evaluationData(:));
disp('Evaluation Matrix for For all of the twelve joint pairs');
disp('     Mean      Std       Min      Median     Max');
disp(quantitativeAll);

% Calculate error sums per frame
frame_error_sums = zeros(1,iterations);
for i = 1:iterations
    %disp(evaluationData(:,i));
    frame_error_sums(1, i) = sum(evaluationData(:,i));
end
frame = (firstIte: lastIte);
figure();
plot(frame, frame_error_sums);
title("Sum of Errors for Time Sequence");
xlabel("Frame"); ylabel("Total Error");

%profile report;
profile off;
