% measurements
x = [0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 127]; % steering input to the steering servo
y = [0, 2, 4, 6, 8, 11, 13, 16, 19, 22, 25, 28, 32, 35]; % wheel turning angle in degrees

% 2nd order polynomial fitted to the measurement data
p1 = 0.0007722;
p2 = 0.1723;
p3 = 0.1239;
angle = @(x) p1.*x.^2 + p2.*x + p3;

% interpolated data from the fit function
x1 = [0:1:127];
y1 = angle(x1);

plot(x, y, 'o', x1, y1);

% convert angles (in degrees) to sines
y2 = sind(y1);

% output result to a file
fileID = fopen('sines.txt', 'w');
fprintf(fileID, 'F8(%3.6f), ', y2);
fclose(fileID);
