import React, {Component} from 'react';
import '../../App.css';


export default class MenuItem extends Component {
	constructor(props) {
		super(props);
		this.state = {
			status: this.props.stateP,
			precio: this.props.precio,
			id: this.props.id
		};
		this.handleChange = this.handleChange.bind(this);
		this.updateSelected = this.updateSelected.bind(this);
	}

	updateSelected() {
        console.log(this.state);
        this.props.handleSelectedItems(this.state);
	}

	handleChange() {
		this.setState({status: !this.state.status}, this.updateSelected);
	}

	render() {
		return (
			<div className="item-box">
                <input className = "check-box" type="checkbox" name={this.props.name} value={this.props.value} onChange={this.handleChange}/>
                <label className = "label-nombre"> {this.props.text} </label>
                <label className = "label-precio"> {this.props.precio} </label>
            </div>
		);
	}
}


