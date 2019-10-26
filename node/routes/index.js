const express = require("express");
var fs = require('fs');
var path = require('path');
const router = express.Router();

/*router.get("/", (req, res) => {
  res.send({ response: "I am alive" }).status(200);
});
module.exports = router;*/

router.get("/", (req, res) => {
  try {
    var jsonPath1 = path.join(__dirname, '..', '..', 'data', '1.txt');
    var jsonPath2 = path.join(__dirname, '..', '..', 'data', '2.txt');
    var jsonPath3 = path.join(__dirname, '..', '..', 'data', '3.txt');
    var data1 = fs.readFileSync(jsonPath1, "utf8");
    var data2 = fs.readFileSync(jsonPath2, "utf8");
    var data3 = fs.readFileSync(jsonPath3, "utf8");
    var result = "[" + data1 + "," + data2 + "," + data3 + "]";
    //console.log("Request from GET");
    console.log(result);
    res.send(result).status(200);
    res.end();
  } catch (error) {
    console.error(`Error: ${error.code}`);
  }
});
module.exports = router;
