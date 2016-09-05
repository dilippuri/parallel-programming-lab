th= [2 4 8 16 32];
s1 = [2.25 12.5 28.75 79.25 121];
s2 = [1.23 4.82 13.88 15.47 24.47];
s3 = [1.75 1.80 1.38 1.41 1.79];
s4 = [1.17 1.16 2.03 2.06 2.13];
s5 = [.84 2.01 1.88 1.86 1.90];

ylim([0 121])
plot(th,s1,th,s2, th,s3, th,s4, th,s5)
xlabel("No of threads");
ylabel("Speedup Time");
title ('Measurement plot');
legend ('show');
print -dpng speedup2.png
 
