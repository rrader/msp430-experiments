import matplotlib.pyplot as plt
import matplotlib.animation as animation
import serial
import threading
from collections import deque

data = deque(0 for _ in range(5000))
dataP = deque(0 for _ in range(5000))
avg = 0

def addValue(val):
    global avg

    data.append(val)
    data.popleft()
    
    avg = avg + 0.1 * (val - avg)
    dataP.append(avg)
    dataP.popleft()


def msp430():
    print("Connecting...")
    ser = serial.Serial('/dev/ttyACM0', 9600, timeout=1)
    print("Connected!")

    # Handshake...
    ok = b''
    while ok.strip() != b'OK':
        ser.write(b"1")
        ok = ser.readline()
    print("Handshake OK!\n")

    while True:
        try:
            val = int(ser.readline().strip())
            addValue(val)
        except ValueError:
            pass


if __name__ == "__main__":
    threading.Thread(target=msp430).start()

    fig, (p1, p2) = plt.subplots(2, 1)
    plot_data, = p1.plot(data, animated=True)
    plot_processed, = p2.plot(data, animated=True)
    p1.set_ylim(0, 100)
    p2.set_ylim(0, 100)
    def animate(i):
        plot_data.set_ydata(data)
        plot_data.set_xdata(range(len(data)))
        plot_processed.set_ydata(dataP)
        plot_processed.set_xdata(range(len(dataP)))
        return [plot_data, plot_processed]

    ani = animation.FuncAnimation(fig, animate, range(10000), 
                                  interval=50, blit=True)
    plt.show()
