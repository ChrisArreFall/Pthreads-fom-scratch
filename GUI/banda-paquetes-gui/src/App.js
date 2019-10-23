import React, { Component } from "react";
import socketIOClient from "socket.io-client";


class App extends Component {
  constructor() {
    super();
    this.state = {
      response: false,
      endpoint: "http://127.0.0.1:4001"
    };
  }
  componentDidMount() {
    const { endpoint } = this.state;
    const socket = socketIOClient(endpoint);
    socket.on("FromAPI", data => this.setState({ response: data }));
  }
  CBGenerator(data){
    var list_items = []
    for(var i = 0;i<data.tamano;i++){
      //0 radioactivo, 1 urgente, 2 normal
      if(i===data.paquete.pos){
        if(data.paquete.tipo===0){
          list_items.push(
            <button className="segment" style={{backgroundColor:"red"}}>
              ID = {data.paquete.id} <br />
              M = {data.paquete.masa} kg <br />
              T = {data.paquete.tipo}
            </button>)
        }
        else if(data.paquete.tipo===1){
          list_items.push(
            <button className="segment" style={{backgroundColor:"yellow"}}>
              ID = {data.paquete.id} <br />
              M = {data.paquete.masa} kg <br />
              T = {data.paquete.tipo}
            </button>)
        }
        else{
          list_items.push(
            <button className="segment" style={{backgroundColor:"green"}}>
              ID = {data.paquete.id} <br />
              M = {data.paquete.masa} kg <br />
              T = {data.paquete.tipo}
            </button>)
        }
      }
      
      else{
        list_items.push(
          <button className="segment" style={{backgroundColor:"blue"}}>
            -<br />
            -<br />
            -
          </button>)
      }
    }
    return (
        <div className="conveyor-belt">
            {list_items}
        </div>
    )
  }
  render() {
    const { response } = this.state;
    return (
        <div style={{ textAlign: "center" }}>
          {response
              ? <p>
                
                {this.CBGenerator(JSON.parse(response)[0])}
                {JSON.parse(response)[0].letrero}
                
                {this.CBGenerator(JSON.parse(response)[1])}
                {JSON.parse(response)[1].letrero}
                
                {this.CBGenerator(JSON.parse(response)[2])}
                {JSON.parse(response)[2].letrero}
              </p>
              : <p>Loading...</p>}
        </div>
    );
  }
}
export default App;