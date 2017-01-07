load('A.mat');
[len, ~] = size(rawStarData);

x = zeros(len, 1);
y = zeros(len, 1);
u = zeros(len, 1);
v = zeros(len, 1);

for i=1:len
   [loc, rot] = get_location(rawStarData(i,:));
   x(i) = loc(1);
   y(i) = loc(2);
   pos = [1 0];
   loc = [cos(-rot) -sin(-rot); sin(-rot) cos(-rot)]*pos';
   u(i) = loc(1);
   v(i) = loc(2);
end

figure
hold on
title('Plot A')
xlabel('x offset (px)')
ylabel('y offset (px)')
quiver(x, y, u, v)
hold off