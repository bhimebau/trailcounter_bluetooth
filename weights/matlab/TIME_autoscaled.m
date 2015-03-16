load 'TIME_set.mat'
for i = 1:7
    TIME_auto(:,i) = TIME(:,i)/std(TIME(:,i));
    TIME_sampled_auto(:,i) = TIME_sampled(:,i)/std(TIME_sampled(:,i));
end 
save('TIME_set_auto.mat','TIME_auto','TIME_sampled_auto')
