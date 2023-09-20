from queue import SimpleQueue, Empty
import threading
import serial

class SensorReader:
    def __init__(self, port, baud):

        self.queue = SimpleQueue()

        ser = serial.Serial(port, baud)

        print(f'Connected to Arduino on {port} at {baud} baud')

        producer_thread = threading.Thread(target=self.read_arduino_data, args=(ser, ))
        producer_thread.start()

    def read_arduino_data(self, ser):
        while True:
            try:
                data = ser.readline().decode().strip()
            except:
                continue
            self.queue.put(data)

    def get_oldest_data(self):
        try:
            data = self.queue.get()
        except Empty:
            data = None
        return data