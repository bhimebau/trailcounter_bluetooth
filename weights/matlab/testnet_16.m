load('singlenet_16nodes_64bits_04-Mar-2015.mat')
load('TIME_set.mat')

class = 1;
scaled_wave = TIME_sampled(:,class)/std(TIME_sampled(:,class));
scaled_wave = [scaled_wave1(end); scaled_wave1(1:end-1)];
% for i = 1:32
%     scaled_wave_iter(i) = TIME_sampled(i,class)/std(TIME_sampled(:,class));
%     if scaled_wave(i) == scaled_wave_iter(i)
%         disp('matches')
%     else
%         disp('does not match')
%     end
% end

for hidden_neuron = 1:16
    sum = 0;
    for input = 1:32
        sum = sum + scaled_wave(input) * weightsIW{class}(hidden_neuron,input);
    end
    if sum > 1
        sum = 1;
    elseif sum < 0
        sum = 0;
    end
    hidden_out(hidden_neuron) = sum;
end

sum = 0;
for input = 1:16
    sum = sum + hidden_out(input) * weightsLW{class}(input);
end
if sum > 1
    sum = 1;
elseif sum < 0
    sum = 0;
end
sum