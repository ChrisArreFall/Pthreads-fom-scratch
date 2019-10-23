import React, { Component } from 'react';

class Segment extends Component {
  constructor(props) {
    super(props);
    this.state = { message: 'Welcome to the Welcome Page!'};
  }
  
  render() {
    return (
      <div className="welcome-component">
        <Button variant="dark">{message}</Button>
      </div>
    );
  }
}

export default Segment;