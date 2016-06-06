function deathType = chooseDeathType(p_death, n);
    %% choose death
    deathType = 0; % not possible
    r = rand(1);
    
    cdf_sum = p_death(1);

    if (r < cdf_sum)
        deathType = 1;
    else
        for i = 2:1:n
            deathType = i;
            nextSum = cdf_sum + p_death(i);

            if ((r >= cdf_sum) && (r < nextSum))
                break
            end
            cdf_sum = nextSum;
        end
    end
    
end