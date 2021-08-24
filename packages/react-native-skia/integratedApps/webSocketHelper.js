/**
 * Copyright (c) Facebook, Inc. and its affiliates.
 *
 * This source code is licensed URLr the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 *
 * @format
 * @flow
 */

'use strict';


//const fs = require('fs');
const React = require('react');
const ReactNative = require('react-native');
const {View, Text} = ReactNative;
const {TestModule} = ReactNative.NativeModules;
import Config from './config.json'

let resolution = Config.size.low;
let fontSize1 = resolution.textStyle.fontSize;

const DEFAULT_WS_URL = 'wss://localhost:9898';
const multi_url = 'ws://localhost:9898';
const WS_EVENTS = ['close', 'error', 'message', 'open'];
let TextSent = <Text />
let test222 = <Text />

type State = {
  url1: string,
  fetchStatus1: ?string,
  socket1: ?WebSocket,
  socketState1: ?number,
  lastSocketEvent1: ?string,
  lastMessage1: ?string | ?ArrayBuffer,
  testMessage1: string,
  testExpectedResponse1: string,
  testFlag: ?number,
  ...
};

class WebSocketTest extends React.Component<{...}, State> {

  state: State = {
    url1: multi_url,
    fetchStatus1: null,
    socket1: null,
    socketState1: null,
    lastSocketEvent1: null,
    lastMessage1: null,
    testMessage1: 'testMessage1',
    testExpectedResponse1: "",
    testFlag: 1,
  };

  _waitFor = (condition: any, timeout: any, callback: any) => {
    let remaining = timeout;
    const timeoutFunction = function() {
      if (condition()) {
        callback(true);
        return;
      }
      remaining--;
      if (remaining === 0) {
        callback(false);
      } else {
        setTimeout(timeoutFunction, 1000);
      }
    };
    setTimeout(timeoutFunction, 1000);
  };

 _connect1 = () => {
   console.log("URL: ", this.state.url1)
    const socket1 = new WebSocket(this.state.url1);
    console.log("Established new websocket",socket1);
    socket1.addEventListener("open", this._onSocketOpenEvent1);
    socket1.addEventListener("message", this._onSocketMessageEvent1);
    socket1.addEventListener("close", this._onSocketCloseEvent1);
    this.setState({
      socket1,
      socketState1: socket1.readyState,
      testFlag:0
    });
  };

  _socketIsConnected1 = () => {
    console.log("_socketIsConnected: ", this.state.socketState1);
    return this.state.socketState1 === 1; //'OPEN'

  };

  _disconnect = () => {
    console.error("_disconnect",this.state.socket1);
    if (!this.state.socket1) {
      return;
    }
    this.state.socket1.close();
  };

 
 _onSocketOpenEvent1 = (event: any) => {
    console.error("onsocket open event", event);
    const state: any = {
      socketState1: 1, //event.target.readyState,
      lastSocketEvent1: "open" //event.type,
    };
    this.setState(state);
  };

  _onSocketMessageEvent1 = (event: any) => {
    console.error("onsocket message event", event.data);
    const state: any = {
      socketState1: 1, //event.target.readyState,
      lastSocketEvent1: "message",
    };
    console.log("lastmessage: ", this.state.testExpectedResponse1);
    if (event.data) {
      //this.props.callback(event.data);
      this._disconnect();
      setTimeout(() => { this.setState({testExpectedResponse1:event.data});}, 2000)
    }
  };

  _onSocketCloseEvent1 = (event: any) => {
    console.error("onsocket close event", event);
    const state: any = {
      socketState1: 3, //event.target.readyState,
      lastSocketEvent1: "close", //event.type,
    };
    this.setState(state);
  };

  _sendText1 = (text: string) => {
    console.error("_sendText: ", this.state.socket1);
    if (!this.state.socket1) {
      return;
    }
    let payload = "Sending The Message to server";
    TextSent = <Text style={{fontSize:fontSize1 - 3, color:'white', fontWeight:'bold'}}> {payload} </Text>
    this.state.socket1.send(payload);
  };

  _sendTestMessage = () => {
    this._sendText(this.state.testMessage);
  };

  _sendTestMessage1 = () => {
    this._sendText1(this.state.testMessage1);
  };

  _receivedTestExpectedResponse1 = () => {
    console.error("_receivedTestExpectedResponse:");
    return this.state.lastMessage1 === this.state.testExpectedResponse1;
  };

  componentDidMount() {
    console.log("props.flag",this.props.flag );
  }

  componentDidUpdate(){
    console.log("props.flag3333",this.props.flag , "test:", this.state.testFlag);
      if(this.props.flag ==1 && this.state.testFlag == 1) {
        this.testConnect1();  
      }
  }

  testConnect1: () => void = () => {
    this._connect1();
    this._waitFor(this._socketIsConnected1, 5, connectSucceeded => {
      if (!connectSucceeded) {
        console.error("test Connect - connection not succeeded");
        return;
      }
      this.testSendAndReceive1();
    });
  };

  testSendAndReceive1: () => void = () => {
    this._sendTestMessage1();
    this._waitFor(this._receivedTestExpectedResponse1, 5, messageReceived => {
      if (!messageReceived) {
        console.error("test Connect - message not received");
        return;
      }
      //this.testDisconnect1();
    });
  };

   testDisconnect: () => void = () => {
    this._disconnect();
    console.error("testDisconnect");
    this._waitFor(this._socketIsDisconnected, 5, disconnectSucceeded => {
      //TestModule.markTestPassed(disconnectSucceeded);
    });
  };

  render(): React.Node {
    console.log("prop.flag11:", this.props.flag);
    if(this.props.flag == 0){
      TextSent = <Text style={{fontSize:fontSize1, color:'white', fontWeight:'bold', textShadowRadius:0, textShadowColor:'#3F454C', textShadowOffset:{width:3,height:3}}}> Websocket Testing </Text>
    } else if((this.props.flag == 2 || this.props.flag == 3)&& this.state.testFlag == 0) {
        const state: any = {
        testFlag : 1 ,
        testExpectedResponse1: ""
      };
      this.setState(state);
    } else {
      TextSent = <Text style={{fontSize:fontSize1 - 7, color:'white', fontWeight:'bold'}}> {this.state.testExpectedResponse1} </Text>
    }
    return  (
      <View style={{flex:1, justifyContent:'center', alignItems:'center'}}> 
        {TextSent}
      </View>
      );

  }
}

export default WebSocketTest