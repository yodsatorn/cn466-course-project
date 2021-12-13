const express = require("express");
const mqtt = require("mqtt");
const axios = require("axios");
const Module = require("./edge-impulse-standalone");

const app = express();

// Express
app.use(express.json());

app.get('/', (req, res) => {
  res.json({ message: 'Hello world!' })
})

// MQTT
const mqttClient = mqtt.connect("mqtt://broker.hivemq.com");
const TOPIC_MQTT = "cn466/yodsatorn/cucumber/#";

// Classifier module
let classifierInitialized = false;
Module.onRuntimeInitialized = function () {
  classifierInitialized = true;
};

class EdgeImpulseClassifier {
  _initialized = false;

  init() {
    if (classifierInitialized === true) return Promise.resolve();

    return new Promise((resolve) => {
      Module.onRuntimeInitialized = () => {
        resolve();
        classifierInitialized = true;
      };
    });
  }

  classify(rawData, debug = false) {
    if (!classifierInitialized) throw new Error("Module is not initialized");

    const obj = this._arrayToHeap(rawData);
    let ret = Module.run_classifier(
      obj.buffer.byteOffset,
      rawData.length,
      debug
    );
    Module._free(obj.ptr);

    if (ret.result !== 0) {
      throw new Error("Classification failed (err code: " + ret.result + ")");
    }

    let jsResult = {
      anomaly: ret.anomaly,
      results: [],
    };

    for (let cx = 0; cx < ret.size(); cx++) {
      let c = ret.get(cx);
      jsResult.results.push({
        label: c.label,
        value: c.value,
      });
      c.delete();
    }

    ret.delete();

    return jsResult;
  }

  getProperties() {
    return Module.get_properties();
  }

  _arrayToHeap(data) {
    let typedArray = new Float32Array(data);
    let numBytes = typedArray.length * typedArray.BYTES_PER_ELEMENT;
    let ptr = Module._malloc(numBytes);
    let heapBytes = new Uint8Array(Module.HEAPU8.buffer, ptr, numBytes);
    heapBytes.set(new Uint8Array(typedArray.buffer));
    return { ptr: ptr, buffer: heapBytes };
  }
}

var features = [];

mqttClient.on("connect", () => {
  console.log("HIVEMQ connected");
  mqttClient.subscribe([TOPIC_MQTT], () => {
    console.log("Topic subscribed");
  });
});

let cucumber = [];

mqttClient.on("message", async (topic, payload) => {
  // Data from payload
  let data = JSON.parse(payload);
  if (topic == "cn466/yodsatorn/cucumber/status") {
    //console.log("from cucumber");
    cucumber[0] = data;
    //console.log(data)
  }

  let accx = data.acceleration[0];
  let accy = data.acceleration[1];
  let accz = data.acceleration[2];
  let angx = data.angular_velocity[0];
  let angy = data.angular_velocity[1];
  let angz = data.angular_velocity[2];

  features.push(accx, accy, accz, angx, angy, angz);
  //console.log(features.length);

  // if feature.lenghth +6 = x then do
  if (features.length > 294) {
    //let result_val = predict(features);
    predict(features);
    features = [];
  }
});
// END MQTT

function predict(features) {
  let feature = features.toString();
  // Initialize the classifier, and invoke with the argument passed in
  let classifier = new EdgeImpulseClassifier();
  classifier
    .init()
    .then(async () => {
      let result = classifier.classify(
        feature
          .trim()
          .split(",")
          .map((n) => Number(n))
      );
      console.log(result);
      checkMoving(result);
    })
    .catch((err) => {
      console.error("Failed to initialize classifier", err);
    });
}

// variable for check alert
var alert = false

function checkMoving(result) {
  let mFlat = result.results[0].value;
  let mUpDown = result.results[1].value;
  let mWave = result.results[2].value;
  let vertical = result.results[3].value;
  if (vertical < 0.6 && (mFlat > 0.7 || mUpDown > 0.7 || mWave > 0.7)) {
    alert=true
  }
  else if(vertical > 0.6 && (mFlat < 0.7 && mUpDown < 0.7 && mWave < 0.7)){
    alert=false
  }
  if(alert){
    axios
      .post(
        "https://api.line.me/v2/bot/message/broadcast",
        {
          messages: [
            {
              type: "text",
              text: "แจ้งเตือน! กระถางมีการเคลื่อนที่หรืออาจถูกเคลื่อนย้าย",
            },
            {
              type: "sticker", 
              packageId: "11538", 
              stickerId: "51626511"
            },
          ],
        },
        {
          headers: {
            "Content-Type": "application/json",
            Authorization: `Bearer {${process.env.CHANNEL_ACCESS_TOKEN}}`,
          },
        }
      )
      .then(console.log("Line Alerted!"))
      .catch((err) => {
        console.log(err.response.data);
      });
  }
}

const port = process.env.PORT;

app.all("*", (req, res) => {
  res.send({ message: "Invalid URL maybe you should check it." }, 404);
});

app.listen(port, () => {
  console.log(`listening on ${port}`);
});