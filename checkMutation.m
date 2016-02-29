function newBirthType = checkMutation(q, birthType, n)
    cdf_sum = q(birthType,1);
    newBirthType = 0; % not possible
    r = rand(1);
    
    if (r < cdf_sum)
        newBirthType = 1;
    else
        for i = 2:1:n
            newBirthType = i;
            nextSum = cdf_sum + q(birthType, i);

            if ((r >= cdf_sum) && (r < nextSum))
                break
            end
            cdf_sum = nextSum;
        end
    end

end