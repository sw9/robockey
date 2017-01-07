function [loc, rot] = get_location(arr)

max_d = -1;
offset = [512 384];

for i=1:3
    for j=i+1:4
        i_x = arr((i-1)*2 + 1);
        i_y = arr((i-1)*2 + 2);
        j_x = arr((j-1)*2 + 1);
        j_y = arr((j-1)*2 + 2);
        d = sqrt((i_x - j_x)^2 + (i_y - j_y)^2);
        if d > max_d
            ind_i = i;
            ind_j = j;
            max_d = d;
            p_i = [i_x i_y];
            p_j =  [j_x j_y];
        end
    end
end

center = [(p_i(1)+p_j(1))/2 (p_i(2)+p_j(2))/2];

i_sum = 0;
j_sum = 0;
for k=1:4
    if k ~= ind_i && k~= ind_j
        k_x = arr((k-1)*2 + 1);
        k_y = arr((k-1)*2 + 2);
        d_i = sqrt((k_x - p_i(1))^2 + (k_y - p_i(2))^2);
        d_j = sqrt((k_x - p_j(1))^2 + (k_y - p_j(2))^2);
        i_sum = i_sum + d_i;
        j_sum = j_sum + d_j;
    end
end

if i_sum > j_sum
    y = p_i - center;
else
    y = p_j - center;
end

rot = acos(y(2)/norm(y));

if y(1) > 0
    rot = -rot;
end

pos = (center - offset);
loc = -[cos(-rot) -sin(-rot); sin(-rot) cos(-rot)]*pos';

end