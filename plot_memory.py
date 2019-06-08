import matplotlib as mpl        # "QXcbConnection: Could not connect to display" への対策
mpl.use('Agg')                  # "QXcbConnection: Could not connect to display" への対策
import matplotlib.pyplot as plt # "QXcbConnection: Could not connect to display" への対策
import matplotlib.ticker as tick
import math
import itertools

def flatten(rhs):
    return list(itertools.chain(*rhs))

def vvec2graph(savePath, xlabel, ylabel, vecLabel, vvecX, vvecY):
    plt.clf()
    fig = plt.figure(figsize=(8*3, 2.5*4)) # アスペクト比の設定
    ax1 = fig.add_subplot(111)
    
    ax1.grid(which='minor', linewidth=1, linestyle=':',  color='gainsboro')
    ax1.grid(which='major', linewidth=1, linestyle='-',  color='silver'   )
    
    # x: locator
    ax1.set_xlabel(xlabel)
    maxVal   = max(flatten(vvecX))
    digits   = int(math.log10(maxVal))
    majorLoc = int(math.pow(10, digits  ))/2
    minorLoc = int(math.pow(10, digits-1))
    ax1.xaxis.set_major_locator(tick.MultipleLocator(majorLoc))
    ax1.xaxis.set_minor_locator(tick.MultipleLocator(minorLoc))
    
    # x: lim
    num_of_minorLoc = int((maxVal+minorLoc*0.5)//minorLoc)
    margen = 0.3
    xlim_min = -margen*minorLoc
    xlim_max = num_of_minorLoc * minorLoc + margen*minorLoc
    ax1.set_xlim(xlim_min, xlim_max)
    
    # x: x10^X
    ax1.xaxis.set_major_formatter(tick.ScalarFormatter(useMathText=True))
    ax1.ticklabel_format(style='sci', axis='x', scilimits=(0,0))
    
    # y: locator
    ax1.set_ylabel(ylabel)
    ax1.yaxis.set_major_locator(tick.MultipleLocator(100))
    ax1.yaxis.set_minor_locator(tick.MultipleLocator( 20))
    #ax1.set_yscale('log')
    #ax1.set_ylim(0.05, 1000)
    ax1.set_ylim(-10, 610)
    
    # x-y: tick
    ax1.tick_params(pad=5, which='major', direction='in', bottom=True, top=True, left=True, right=True, length=4) # 軸の余白 # which: major tick と minor tick に対して変更を適用 # tick を内側方向に # tick を bottom, top, left, right に付加 # tick width # tick length
    ax1.tick_params(pad=5, which='minor', direction='in', bottom=True, top=True, left=True, right=True, length=2) # 軸の余白 # which: major tick と minor tick に対して変更を適用 # tick を内側方向に # tick を bottom, top, left, right に付加 # tick width # tick length
    
    #cmap = plt.get_cmap("tab10")
    vColor=['black', 'blue', 'red', 'green', 'sandybrown']
    vLineStyle = ['solid', 'dashed', 'dashdot', 'dotted', 'dotted'] # solid, dashed, dashdot, dotted
    for i in range(len(vecLabel)):
        ax1.plot(vvecX[i], vvecY[i], linewidth=1, color=vColor[i], linestyle=vLineStyle[i], label=vecLabel[i])
        #ax1.plot(vvecX[i], vvecY[i], linewidth=1, color=cmap(i), label=vecLabel[i])
    ax1.legend(loc='upper left')
    plt.savefig(savePath, bbox_inches='tight')

