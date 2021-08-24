import React from 'react'
import {View, Text} from 'react-native'
import WebSocketHelper from './webSocketHelper'

const WebSocketReceive = (props) => {
  
  console.log("WebSocketTest: ",props.message);
  return(
    <View>
      <Text> {props.message}</Text>
    </View>
  );
  
}

export default WebSocketReceive