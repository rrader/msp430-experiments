import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import threading
from collections import deque
import numpy

data = deque((0 for _ in range(5000)), maxlen=5000)
dataAVG = deque((0 for _ in range(5000)), maxlen=5000)
dataP = deque((0 for _ in range(5000)), maxlen=5000)
dataS = deque((0 for _ in range(5000)), maxlen=5000)
dataV = deque((0 for _ in range(5000)), maxlen=5000)
window = deque((0 for _ in range(500)), maxlen=10)
avg = 0
norm = 0
avg_norm = 0
i = 0
var = 0

def addValue(val):
    global avg, norm, n, avg_norm, i, var

    data.append(val)

    if avg == 0:
        avg = val
    avg = avg + 0.001 * (val - avg)  # slowly converge to average
    dataAVG.append(avg)

    nval = abs(val - avg)
    norm = norm + 0.1 * (nval - norm)
    dataP.append(norm)
    if i >= 10:
        window.append(norm)
        i = 0
        var = numpy.var(window)
    i += 1

    # confidence = numpy.mean(window) + numpy.var(window)*3  # 3 sigma interval, 99.7%
    VMR = 0
    avg_norm = avg_norm + 0.1 * (norm - avg_norm)  # slowly converge to average
    if avg_norm > 0:
        VMR = var / avg_norm  # variation coefficient
    dataV.append(VMR)

    level = 0
    if VMR >= 1:
        level = 1
    dataS.append(level)


def msp430():
    print("Connecting...")
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    print("Connected!")

    # Handshake...
    ok = b''
    while ok.strip() != b'OK':
        ser.write(b"1")
        ok = ser.readline()
        print(ok.strip())
    print("Handshake OK!\n")

    while True:
        try:
            val = int(ser.readline().strip())
            addValue(val)
        except ValueError:
            pass


if __name__ == "__main__":
    threading.Thread(target=msp430).start()

    fig = plt.figure()
    p1 = plt.subplot(3, 2, 1)
    plot_data, = p1.plot(data, animated=True)
    plot_avg, = p1.plot(data, animated=True)
    p2 = plt.subplot(3, 2, 3)
    plot_processed, = p2.plot(data, animated=True)
    p3 = plt.subplot(3, 2, 5)
    plot_signal, = p3.plot(data, animated=True)
    p4 = plt.subplot(1, 2, 2)
    plot_var, = p4.plot(data, animated=True)

    p1.set_title("raw data and mean")
    p1.set_ylim(0, 0x200)
    p2.set_title("|x_i - mean|")
    p2.set_ylim(0, 100)
    p3.set_title("binary signal")
    p3.set_ylim(0, 2)
    p4.set_title("VMR")
    p4.set_ylim(0, 25)

    p4.axhline(y=1.,color='k',ls='dashed')
    def animate(i):
        global data
        plot_data.set_ydata(data)
        plot_data.set_xdata(range(len(data)))
        plot_avg.set_ydata(dataAVG)
        plot_avg.set_xdata(range(len(dataAVG)))
        plot_processed.set_ydata(dataP)
        plot_processed.set_xdata(range(len(dataP)))
        plot_signal.set_ydata(dataS)
        plot_signal.set_xdata(range(len(dataS)))
        plot_var.set_ydata(dataV)
        plot_var.set_xdata(range(len(dataV)))
        return [plot_data, plot_avg, plot_processed, plot_signal, plot_var]

    ani = animation.FuncAnimation(fig, animate, range(10000), 
                                  interval=100, blit=True)
    plt.show()
