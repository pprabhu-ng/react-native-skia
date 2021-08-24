import React, {useEffect} from 'react'
import {View, Text, StyleSheet, Animated} from 'react-native'
import Config from './config.json'

let resolution = Config.size.low;
let fontSize1 = resolution.textStyle.fontSize;

const TextApp = (props) =>{
 
  let value = new Animated.ValueXY({x:0,y:10})  
  let test = 1;
  let text1 = <Text />;
  let text2 = <Text />;
  let text3 = <Text />;
  let text4 = <Text />;
  let text5 = <Text />;
  let text6 = <Text />;
  let text7 = <Text />;
  let textBackground = Config.textApp.textBackground;
  let bColor = '';
  let justifyContent = 'center';
  let alignItems = 'center';
  let width1 = '90%';
  let margin = 0;

  useEffect( () =>   {
    console.log("---moveRect:" + fontSize1)
    if(props.flag == 3) {
      Animated.loop(Animated.timing(value,{
        toValue: {x:150, y:10},
        duration: 4000,
        velocity: 1,
        tension: 40,
        useNativeDriver:false
      }),{}).start();
    }
  })

    console.log("Flag: ", props.flag, "fontSize1:", fontSize1);
    if(props.flag == 0) {
      text1 = <Text style={[styles.textStyle, {color:'white', fontSize:fontSize1, textShadowRadius:0, textShadowColor:'#3F454C', textShadowOffset:{width:3,height:3}}]} > Text Properties </Text>
      bColor = Config.main.tilesBackground;
      justifyContent = 'center';
      alignItems = 'center';
      width1 = '100%';
      textBackground = '';
    } else if(props.flag == 1){
        textBackground = '';
        justifyContent = 'center';
        alignItems = 'center';
        margin = 5;
        text1 =  <Text  style={[styles.textStyle, { color:'orange' }]} > Default Letter Spacing. </Text>
        text2 = <Text style={[styles.textStyle, { letterSpacing:5, fontFamily: "Arial", color:'orange' }]}>Letter Spacing Value as 5</Text>
        text3 =<Text style={[styles.textStyle, { letterSpacing:-3 , color:'orange'}]}>Letter Spacing Value as -3. </Text>
        bColor = Config.main.focusBackground;
    } else if(props.flag == 2){
        justifyContent = 'center';
        alignItems = 'flext-start';
        margin = 5
        bColor = Config.main.focusBackground;
        text1 = <Text style={[styles.textStyle,{ color:'black'}]}>Default lineHeight</Text>
        text2 = <Text style={[styles.textStyle, { lineHeight:40, color:'black' }]}>lineHeight:40</Text>
        text3 = <Text style={[styles.textStyle, { lineHeight:30,color:'black'}]}>lineHeight:30</Text>
        text4 = <Text style={[styles.textStyle, { lineHeight:20, color:'black'}]}>lineHeight:30</Text>
        text5 = <Text style={[styles.textStyle, { lineHeight:12, color:'black'}]}>lineHeight:12</Text>
        text6 = <Text style={[styles.textStyle, { lineHeight:5, color:'black'}]}>lineHeight:5</Text>
        text7 = <Text style={[styles.textStyle, { lineHeight:0,color:'black'}]}>lineHeight:0</Text>
    } else if(props.flag == 3){
        textBackground = '';
        justifyContent = 'center';
        alignItems = 'center';
        bColor = Config.main.focusBackground;
        text4 = <Text style={{color:'white', fontWeight:'bold', fontFamily: 'Arial', fontSize:fontSize1 - 4, margin: 20}}>Font Properties with font size {fontSize1 - 4}</Text>
        text5 = <Text style={{fontWeight:'bold', color:'orange', fontFamily: 'Arial', fontSize:fontSize1 -4}}>fontFamily: Arial</Text>
        text6 = <Text style={{ lineHeight:40 ,  color:'white', fontStyle:'italic', fontSize:fontSize1 -4}}>fontStyle: Italic</Text>
        text7 = <Text style={{ lineHeight:30, fontWeight:'bold', color:'green', fontSize:fontSize1 -4 }}>fontWeight: Bold</Text>
    } 


return (
  <View style={[ styles.MainContainer,{ flex:1, flexDirection:'row' ,justifyContent:justifyContent, alignItems:alignItems, backgroundColor:bColor, borderWidth:5, borderRadius:10, borderColor:bColor,} ]}>
        <View style={{justifyContent:justifyContent, alignItems:alignItems}}>
          <View style={{ margin:margin, backgroundColor: textBackground }}>
            {text1}
          </View>
          <View style={{ margin:margin, backgroundColor: textBackground }}>
            {text2}
          </View>
          <View style={{ margin:margin, backgroundColor: textBackground }}>
            {text3}
          </View>
        </View>
        <View style={{justifyContent:justifyContent, alignItems:alignItems}}>
          <View style={{ margin:margin, backgroundColor: textBackground }}>
            {text4}
          </View>
          <View style={{ margin:margin, backgroundColor: textBackground }}>
            {text5}
          </View>
          <View style={{ margin:margin, backgroundColor: textBackground }}>
            {text6}
          </View>
          <View style={{ margin:margin, backgroundColor: textBackground }}>
            {text7}
          </View>
        </View>
  </View>
);
}

const styles = StyleSheet.create({
  MainContainer: {
    width: resolution.maincontainer.width,
    height: resolution.maincontainer.height
  },
  textStyle: {
    fontSize: fontSize1 - 4,
    color: Config.textApp.textColor, 
    fontWeight: 'bold',
    margin: 1
  }
});

export default TextApp