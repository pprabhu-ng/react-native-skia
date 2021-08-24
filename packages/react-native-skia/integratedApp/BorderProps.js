import React, {useEffect} from 'react'
import {View, AppRegistry, Text, StyleSheet, Animated} from 'react-native'
import Config from './config.json'

let resolution = Config.size.low;
let fontSize1 = resolution.textStyle.fontSize;

const BorderProps = (props) => {

  var test = 0;
  let text = <Text /> 
  let subViewWidth= resolution.maincontainer.width * 0.6;
  let subViewHeight= resolution.maincontainer.height * 0.6;
  let bgColor= Config.main.tilesBackground;
  var mVbgColor = Config.main.mainBackground
  
  let baseOpacity=0.8;

  let textColor = 'white';

  let borderRadius=0;
  let borderWidth=0;

  let borderLeftColor = '';
  let borderRightColor = '';
  let borderBottomColor = '';
  let borderTopColor = '';

  let borderTopWidth = 0;
  let borderRightWidth = 0;
  let borderLeftWidth = 0;
  let borderBottomWidth = 0;
  let justifyContent = 'center';
  let alignItems = 'center';

  let value = new Animated.ValueXY({x:0,y:0})

  useEffect( () =>   {
    console.log("---moveRect:" + JSON.stringify(value))
    if(props.flag != 0 && props.flag != 1) {
      Animated.timing(value,{
        toValue: {x:subViewWidth * .3, y:subViewHeight * .25},
        duration: 2000,
        velocity: 1,
        tension: 40,
        useNativeDriver:false
      }).start();
    } else {
        Animated.timing(value,{
        toValue: {x:0, y:0},
        duration: 2000,
        velocity: 1,
        tension: 40,
        useNativeDriver:false
      }).start();
    }
  })


    if(props.flag == 0) {
      text = <Text style={[styles.textStyle, {color:'white', fontSize:fontSize1, textShadowRadius:0, textShadowColor:'#3F454C', textShadowOffset:{width:3,height:3}}]} > Border Properties</Text>
      console.log("j = ", justifyContent, alignItems);
      mVbgColor = Config.main.tilesBackground;
    }else if(props.flag == 1) {
      textColor = 'black';
      text = <Text> With Border </Text>;
      bgColor='#1a7599' ;
      borderWidth = 30 ;
      borderLeftColor ='#2196c4';
      borderRightColor ='#2196c4';
      borderBottomColor ='#51b8e1';
      borderTopColor ='#51b8e1'; 
      borderTopWidth= 30; 
      borderRightWidth=30;
      borderLeftWidth=30;
      borderBottomWidth= 30;
      justifyContent = 'flex-start';
      alignItems = 'flex-start';
      console.log("j = ", justifyContent, alignItems);
    }  else if(props.flag == 2) {
      text = <Text> With different border width </Text>;
      textColor = 'black';
      bgColor='#1a7599' ;
      borderLeftColor ='#2196c4';
      borderRightColor ='#2196c4';
      borderBottomColor ='#51b8e1';
      borderTopColor ='#51b8e1';
      borderTopWidth= 15; 
      borderRightWidth=30;
      borderLeftWidth=40;
      borderBottomWidth= 50;
      justifyContent = 'flex-start';
      alignItems = 'flex-start';
      console.log("j = ", justifyContent, alignItems);
    } else if(props.flag == 3){

      text = <Text> With Border radius </Text>
      textColor = 'black';
      bgColor ='#1a7599';
      borderLeftColor='#2196c4';
      borderRightColor='#2196c4';
      borderBottomColor='#51b8e1';
      borderTopColor='#51b8e1';  
      borderTopWidth= 30; 
      borderRightWidth=30;
      borderLeftWidth=30;
      borderBottomWidth= 30;
      borderRadius = 20;
      justifyContent = 'center';
      alignItems ='center';
      console.log("j = ", justifyContent, alignItems);
    }

    return (
        <View style={[styles.MainContainer, {flex: 1, justifyContent:justifyContent, alignItems:alignItems, backgroundColor:mVbgColor, borderWidth:5, borderRadius:10, borderColor:mVbgColor}]}>
          <Animated.View style={value.getLayout()}>
            <View
            style={{ 
              width:subViewWidth ,
              height : subViewHeight,
              backgroundColor: bgColor,
              borderWidth:borderWidth,
              borderLeftColor:borderLeftColor,
              borderRightColor:borderRightColor,
              borderBottomColor:borderBottomColor,
              borderTopColor:borderTopColor, 
              borderTopWidth: borderTopWidth, 
              borderRightWidth:borderRightWidth,
              borderLeftWidth:borderLeftWidth,
              borderBottomWidth: borderBottomWidth,
              borderRadius: borderRadius,
              justifyContent: 'center',
              alignItems: 'center'
            }}
          >
              {text} 
          </View> 
        </Animated.View>
      </View>
    );
        
}

const styles = StyleSheet.create({ 
  MainContainer: {
    width: resolution.maincontainer.width,
    height: resolution.maincontainer.height,
  }, 
  textStyle: {
    fontSize: fontSize1 - 5,
    color: 'black', 
    fontWeight: 'bold',
    margin: 10
  }
})

AppRegistry.registerComponent('BorderProps', () => BorderProps);
export default BorderProps