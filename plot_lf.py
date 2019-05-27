import matplotlib as mpl        # "QXcbConnection: Could not connect to display" への対策
mpl.use('Agg')                  # "QXcbConnection: Could not connect to display" への対策
import matplotlib.pyplot as plt # "QXcbConnection: Could not connect to display" への対策
import matplotlib.ticker as tick

def vvec2graph(savePath, xlabel, ylabel, vecLabel, vvecX, vvecY):
    plt.clf()
    fig = plt.figure(figsize=(8*3, 2.5*4)) # アスペクト比の設定
    ax1 = fig.add_subplot(111)
    
    ax1.grid(which='minor', linewidth=1, linestyle=':',  color='gainsboro')
    ax1.grid(which='major', linewidth=1, linestyle='-',  color='silver'   )
    
    ax1.set_xlabel(xlabel)
    ax1.set_xscale('log')
#    ax1.set_xlim(0.9, 55000000)
    
    ax1.set_ylabel(ylabel)
#    ax1.set_yscale('log')
#    ax1.set_ylim(0.05, 1000)
    ax1.set_ylim(0, 1.025)
    
    ax1.tick_params(pad=5, which='major', direction='in', bottom=True, top=True, left=True, right=True, length=4) # 軸の余白 # which: major tick と minor tick に対して変更を適用 # tick を内側方向に # tick を bottom, top, left, right に付加 # tick width # tick length
    ax1.tick_params(pad=5, which='minor', direction='in', bottom=True, top=True, left=True, right=True, length=2) # 軸の余白 # which: major tick と minor tick に対して変更を適用 # tick を内側方向に # tick を bottom, top, left, right に付加 # tick width # tick length
    ax1.yaxis.set_major_locator(tick.MultipleLocator(0.1  ))
    ax1.yaxis.set_minor_locator(tick.MultipleLocator(0.025))
    
    #cmap = plt.get_cmap("tab10")
    vColor=['black', 'blue', 'red', 'green', 'sandybrown']
    vLineStyle = ['solid', 'dashed', 'dashdot', 'dotted', 'dotted'] # solid, dashed, dashdot, dotted
    for i in range(len(vecLabel)):
        ax1.plot(vvecX[i], vvecY[i], linewidth=1, color=vColor[i], linestyle=vLineStyle[i], label=vecLabel[i])
        #ax1.plot(vvecX[i], vvecY[i], linewidth=1, color=cmap(i), label=vecLabel[i])
    #ax1.legend(loc='upper right')
    ax1.legend(loc='lower right', borderaxespad=7)
#    ax1.plot(vecX[i], vecY[i], color='k', linewidth=1)
    plt.savefig(savePath, bbox_inches='tight')
    
    return
