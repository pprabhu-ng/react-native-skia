/**
 * Sample React Native App
 */
 
 import * as React from 'react';
 import type {Node} from 'react';
 import {
         AppRegistry,
         Image,
         Text,
         View,
 } from 'react-native';
 import { useState } from 'react';
 
 const SimpleViewApp = React.Node = () => {
 var useCases =10;
    var timerValue=10000;
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
             style={{  width: 300,
                       height: 300,
                       margin:10,
                       backgroundColor:'cornsilk',
         }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200, 
                         height: 200 ,
                         
            }}/>
            <Text
           style={{ color: '#fff', fontSize: 30,
                       textAlign: 'center', marginTop: 32,color:'black' }}>
          Local Image</Text>
        </View>)
   }
   else if(UseCaseCount ==2)
   {
       return (
        <>
        <View
            style={{ width: 300,
                     height: 300,
                     margin:10,
                     backgroundColor:'cornsilk',  
            }}>
            <Image
                source={{uri: 'https://pngimg.com/uploads/lion/lion_PNG23269.png'}}
                style={{ width: 200,
                         height: 200,
                         
                         
               }}/>
            <Text
           style={{ color: '#fff', fontSize: 30,
                       textAlign: 'center', marginTop: 32,color:'black' }}>
          Network Image</Text>   
        </View>
  </>)

 }
   else if(UseCaseCount ==3)
   {
       return (
         <>
      <View
          style={{
                  flexDirection:'row',
          }}>
         <View
             style={{  width: 300,
                       height: 300,
                       margin:10,
                       backgroundColor:'cornsilk',
         }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200, 
                         height: 200 ,       
            }}/>

        </View>
        <View
            style={{ width: 300,
                     height: 300,
                     margin:10,
                     backgroundColor:'cornsilk',
                     
            }}>
            <Image
                source={{uri: 'https://png.pngtree.com/png-clipart/20210801/original/pngtree-resolution-logo-720p-tags-png-image_6578799.jpg  ',}}
                style={{ width: 200,
                         height: 200,     
               }}/>
        </View>
    </View>
    <View 
      style={{
                  flex:0.5,
                  flexDirection:'row',
          }}>
      <Text
           style={{ color: '#fff', fontSize: 30,
                       textAlign: 'center', marginTop: 32,color:'black' }}>
          Local Image  + Network Image </Text> 
    </View>
  </>)
   }
    else if(UseCaseCount ==4)
   {
       return (
          <>
      <View
          style={{
                  flex:0.5,
                  flexDirection:'row',
          }}>
         <View
             style={{  width: 300,
                       height: 300,
                       backgroundColor:'cornsilk',
         }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200, 
                         height: 200 ,       
            }}/>
        </View>
        <View
            style={{ width: 300,
                     height: 300,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
                     
            }}>
            <Image
                source={{uri: 'http://10.0.2.15:8001/gangster_PNG70.png'}}
                style={{ width: 200,
                         height: 200,      
               }}/>
        </View>
    </View>
    <View 
      style={{
                  flexDirection:'row',
          }}>
      <Text
           style={{ color: '#fff', fontSize:30,
                       textAlign: 'center', marginTop: 32,color:'black' }}>
          Local Image  + Network Image no-cache </Text> 
    </View>
  </>)
   }
   else if(UseCaseCount ==5)
   {
       return (
         <>
      <View
          style={{
                  flexDirection:'row',
          }}>
         <View
             style={{  width: 300,
                       height: 300,
                       margin:10,
                       backgroundColor:'cornsilk',
         }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200, 
                         height: 200 ,       
            }}/>
        </View>
        <View
            style={{ width: 300,
                     height: 300,
                     margin:10,
                     backgroundColor:'cornsilk',
                     
            }}>
            <Image
                source={{uri: 'http://10.0.2.15:8002/ninja_PNG10.png'}}
                style={{ width: 200,
                         height: 200,      
               }}/>
        </View>
    </View>
    <View 
      style={{
                  flexDirection:'row',
          }}>
      <Text
           style={{ color: '#fff', fontSize: 30,
                       textAlign: 'center', marginTop: 32,color:'black' }}>
          Local Image  + Network Image no-store </Text> 
    </View>
  </>)
   }
   else if(UseCaseCount ==6)
   {
       return (
         <>
      <View
          style={{
                  flexDirection:'row',
          }}>
         <View
             style={{  width: 300,
                       height: 300,
                       margin:10,
                       backgroundColor:'cornsilk',
         }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200, 
                         height: 200 ,       
            }}/>
        </View>
        <View
            style={{ width: 300,
                     height: 300,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
                     
            }}>
            <Image
                source={{uri: 'http://10.0.2.15:8003/2022_year_PNG23.png'}}
                style={{ width: 200,
                         height: 200,       
               }}/>
        </View>
    </View>
    <View 
      style={{
                  flexDirection:'row',
          }}>
      <Text
           style={{ color: '#fff', fontSize: 30,
                       textAlign: 'center', marginTop: 32,color:'black' }}>
          Local Image  + Network Image max-age=0 </Text> 
    </View>
  </>)
   }
   else if(UseCaseCount ==7)
   {
       return (
         <>
      <View
          style={{
                  flexDirection:'row',
          }}>
         <View
             style={{  width: 300,
                       height: 300,
                       margin:10,
                       backgroundColor:'cornsilk',
         }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200, 
                         height: 200 ,       
            }}/>
        </View>
        <View
            style={{ width: 300,
                     height: 300,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
                     
            }}>
            <Image
                source={{uri:'http://10.0.2.15:8004/football_PNG52796.png'}}
                style={{ width: 200,
                         height: 200,
               }}/>
        </View>
    </View>
    <View 
      style={{
                  flex:0.5,
                  flexDirection:'row',
          }}>
      <Text
           style={{ color: '#fff', fontSize: 30,
                       textAlign: 'center', marginTop: 32,color:'black' }}>
          Local Image  + Network Image max-age less than 5min </Text> 
    </View>
  </>)
   }
   else if(UseCaseCount ==8)
   {
       return (
         <>
      <View
          style={{
                  flexDirection:'row',
          }}>
         <View
             style={{  width: 300,
                       height: 300,
                       margin:10,
                       backgroundColor:'cornsilk',
         }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200, 
                         height: 200 ,       
            }}/>
        </View>
        <View
            style={{ width: 300,
                     height: 300,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
                     
            }}>
            <Image
                source={{uri: 'http://10.0.2.15:8005/robot_PNG94.png'}}
                style={{ width: 200,
                         height: 200,      
               }}/>
        </View>
    </View>
    <View 
      style={{
                  flex:0.5,
                  flexDirection:'row',
          }}>
      <Text
           style={{ color: '#fff', fontSize: 30,
                       textAlign: 'center', marginTop: 32,color:'black' }}>
          Local Image  + Network Image max-age = 5min </Text> 
    </View>
  </>)
   }
   else if(UseCaseCount ==9)
   {
       return (
         <>
      <View
          style={{
                  flexDirection:'row',
          }}>
         <View
             style={{  width: 300,
                       height: 300,
                       margin:10,
                       backgroundColor:'cornsilk',
         }}>
            <Image
                source={require('./disney-1-logo-png-transparent.png')}
                style={{ width: 200, 
                         height: 200 ,       
            }}/>
        </View>
        <View
            style={{ width: 300,
                     height: 300,
                     marginLeft:10,
                     backgroundColor:'cornsilk',
                     
            }}>
            <Image
                source={{uri: 'http://10.0.2.15:8006/github_PNG75.png'}}
                style={{ width: 200,
                         height: 200,    
               }}/>
        </View>
    </View>
    <View 
      style={{
                  flexDirection:'row',
          }}>
      <Text
           style={{ color: '#fff', fontSize: 30,
                       textAlign: 'center', marginTop: 32,color:'black' }}>
          Local Image  + Network Image max-age greater than 5min </Text> 
    </View>
  </>)
   }
       else
   {
       return (
        <View
            style={{
            justifyContent: 'center',
            alignItems: 'center',
            borderWidth: 2,
           // backgroundColor: 'darksalmon',
            height: 500,
            width: 500,
            margin:10,
         }}>
            <Image
                style={{ width:300, height: 300, backgroundColor: 'gray', }}
               source={require('react-native/Libraries/NewAppScreen/components/logo.png')}/>
            <Text
                style={{ color: '#fff', fontSize: 46,
                textAlign: 'center', marginTop: 32,color:'black' }}>
                React Native loves Skia</Text>
        </View>)
    }
  }
  return (
    renderMainView()
   );
 };
AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);
