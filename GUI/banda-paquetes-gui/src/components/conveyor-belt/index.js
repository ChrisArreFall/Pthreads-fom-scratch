import React, { Component } from 'react';
import '../../index.css';

class ConveyorBelt extends Component {
  constructor(props) {
    super(props);
    this.state = { 
        data: JSON.parse(this.props.data)
      };
    this.CBGenerator = this.CBGenerator.bind(this);
  }

  CBGenerator(){
    var list_items = []
    for(var i = 0;i<this.state.data.tamano;i++){
      if(i===this.state.data.paquete.pos){
        list_items.push(
          <button className="segment" style={{backgroundColor:"red"}}>
            {this.state.data.paquete.id}
          </button>)
      }
      else{
        list_items.push(
          <button className="segment" style={{backgroundColor:"blue"}}>
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
    return (
      <div className="welcome-component">
        {this.CBGenerator()}
        {this.props.data}
      </div>
    );
  }
}

export default ConveyorBelt;