const mqtt = require("mqtt");
const fs = require("fs");
const readline = require("readline");

// Keyevent
readline.emitKeypressEvents(process.stdin);

if (process.stdin.isTTY) process.stdin.setRawMode(true);

// MQTT
const mqttClient = mqtt.connect("mqtt://broker.hivemq.com");

// TOPIC
const TOPIC = ["cn466/yodsatorn/cucumber/status"];

// FS
var stream = fs.createWriteStream("vertical.csv", { flags: "a" });

// Write header
stream.write("timestamp,accX,accY,accZ,anvX,anvY,anvZ\r\n");

// Counter
var count = 0;

mqttClient.on("connect", () => {
  console.log("HIVEMQ connected!");
  mqttClient.subscribe(TOPIC, () => {
    console.log("Topic subscribed!");
  });
});

mqttClient.on("message", (topic, payload) => {
  let data = JSON.parse(payload);
  delete data.pressure;
  delete data.temperature;
  delete data.humidity;

  console.log(
    count,
    "acceleration: ",
    data.acceleration,
    " ",
    "angular_velocity: ",
    data.angular_velocity
  );

  // Count for element
  stream.write(count + ",");

  data.acceleration.forEach((element) => {
    stream.write(element + ",");
  });

  let c = 0;
  data.angular_velocity.forEach((element) => {
    c++;
    if (c == 3) {
      stream.write(element.toString());
    } else {
      stream.write(element + ",");
    }
  });

  stream.write("\r\n");
  count = count + 100;
});

mqttClient.on("end", () => {
  console.log("MQTT disconnect");
});

process.stdin.on("keypress", (chunk, key) => {
  if (key && key.name == "q") {
    console.log("End");
    mqttClient.end();
    process.exit();
  } 
});
