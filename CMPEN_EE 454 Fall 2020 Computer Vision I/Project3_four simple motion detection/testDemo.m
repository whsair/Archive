profile on;
    %load frame 
    % reference: https://www.mathworks.com/matlabcentral/answers/333532-how-to-extract-and-save-frames-in-a-video
    path = 'DataSets';
    folderName = 'walk'; %name of folder 
    %folder = strcat('/', folderName, '/'); %name of folder to load pictures from
    directory = strcat(path,'/', folderName); 
    %setup
    l = 150; %parameter theshold walk  tree 40  30ArenaN  50
    a = 0.1; %parameter alpha
    g = 80; %parameter gammar  walk tree 30    50ArenaN 140

    directory1 = strcat(directory, '/','*.jpg'); 
    files = dir(directory1); %load pictures
    disp(length(files))
    
    proj3main(directory, length(files), l, a, g);
    %create video
    %edit2Video('output', 'walk');
profile report;
profile off;
