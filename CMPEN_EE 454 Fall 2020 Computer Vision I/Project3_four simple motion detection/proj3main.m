function proj3main(directory, maxframenum, l, a, g)
    directory1 = strcat(directory, '/','*.jpg'); 
    files = dir(directory1); %load pictures
    %create new directory 
    outputFolderName = strcat('output', ''); %name of folder for new images
    mkdir(pwd, outputFolderName); %make new folder 
    
    l = l/225; %parameter theshold conversion 
    g = g/225; %parameter gammar conversion
    
    B1 = strcat(directory, '/',files(1).name); %initialize the same frame as the background frame
    %[y,x,z] = size(imread(B1)); %get frame size
    B1 = grayscale(B1); %convert background frame to grayscale frame
    B2 = B1; % B2, B3, B4 are the background frame for each algo 
    B3 = B1; 
    B4 = B1; 
    H = 0; % parameter history Persistent Frame Differencing

    

    for i = 1: maxframenum

        %Simple Background Subtraction
        %{
            loop time t
            I(t) = next frame;
            diff = abs[B – I(t)];
            M(t) = threshold(diff,l);
        %}

        I1 = strcat(directory, '/',files(i).name); %concat path to current frame
        I1 = grayscale(I1); %convert background image to greyscale image
        I2 = I1; 
        I3 = I1;
        I4 = I1;


        image_1 = sbs(I1,B1,l); 

        %Simple Frame Differencing
        %{
            B(0) = I(0);
            …
            loop time t
            I(t) = next frame;
            diff = abs[B(t-1) – I(t)];
            M(t) = threshold(diff,l);
            …
            B(t) = I(t);
            end
        %}

        image_2 = sbs(I2,B2,l);
        B2 = I2; %Update background frame with current frame

        %Adaptive Background Subtraction
        %{
            B(0) = I(0);
            …
            loop time t
            I(t) = next frame;
            diff = abs[B(t-1) – I(t)];
            M(t) = threshold(diff,l);
            …
            B(t) = a I(t)+(1–a)B(t-1);
            end

        %}

        image_3 = sbs(I3,B3,l);
        B3 = a.*I3 + (1-a).*B3; %Current image is “blended” into the background model with parameter a
                                %a = 0 yields simple background subtraction, a = 1 yields frame differencing

        %Persistent Frame Differencing
        %{
            B(0) = I(0);
            H(0) = 0;
            loop time t
            I(t) = next frame;
            diff = abs[B(t-1) – I(t)];
            M(t) = threshold(diff,l);
            tmp = max[H(t-1)-g,0)];
            H(t) = max[255*M(t),tmp)];
            …
            B(t) = I(t);
            end
        %}

        image_4 = sbs(I4,B4,l);
        tmp = max(H-g,0); %decay the white term
        H = max(image_4, tmp); %update history
        image_4 = H; %return output
        B4 = I4; %Update background frame with current frame


        %create figure
        figure('visible', 'off');
        imshow([image_1 image_2; image_3 image_4]); %put images onto panel figure

       

        saveas(gcf, strcat(outputFolderName, '/', sprintf('out%04d', i), '.png')); %save figure in the out000x.png format
        clf(); %clear figure
    end

end