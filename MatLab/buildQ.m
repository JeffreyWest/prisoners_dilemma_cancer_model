%% hamming distance
function q_mut = buildQ(n,m_d,m_p)
    genetic_strings = zeros(2^n, 1);

    for k = 1:1:(2^n)
        str = dec2bin(k-1,n);

        for i = 1:1:n
            genetic_strings(k,i) = bin2dec(str(i));
        end
    end


    q = zeros((2^n),(2^n));

    for i = 1:1:(2^n)
        for j = 1:1:(2^n)
            A = uint8(genetic_strings(i, :));
            B = uint8(genetic_strings(j, :));
            if sum(bitxor(A, B)) == 1
                q(i,j) = 1;
            end
        end
    end

    divisor = 11;
    ending = 2^n;

    %% fix passenger


    q_passenger = zeros(ending,ending);
    for i = 1:1:divisor
        for j = 1:1:divisor
            q_passenger(i,j) = q(i,j);
        end
    end

    for i = divisor+1:1:ending
        for j = divisor+1:1:ending
            q_passenger(i,j) = q(i,j);
        end
    end    


    % normalize
    [H, ~] = size(q_passenger);
    for i = 1:H
        q_passenger(i,:) = q_passenger(i,:)/sum(q_passenger(i,:))*m_p;
    end


    %% fix driver


    q_driver = zeros(ending,ending);
    for i = 1:1:divisor
        for j = divisor+1:1:ending
            q_driver(i,j) = q(i,j);
        end
    end




    % normalize
    [H, ~] = size(q_driver);
    for i = 1:H
        if sum(q_driver(i,:)) ~= 0
            q_driver(i,:) = q_driver(i,:)/sum(q_driver(i,:))*m_d;
        end
    end

    q_mut = q_driver + q_passenger;

    for i = 1:1:size(q_driver)
        q_mut(i,i) = 1 - sum(q_mut(i,:));
    end

end