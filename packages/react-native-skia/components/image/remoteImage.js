/*
https://pngimg.com/uploads/lion/lion_PNG23269.png
https://pngimg.com/uploads/tiger/tiger_PNG23244.png
https://pngimg.com/uploads/fish/fish_PNG25146.png
https://pngimg.com/uploads/arctic_fox/arctic_fox_PNG41376.png
https://pngimg.com/uploads/panda/panda_PNG10.png
https://pngimg.com/uploads/gorilla/gorilla_PNG18708.png
https://pngimg.com/uploads/mango/mango_PNG9173.png
https://pngimg.com/uploads/birds/birds_PNG95.png
*/
import * as React from 'react';
import type {Node} from 'react';
import {AppRegistry,Image,Text,View,} from 'react-native';
import { useState } from 'react';

var  subwidth = 800
var subheight =500
var widthImage=400;
var heightImage=400;[100,200,300,400]

const SimpleViewApp = React.Node = () => {
 var useCases =5;
 var timerValue=3000;
 const [UseCaseCount, toggleViewState] = useState(true);

 const timer = setTimeout(()=>{
   toggleViewState((UseCaseCount+1)%useCases);
 }, timerValue)

 var subViewWidth=150;
 var subViewHeight=300;

 const renderMainView = () => {
 if(UseCaseCount == 1)
   {
      return (
        <View
          style={{ width: 400,
                   height: 400,
                   marginLeft:40,
                   backgroundColor:'cornsilk',
          }}>
          <Image
            source={{uri: 'https://pngimg.com/uploads/lion/lion_PNG23269.png',}}
            style={{ width: 200,
                     height: heightImage[UseCaseCount],
                     backgroundColor:'ivory',
            }}/>
          <Text
            style={{ color: '#fff',
                     fontSize: 46,
                     textAlign: 'center',color: 'black',
                     marginTop: 20}}>
            Network Image
          </Text>
        </View>)
   }
 else if(UseCaseCount ==2)
   {
      return (
        <View
          style={{ width: 400,
                   height: 400,
                   marginLeft:10,
                   backgroundColor:'cornsilk',
          }}>
          <Image
            source={require('./disney-1-logo-png-transparent.png')}
            style={{ width: 200,
                     height: 200,
            }}/>
          <Text
            style={{ color: '#fff',
                     fontSize: 46,
                     textAlign: 'center',
                     color: 'black',
                     marginTop: 32 }}>
            Local Image
          </Text>
        </View>)
   }
 else if(UseCaseCount ==3)
   {
      return (
        <>
          <View
            style={{ flex:0.5,
                     flexDirection:'row',
            }}>
            <View
              style={{ width: 400,
                       height: 400,
                       marginLeft:30,
                       backgroundColor:'cornsilk',
              }}>
              <Image
                source={{uri: 'https://pngimg.com/uploads/tiger/tiger_PNG23244.png',}}
                style={{  width: 200,
                          height: 200,
                          backgroundColor:'blue',
              }}/>
            </View>
            <View
              style={{ width: 400,
                       height: 400,
                       marginLeft:30,
                       backgroundColor:'cornsilk',
              }}>
              <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200,
                         height: 200,
              }}/>
            </View>
          </View>
          <View
            style={{ flex:0.5,
                     flexDirection:'row',
            }}>
          <Text
            style={{ color: '#fff', fontSize: 46,
                     textAlign: 'center', marginTop: 10,
                     color:'black' }}>
            Network Image + local Image</Text>
          </View>
        </>)
   }
 else if(UseCaseCount ==4)
   {
      return (
        <View
            style={{ justifyContent: 'center',
                     alignItems: 'center',
                     borderWidth: 2,
                     height: subheight,
                     width: subwidth,
            }}>
            <Image
              style={{ width: widthImage, height: heightImage, backgroundColor: 'aquamarine', }}
              source={{uri: 'https://pngimg.com/uploads/lion/lion_PNG23269.png',}}/>
           <Text
              style={{ color: '#fff', fontSize: 46,color:'black',
                       textAlign: 'center', marginTop: 32 }}>
              Change network image in 3 sec ...
           </Text>
        </View>)
   } else {
     return (
       <View
         style={{ justifyContent: 'center',
                  alignItems: 'center',
                  borderWidth: 2,
                  height: subheight,
                  width: subwidth,
         }}>
         <Image
           style={{ width: widthImage, height: heightImage, backgroundColor: 'darksalmon', }}
           source={{uri: 'https://pngimg.com/uploads/mango/mango_PNG9173.png',}}/>
         <Text
           style={{ color: '#fff', fontSize: 46,
                    textAlign: 'center', marginTop: 32,color:'black', }}>
           Repeat again ....</Text>
       </View>)
   }
 }
return (
  renderMainView()
);
};
AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);