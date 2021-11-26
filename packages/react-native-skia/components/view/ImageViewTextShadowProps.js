import * as React from 'react';
import { View, AppRegistry, Image, Text } from 'react-native';

const SimpleViewApp = React.Node = () => {
  return (
      <View
          style={{
                  flex:1,
                  flexDirection:'row',
          }}>

         <View
             style={{  width: 300,
                       height: 300,
                       borderLeftWidth:10,
                       borderBottomWidth:10,
                       borderColor:'#fff700',
                       shadowColor:'aqua',
                       shadowOpacity:1,
                       shadowOffset:{
                                     width:10,
                                     height:10
                       },
         }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200, 
                         height: 200 ,
                         shadowColor: "black",
                         shadowRadius: 1,
                         shadowOpacity: 1,
                         borderWidth:5,
                         borderColor:'#ff0000',
                         shadowOffset: {
                                       width: 10,
                                       height: 10}
            }}/>
           
           <Text 
               style={{
                        color: '#ffa500',
                        fontSize: 20,
                        shadowColor: "red",
                        shadowRadius: 0,
                        shadowOpacity: 1,
                        shadowOffset: {
                                       width: 10,
                                       height: 10},
                        textShadowColor:'green',
                        textShadowOffset:{
                                          width:10,
                                          height:10
                        },
                        textAlign: 'center',
                        marginTop: 32
               }}>
                      React Native loves Skia
          </Text>
        </View>
        
        <View
            style={{ width: 300,
                     height: 300,
                     marginLeft:30,
                     borderLeftWidth:10,
                     borderBottomWidth:10,
                     borderColor:'#ffd700',
                     shadowColor:'cyan',
                     shadowOpacity:1,
                     backgroundColor:'cornsilk',
                     shadowOffset:{
                                   width:10,
                                   height:10
                     },
            }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200,
                         height: 200,
                         shadowColor:"black",
                         shadowRadius:1,
                         shadowOpacity:1,
                         borderWidth:5,
                         backgroundColor:'#ffe4e1',
                         borderColor:'black',
                         shadowOffset: {
                                         width: 10,
                                         height: 10}
               }}/>
            <Text
               style={{
                       color: '#ffa500',
                       fontSize: 20,
                       shadowColor: "red",
                       shadowRadius: 0,
                       shadowOpacity: 1,
                       shadowOffset: {
                                      width: 20,
                                      height: 20},
                       textShadowColor:'green',
                       textShadowOffset:{
                                       width:10,
                                       height:10
                       },
                       textAlign:'center',
                       marginTop:32
               }}>
                      React Native loves Skia
           </Text>
        </View>
    </View>
  );
};
AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);
