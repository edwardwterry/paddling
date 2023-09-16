from queue import SimpleQueue, Empty
import threading
import serial

class SensorReader:
    def __init__(self):

        self.queue = SimpleQueue()

        ser = serial.Serial('/dev/ttyACM0', 115200)

        producer_thread = threading.Thread(target=self.read_arduino_data, args=(ser, ))
        producer_thread.start()

    def read_arduino_data(self, ser):
        while True:
            data = ser.readline().decode().strip()
            self.queue.put(data)

    def get_oldest_data(self):
        try:
            data = self.queue.get()
        except Empty:
            data = None
        return data