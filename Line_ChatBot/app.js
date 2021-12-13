const line = require("@line/bot-sdk");
const express = require("express");
const mqtt = require("mqtt");

const config = {
  channelAccessToken: process.env.CHANNEL_ACCESS_TOKEN,
  channelSecret: process.env.CHANNEL_SECRET,
};

const client = new line.Client(config);
const app = express();

// MQTT
const mqttClient = mqtt.connect("mqtt://broker.hivemq.com");

mqttClient.on("connect", () => {
  console.log("HIVEMQ connected");
  mqttClient.subscribe([process.env.TOPIC_ALL], () => {
    console.log("Topic subscribed");
  });
});

const cucumber = [];

mqttClient.on("message", async (topic, payload) => {
  // Data from payload
  let data = JSON.parse(payload);
  if (topic == "cn466/yodsatorn/cucumber/status") {
    console.log("from cucumber");
    cucumber[0] = data;
    console.log(cucumber);
  }
});
// END MQTT

app.post("/callback", line.middleware(config), (req, res) => {
  Promise.all(req.body.events.map(handleEvent))
    .then((result) => res.json(result))
    .catch((err) => {
      console.error(err);
      res.status(500).end();
    });
});

function handleEvent(event) {
  var eventText = event.message.text.toLowerCase();
  if (
    eventText === "cucumber" ||
    eventText === "status" ||
    eventText === "สถานะ"
  ) {
    var temp = cucumber[0].temperature;
    var humid = cucumber[0].humidity;
    var press = cucumber[0].pressure;
    const repMSG = [
      { type: "text", text: "Cucumber status" },
      { type: "text", text: "Temperature : " + temp.toString() },
      { type: "text", text: "Humidity : " + humid.toString() },
      { type: "text", text: "Pressure : " + press.toString() },
    ];
    return client.replyMessage(event.replyToken, repMSG);
  } else if (
    eventText === "temp" ||
    eventText === "temperature"
  ) {
    var temp1 = cucumber[0].temperature;
    const repMSG = [
      { type: "text", text: "Temperature : " + temp.toString() },
    ];
    return client.replyMessage(event.replyToken, repMSG);
  } else if (
    eventText === "humid" ||
    eventText === "humidity"
  ) {
    var humid1 = cucumber[0].humidity;
    const repMSG = [
      { type: "text", text: "Humidity : " + humid.toString() },
    ];
    return client.replyMessage(event.replyToken, repMSG);
  } else if (
    eventText === "press" ||
    eventText === "pressure"
  ) {
    var press1 = cucumber[0].pressure;
    const repMSG = [
      { type: "text", text: "Pressure : " + press.toString() },
    ];
    return client.replyMessage(event.replyToken, repMSG);
  }

  // create a echoing text message
  const echo = [
    { type: "text", text: "Hello, your message is" },
    { type: "text", text: event.message.text },
  ];

  // use reply API
  return client.replyMessage(event.replyToken, echo);
}

// LIFF UI
app.use(express.static("static"));

const port = process.env.PORT;

app.listen(port, () => {
  console.log(`listening on ${port}`);
});
