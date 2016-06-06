%% build A
% 
% clear;
% clc;
% close all;


function A = buildA(num, a, b, c, d)


A = zeros(num,num);



A(1,1) = a;
A(1,num) = b;
A(num,1) = c;
A(num,num) = d;

if num == 16
    for i = 1:1:11
        for j = 1:1:11
            A(i,j) = a;
        end
    end
    
    for i = 1:1:11
        for j = 12:1:16
            A(i,j) = b;
        end
    end
    
    for i = 12:1:16
        for j = 1:1:11
            A(i,j) = c;
        end
    end
    
    for i = 12:1:16
        for j = 12:1:16
            A(i,j) = d;
        end
    end
    
    
elseif num == 256
    for i = 1:1:171
        for j = 1:1:171
            A(i,j) = a;
        end
    end
    
    for i = 1:1:171
        for j = 172:1:256
            A(i,j) = b;
        end
    end
    
    for i = 172:1:256
        for j = 1:1:171
            A(i,j) = c;
        end
    end
    
    for i = 172:1:256
        for j = 172:1:256
            A(i,j) = d;
        end
    end
end




end

