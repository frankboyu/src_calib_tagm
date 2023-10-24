import numpy as np
import matplotlib.pyplot as plt

file = open('results/mon_ver07/summary_problems.txt','w')

numbers = np.zeros(600)
offsets = np.zeros((600,2,102))
resolutions = np.zeros((600,2,102))
runlist = np.loadtxt("runs.dat",dtype=int)
i = -1

for run in runlist:
    if np.loadtxt("results/mon_ver07/tagm-timing-stats-"+str(run)+".txt").size>0:
        i += 1
        
        numbers[i] = run
        offsets[i,0,:] = np.loadtxt("results/mon_ver07/tagm-timing-stats-"+str(run)+".txt")[:,1]*1000
        offsets[i,1,:] = np.loadtxt("results/mon_ver07/tagm-timing-stats-"+str(run)+".txt")[:,9]*1000
        resolutions[i,0,:] = np.loadtxt("results/mon_ver07/tagm-timing-stats-"+str(run)+".txt")[:,2]*1000
        resolutions[i,1,:] = np.loadtxt("results/mon_ver07/tagm-timing-stats-"+str(run)+".txt")[:,10]*1000
        offsets[i,0,99] = 0
        offsets[i,1,99] = 0
        resolutions[i,0,99] = 0
        resolutions[i,1,99] = 0  
        
        if np.max(abs(offsets[i,0,:])) > 250:
            file.write(str(run)+": max ADC offset is "+str("%.2f" % np.max(abs(offsets[i,0,:])))+" ps, greater than 250 ps\n")
            continue
        if np.max(abs(offsets[i,1,:])) > 250:
            file.write(str(run)+": max TDC offset is "+str("%.2f" % np.max(abs(offsets[i,1,:])))+" ps, greater than 250 ps\n")
            continue
        if np.average(abs(offsets[i,0,:])) > 20:
            file.write(str(run)+": average ADC offset is "+str("%.2f" % np.average(abs(offsets[i,0,:])))+" ps, greater than 20 ps\n")
            continue    
        if np.average(abs(offsets[i,1,:])) > 20:
            file.write(str(run)+": average TDC offset is "+str("%.2f" % np.average(abs(offsets[i,1,:])))+" ps, greater than 20 ps\n")
            continue    
        if np.average(resolutions[i,1,:]) > 800:
            file.write(str(run)+" average TDC resolution is "+str("%.2f" % np.average(abs(resolutions[i,1,:])))+" ps, greater than 800 ps\n")
            continue
        if np.average(resolutions[i,1,:]) < 400:
            file.write(str(run)+" average TDC resolution is "+str("%.2f" % np.average(abs(resolutions[i,1,:])))+" ps, smaller than 400 ps\n")
            continue
    else:
        file.write(str(run)+" file doesn't exist\n")

plt.scatter(numbers[:i],np.average(offsets[:i,0,:],axis=1))
plt.axhline(20, color='r')
plt.axhline(-20, color='r')
plt.ylim([-200,200])
plt.xlabel("Run")
plt.ylabel("ADC offset(ps)")
plt.title("ADC offsets, average of all sectors")
plt.savefig('results/mon_ver07/summary_offsets_adc_average.png')
plt.close()

plt.scatter(numbers[:i],np.average(offsets[:i,1,:],axis=1))
plt.axhline(20, color='r')
plt.axhline(-20, color='r')
plt.ylim([-200,200])
plt.xlabel("Run")
plt.ylabel("TDC offset(ps)")
plt.title("TDC offsets, average of all sectors")
plt.savefig('results/mon_ver07/summary_offsets_tdc_average.png')
plt.close()

plt.scatter(numbers[:i],np.average(resolutions[:i,1,:],axis=1))
plt.ylim([400,800])
plt.xlabel("Run")
plt.ylabel("TDC resolution(ps)")
plt.title("TDC resolutions, average of all sectors")
plt.savefig('results/mon_ver07/summary_resolutions_average.png')
plt.close()

