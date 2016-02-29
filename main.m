%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%   author: Jeffrey West
%   date: 2-29-2016
%
%   publications: this code or code developed on top of this
%       framework has been used in the following publications:
% 
%   The Prisoner's Dilemma as a cancer model
%       http://arxiv.org/abs/1512.04591
%
%   An evolutionary model of tumor cell kinetics and 
%    the emergence of molecular heterogeneity driving 
%    Gompertzian growth
%       http://arxiv.org/abs/1512.04590
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


clc;clear;close all;

%% parameters
m = 0.1;            % mutation rate
w = 0.5;            % selection pressure
N = 1000;           % total cells    
bits = 4;           % numerical genome length
n = 2^bits;         % number of genomic types
a = 3;              % prisoner's dilemma parameters
b = 0; 
c = 5; 
d = 1;
A = buildA(n, a, b, c, d);  % payoff matrix dimension [n x n]
q = buildQ(bits, m/2, m/2); % mutation matrix dimension [n x n]

%% run average simulation
totalTime = 25*N;  % a guess at how long the simulation will require for saturation
t_vec = 0:1:(totalTime-1);
x_over_time = zeros(totalTime,16);

x = zeros(1,n); % a vector of the # of cells of each genetic types (n types)
x(1) = N; % begin the simulation with ALL healthy cells of type 0000


for time = 1:1:totalTime

    i = sum(x(12:16));

    % we have chose to have 11 healthy types and 5 cancer types
    fh = zeros(1,11) + (1 - w + w*(( 3*(N - i - 1) ) / (N - 1)));
    fc = zeros(1,5) + (1 - w + w*(( 5*(N - i) + (i-1) )/(N - 1)));

    f = [fh fc]; % result looks like this: [fh fh fh fh fh fh fh fh fh fh fh fc fc fc fc fc]

    p_birth = x.*f/(x*f');
    p_death = x/sum(x);

    %% choose birth and check for mutation
    birthType = chooseBirthType(p_birth, n);
    birthType = checkMutation(q, birthType, n);
    deathType = chooseDeathType(p_death, n);

    x(birthType) = x(birthType) + 1;
    x(deathType) = x(deathType) - 1;

    x_over_time(time, :) = x;

end

%% plot cancer growth only ( genetic types 12 - 16 )
average_cancer = sum(x_over_time(:,12:16)');

figure(1);clf
plot(t_vec, average_cancer/1000, 'k--', 'LineWidth', 2); box off; set(gcf,'color','w'); hold on;
xlabel('cell divisions', 'Interpreter','Latex', 'FontSize', 25);
ylabel('$i(t)$', 'Interpreter','Latex', 'FontSize', 25);
set(gcf, 'Position',[100,1000,1000,500]);










