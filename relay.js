const dgram = require("dgram");
const Vec3 = require("vec3");
const WebSocket = require("ws");

const udpServer = dgram.createSocket("udp4");
const wss = new WebSocket.Server({ port: 8080 });

// States
var pos = new Vec3();
var vel = new Vec3();
var theta = new Vec3(0.0, 0.0, 0.0);
var theta_dot = new Vec3();

var t_prev = 0;

var mass = 1000;
var Cd = 1000;
const I = 1;

var first = true;

var dists_prev = {
  0: 0
};
var dists_curr = {
  0: 0
};
var F = {
  0: 0,
};

var dist_max = 100;
var d_dist_dt_max = 10;

udpServer.on("listening", () => {
  const address = udpServer.address();
  console.log(`UDP server listening on ${address.address}:${address.port}`);
  udpServer.setBroadcast(true);
});

udpServer.on("message", (msg) => {
  var [
    t_curr,
    dist_rear_left,
    dist_rear_right,
    dist_front_left,
    dist_front_right,
  ] = msg
    .toString("utf-8")
    .split(",")
    .map((value) => parseInt(value, 10) || null);

  // https://www.asawicki.info/Mirror/Car%20Physics%20for%20Games/Car%20Physics%20for%20Games.html
  // https://github.com/spacejack/carphysics2d/blob/master/public/js/Car.js#L121

  dists_curr[0] = dist_rear_left;
  if (first) {
    t_prev = t_curr;
    dists_prev[0] = dist_rear_left;
    // dists_prev["rear_right"] = dist_rear_right;
    // dists_prev["front_left"] = dist_front_left;
    // dists_prev["front_right"] = dist_front_right;
    first = false;
    return;
  }
  var dt = t_curr - t_prev;

  if (dists_prev[0] < dist_max && dists_curr[0] < dist_max &&
      Math.abs(dists_curr[0] - dists_prev[0]) < d_dist_dt_max) {
    F[0] = (dists_curr[0] - dists_prev[0]) / dt;
  } else {
    F[0] = 0;
  }

  console.log("");
  console.log("### NEW ###");
  console.log(dists_curr[0], dists_prev[0]);
  dists_prev[0] = dists_curr[0];

  // Pre-calc heading vector
  var heading = theta.z;
  var sn = Math.sin(heading);
  var cs = Math.cos(heading);

  // linear
  var paddle_force = F[0];
  var drag = vel.scaled(Cd).scaled(-1).scaled(vel.norm());
  var acc = new Vec3(paddle_force * cs, paddle_force * sn, 0).add(drag).scaled(1/mass);
  vel = acc.scaled(dt).plus(vel);
  pos = vel.scaled(dt).plus(pos);

  console.log(dt);
  console.log(F);
  console.log(drag);
  console.log(acc);
  console.log(vel);
  console.log(pos);

  t_prev = t_curr;

  wss.clients.forEach((client) => {
    if (client.readyState === WebSocket.OPEN) {
      let message = [pos.x, pos.y, theta.z].join(",");
      client.send(message);
    }
  });
});

udpServer.bind(5555);
