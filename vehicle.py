import numpy as np
from dataclasses import dataclass
from trajectory import PrecomputedTrajectory

@dataclass
class Pose2D:
    x = 0.0
    y = 0.0
    psi = 0.0

    def __str__(self):
        return f'x: {self.x}, y: {self.y}, psi: {self.psi}'

@dataclass
class DistanceStamped:
    stamp: float # sec
    distance: float # m

    @staticmethod
    def from_raw_string(raw, index, speed_sound = 340.0):
        split = raw.split(',')
        stamp = int(split[0]) / 1000. # millisec to sec
        pulse_time = split[1 + index]
        distance = pulse_time * speed_sound / 2.0
        return DistanceStamped(stamp, distance)

@dataclass
class RawUltrasonicMeasurement:
    raw: str
    num_sensors = 2

    def as_distance_stamped_array(self) -> DistanceStamped:
        split = self.raw.split(',')
        stamp = int(split[0]) / 1000. # millisec to sec
        pulse_times = [float(t) for t in split[1:1+self.num_sensors]]
        distances = [t * self.speed_sound / 2.0 for t in pulse_times]
        return [DistanceStamped(stamp, dist) for dist in distances]

class Vehicle:
    def __init__(self, dt):
        self.x = Pose2D()

        self.dt = dt

    def step(self):
        # do physics here
        pass

    def get_state(self):
        return self.x
    
class OnlineVehicle(Vehicle):
    def __init__(self):
        self.xd = Pose2D()
        self.measurements = []
        # Physical parameters
        self.m = 1.0
        self.I = 1.0
        self.Cd = 1.0
    def add_measurement(self, z: list[DistanceStamped]):
        # if either measurement is spurious, ignore it
        self.measurements.append(z)

    def filter_outliers(self, measurement):
        pass

class PrecomputedVehicle(Vehicle):
    def __init__(self, trajectory_params_yaml):
        self.trajectory = PrecomputedTrajectory(trajectory_params_yaml, self.dt)
        self.index = 0

    def step(self):
        self.x = self.trajectory

#   var dt = t_curr - t_prev;

#   if (dists_prev[0] < dist_max && dists_curr[0] < dist_max &&
#       Math.abs(dists_curr[0] - dists_prev[0]) < d_dist_dt_max) {
#     F[0] = (dists_curr[0] - dists_prev[0]) / dt;
#   } else {
#     F[0] = 0;
#   }

#   console.log("");
#   console.log("### NEW ###");
#   console.log(dists_curr[0], dists_prev[0]);
#   dists_prev[0] = dists_curr[0];

#   // Pre-calc heading vector
#   var heading = theta.z;
#   var sn = Math.sin(heading);
#   var cs = Math.cos(heading);

#   // linear
#   var paddle_force = F[0];
#   var drag = vel.scaled(Cd).scaled(-1).scaled(vel.norm());
#   var acc = new Vec3(paddle_force * cs, paddle_force * sn, 0).add(drag).scaled(1/mass);
#   vel = acc.scaled(dt).plus(vel);
#   pos = vel.scaled(dt).plus(pos);