import matplotlib as mpl        # "QXcbConnection: Could not connect to display" への対策
mpl.use('Agg')                  # "QXcbConnection: Could not connect to display" への対策
import matplotlib.pyplot as plt # "QXcbConnection: Could not connect to display" への対策
import matplotlib.ticker as tick
import math

#------------------------------------------------------------------------------------------------------------------------------------------------
def float2digits(f):
    if f==0:
        return int(0)
    elif f<1:
        return -int(math.log10(1/f))
    else:
        return int(math.log10(f)) + 1
def vecLim2tickInterval(vecLim):
    diff = vecLim[1] - vecLim[0]
    digits = float2digits(diff)
    interval = pow(10, digits)/10
    
    # make the tickNum more than 5.
    tickNum = diff / interval
    if tickNum <= 2:
        interval = interval/10
    elif tickNum <= 6:
        interval = interval/2
    
    return interval

def vvec2graph_base(savePath, saveAs, xlabel, ylabel, vecLabel, labelLoc, vvecX, vvecY, vecXlim, vecYlim, xscale, yscale):
    imgSize = 4
    
    plt.clf()
    plt.rcParams["font.size"] = 10*imgSize
    fig = plt.figure(figsize=(8.5*imgSize, 4*imgSize)) # アスペクト比の設定
    ax1 = fig.add_subplot(111)
    
    ax1.grid(which='minor', linewidth=1, linestyle=':', color='gainsboro')
    ax1.grid(which='major', linewidth=1, linestyle='-', color='silver'   )
    
    ax1.set_xlabel(xlabel)
    ax1.set_xscale(xscale)
    ax1.set_xlim(vecXlim[0], vecXlim[1])
    
    ax1.set_ylabel(ylabel)
    ax1.set_yscale(yscale)
    ax1.set_ylim(vecYlim[0], vecYlim[1])
    
    ax1.tick_params(pad=5*imgSize, which='major', direction='in', bottom=True, top=True, left=True, right=True, length=4*imgSize) # 軸の余白 # which: major tick と minor tick に対して変更を適用 # tick を内側方向に # tick を bottom, top, left, right に付加 # tick width # tick length
    ax1.tick_params(pad=5*imgSize, which='minor', direction='in', bottom=True, top=True, left=True, right=True, length=2*imgSize) # 軸の余白 # which: major tick と minor tick に対して変更を適用 # tick を内側方向に # tick を bottom, top, left, right に付加 # tick width # tick length
    if xscale=='linear':
        ax1.xaxis.set_major_formatter(tick.ScalarFormatter(useMathText=True))
        ax1.ticklabel_format(style='sci', axis='x', scilimits=(0,0))
        ax1.xaxis.set_major_locator(tick.MultipleLocator( vecLim2tickInterval(vecXlim)   ))
        ax1.xaxis.set_minor_locator(tick.MultipleLocator( vecLim2tickInterval(vecXlim)/4 ))
    if yscale=='linear':
        ax1.yaxis.set_major_locator(tick.MultipleLocator( vecLim2tickInterval(vecYlim)   ))
        ax1.yaxis.set_minor_locator(tick.MultipleLocator( vecLim2tickInterval(vecYlim)/4 ))
    
    #cmap = plt.get_cmap("tab10")
    darkred = '#640A1E' # 'crimson'
    vColor=['black', 'blue', 'fuchsia', 'red', darkred, 'green', 'darkorange']
    vLineStyle = ['solid', 'dashed', 'dashed', 'dashed', 'dashdot', 'dotted', 'dotted'] # solid, dashed, dashdot, dotted
    for i in range(len(vecLabel)):
        #ax1.plot(vvecX[i], vvecY[i], linewidth=1, color=cmap(i), label=vecLabel[i])
        ax1.plot(vvecX[i], vvecY[i], linewidth=1, color=vColor[i], linestyle=vLineStyle[i], label=vecLabel[i])
    
    alpha = 0.3
    if len(labelLoc)!=0: ax1.legend(framealpha=alpha, loc=labelLoc)
    else:                ax1.legend(framealpha=alpha)
    
    for ext in saveAs:
        plt.savefig(savePath+ext, bbox_inches='tight')
    
    return
#------------------------------------------------------------------------------------------------------------------------------------------------

# labelLoc = 'upper left'
# labelLoc = 'upper right'
# labelLoc = 'lower right'
# labelLoc = 'lower left'

def vvec2graph(savePath, saveAs, xlabel, ylabel, vecLabel, vvecX, vvecY):
    vecXlim = [0.9, 55000000]
    vecYlim = [0.05, 1000]
    xscale = 'log'
    yscale = 'log'
    labelLoc = ''
    vvec2graph_base(savePath, saveAs, xlabel, ylabel, vecLabel, labelLoc, vvecX, vvecY, vecXlim, vecYlim, xscale, yscale)
    return

def vvec2graph_lf(savePath, saveAs, xlabel, ylabel, vecLabel, vvecX, vvecY):
    vecXlim = [0.9, 55000000]
    vecYlim = [0, 1.025]
    xscale = 'log'
    yscale = 'linear'
    labelLoc = 'lower right'
    vvec2graph_base(savePath, saveAs, xlabel, ylabel, vecLabel, labelLoc, vvecX, vvecY, vecXlim, vecYlim, xscale, yscale)
    return

def vvec2graph_memory(savePath, saveAs, xlabel, ylabel, vecLabel, vvecX, vvecY):
    xmargin=4*10000; vecXlim = [-xmargin, 5*1000000+xmargin]
    ymargin=10; vecYlim=[0-ymargin, 600+ymargin]
    xscale = 'linear'
    yscale = 'linear'
    labelLoc = 'upper left'
    vvec2graph_base(savePath, saveAs, xlabel, ylabel, vecLabel, labelLoc, vvecX, vvecY, vecXlim, vecYlim, xscale, yscale)
    return

def vvec2graph_et_insert(savePath, saveAs, xlabel, ylabel, vecLabel, vvecX, vvecY):
    xmargin=4*10000; vecXlim = [-xmargin, 5*1000000+xmargin]
    ymargin=0.025; vecYlim=[0-ymargin, 2.5+ymargin]
    xscale = 'linear'
    yscale = 'linear'
    labelLoc = 'upper left'
    vvec2graph_base(savePath, saveAs, xlabel, ylabel, vecLabel, labelLoc, vvecX, vvecY, vecXlim, vecYlim, xscale, yscale)
    return 

