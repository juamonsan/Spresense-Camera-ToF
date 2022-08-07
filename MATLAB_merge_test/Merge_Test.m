%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% Merge_test.m
% ------------
%
% This script loads a .csv data file which contais distance measures
% from a VL53L5CX sensor in a 8x8 matrix and an image, both taken at
% the same time. The it shows the image over a mesh containing the 
% distance measures in a 3D figure.
%
% This is part of the project "Spresense-Camera-ToF" located at
% https://github.com/juamonsan/Spresense-Camera-ToF
%
% This sript is based in "Colored Area on a Curved Surface" by Michael
% Wunder at Matlab File Exchange
% (https://es.mathworks.com/matlabcentral/fileexchange/5726-colored-area-on-a-curved-surface)
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Import data from text file

%% Set up the Import Options and import the data
opts = delimitedTextImportOptions("NumVariables", 8);

% Specify range and delimiter
opts.DataLines = [1, Inf];
opts.Delimiter = ";";

% Specify column names and types
opts.VariableNames = ["VarName1", "VarName2", "VarName3", "VarName4", "VarName5", "VarName6", "VarName7", "VarName8"];
opts.VariableTypes = ["double", "double", "double", "double", "double", "double", "double", "double"];

% Specify file level properties
opts.ExtraColumnsRule = "ignore";
opts.EmptyLineRule = "read";

% Import the data
ToF1 = readtable("Test_data/Data3_Hands/ToF004.csv", opts); %Change this filepath to the desired ToF data file

%% Convert to output type
ToF1 = table2array(ToF1);

%% Clear temporary variables
clear opts
 
%% Import Image
I=imread('Test_data/Data3_Hands/PICT004.JPG'); %Change this filepath to the desired image data file
imshow(I)
map=colormap;

sz=size(I);
ix=sz(1);
iy=sz(2);

% filter the image
h=fspecial('average',5);
filtim=imfilter(I,h);

% create reference surface
[x]=[1,2,3,4,5,6,7,8];
[y]=[1,2,3,4,5,6,7,8];
[z]=-ToF1;
sz=size(z);
sx=sz(1);
sy=sz(2);

% scaling
xfkt=sx/ix;
yfkt=sy/iy;

% plot image on surface
warp(z,filtim)
hold on;

