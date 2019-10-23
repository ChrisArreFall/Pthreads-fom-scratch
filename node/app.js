const fs = require('fs');

const express = require("express");
const http = require("http");
const socketIo = require("socket.io");
const axios = require("axios");
const port = process.env.PORT || 4001;
const index = require("./routes/index");
const app = express();
app.use(index);
const server = http.createServer(app);
const io = socketIo(server); // < Interesting!

/*
function sendFile() {
  var data = fs.readFileSync('../data/1.txt', 'utf8');
  console.log(data);
}
setInterval(sendFile, 400);
*/

let interval;
io.on("connection", socket => {
  console.log("New client connected");
  if (interval) {
    clearInterval(interval);
  }
  interval = setInterval(() => getApiAndEmit(socket), 400);
  socket.on("disconnect", () => {
    console.log("Client disconnected");
  });
});

server.listen(port, () => console.log(`Listening on port ${port}`));


const getApiAndEmit = async socket => {
  try {
    var data1 = fs.readFileSync('../data/1.txt', 'utf8');
    var data2 = fs.readFileSync('../data/2.txt', 'utf8');
    var data3 = fs.readFileSync('../data/3.txt', 'utf8');
    var result = "["+data1+","+data2+","+data3+"]";
    console.log(result);
    socket.emit("FromAPI", result); // Emitting a new message. It will be consumed by the client
  } catch (error) {
    console.error(`Error: ${error.code}`);
  }
};