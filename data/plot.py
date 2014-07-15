import time
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import threading
from collections import deque

data = deque(0 for _ in range(5000))
dataAVG = deque(0 for _ in range(5000))
dataP = deque(0 for _ in range(5000))
dataS = deque(0 for _ in range(5000))
avg = 0
norm = 0

def addValue(val):
    global avg, norm

    data.append(val)
    data.popleft()
    
    if avg == 0:
        avg = val
    avg = avg + 0.001 * (val - avg)  # slowly converge to average
    dataAVG.append(avg)
    dataAVG.popleft()

    nval = abs(val - avg)
    norm = norm + 0.1 * (nval - norm)
    dataP.append(norm)
    dataP.popleft()

    n = 0
    if norm >= 10:
        n = 1
    dataS.append(n)
    dataS.popleft()


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
            print(val)
        except ValueError:
            pass


if __name__ == "__main__":
    threading.Thread(target=msp430).start()

    fig, (p1, p2, p3, p4) = plt.subplots(4, 1)
    plot_data, = p1.plot(data, animated=True)
    plot_avg, = p2.plot(data, animated=True)
    plot_processed, = p3.plot(data, animated=True)
    plot_signal, = p4.plot(data, animated=True)
    p1.set_ylim(0, 0x200)
    p2.set_ylim(0, 0x200)
    p3.set_ylim(0, 100)
    p4.set_ylim(0, 2)
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
        return [plot_data, plot_avg, plot_processed, plot_signal]

    ani = animation.FuncAnimation(fig, animate, range(10000), 
                                  interval=50, blit=True)
    plt.show()
