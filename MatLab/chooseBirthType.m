function birthType = chooseBirthType(p_birth, n)
    r = rand(1);

    %% choose birth
    birthType = 0; % not possible
    cdf_sum = p_birth(1);

    if (r < cdf_sum)
        birthType = 1;
    else
        for i = 2:1:n
            birthType = i;
            nextSum = cdf_sum + p_birth(i);

            if ((r >= cdf_sum) && (r < nextSum))
                break
            end
            cdf_sum = nextSum;
        end
    end
end