import React from 'react'
import {View, TestModule} from 'react-native'
import WebSocketHelper from './webSocketHelper'

const WebSocketSend = (props) => {
  
  return(
    <View>
      <WebSocketHelper flag = {props.flag}/>
    </View>
  );
}

export default WebSocketSend