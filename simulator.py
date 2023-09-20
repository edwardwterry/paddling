import yaml
import numpy as np

from read_arduino import SensorReader
from vehicle import RawUltrasonicMeasurement, DistanceStamped, Vehicle
from network import Server

class Simulator:
    def __init__(self):
        self.dt = 0.05 # 20Hz
        self.server = Server('localhost', 12345)
        self.sensor_reader = SensorReader('/dev/ttyACM0', 9600)
        self.vehicles = {'target': Vehicle(self.dt),
                         'ego': Vehicle(self.dt)}

    def publish_state(self):
        for vehicle in self.vehicles:
            self.server.send_to_client(str(vehicle.get_state()))

    def run(self):
        # self.vehicle['target'].

        raw = self.sensor_reader.get_oldest_data()
        if raw is not None:

            raw = RawUltrasonicMeasurement(raw)
            self.vehicles['ego'].add_measurement(raw.as_distance_stamped_array())
            self.vehicles['ego'].step()

if __name__ == '__main__':
    sim = Simulator()
