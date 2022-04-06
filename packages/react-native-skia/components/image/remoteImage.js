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
import { View, AppRegistry, Image, Text } from 'react-native';

const SimpleViewApp = React.Node = () => {
  return (
   <>
      <View
          style={{
                  flex:0.5,
                  flexDirection:'row',
          }}>  
        <View
            style={{ width: 250,
                     height: 250,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
            }}>
            <Image
                source={{uri: 'https://pngimg.com/uploads/lion/lion_PNG23269.png',}}
                style={{ width: 200,
                         height: 200,
                         backgroundColor:'ivory',
               }}/>
        </View>
        <View
            style={{ width: 250,
                     height: 250,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
            }}>
            <Image
                source={{uri: 'https://pngimg.com/uploads/tiger/tiger_PNG23244.png',}}
                style={{ //width: 200,
                         height: 200,
                         backgroundColor:'yellow',
               }}/>
        </View>
         <View
            style={{ width: 250,
                     height: 250,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
            }}>
            <Image
                source={{uri: 'https://pngimg.com/uploads/mango/mango_PNG9173.png',}}
                        style={{ width: 200,
                         height: 200,
                         backgroundColor:'mintcream',
               }}/>
        </View>
        </View>

       <View
          style={{
                  flex:0.5,
                  flexDirection:'row',
          }}>
        <View
            style={{ width: 250,
                     height: 250,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
            }}>
            <Image
                source={{uri: 'https://pngimg.com/uploads/panda/panda_PNG10.png',}}
                style={{  width: 200,
                         height: 200,
                         backgroundColor:'blue',
               }}/>
        </View>
        <View
            style={{ width: 250,
                     height: 250,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
            }}>
            <Image
                source={{uri: 'https://pngimg.com/uploads/gorilla/gorilla_PNG18708.png',}}
                style={{ width: 200,
                         height: 200,
                         backgroundColor:'red',
               }}/>
        </View>
         <View
            style={{ width: 250,
                     height: 250,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
            }}>
            <Image
                source={{uri: 'https://pngimg.com/uploads/birds/birds_PNG95.png',}}
                style={{ width: 200,
                         height: 200,
                         backgroundColor:'green',
                }}/>
        </View>
        </View>
  </>
  );
};
AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);