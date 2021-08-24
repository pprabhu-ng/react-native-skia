import React, { Component, useState, useRef, forwardRef, useImperativeHandle } from "react";
import { Animated, TVEventHandler, TouchableHighlight, Pressable, AppRegistry, Image, Text, View, StyleSheet, Button, SafeAreaView, ImageBackground} from "react-native";
import TextApp from './integratedApps/TextApp'
import Animation from './integratedApps/Animation'
import ShadowProps from './integratedApps/ShadowProps'
import BorderProps from './integratedApps/BorderProps'
import TextApp2 from './integratedApps/TextApp2'
import ImageProps from './integratedApps/ImageProps'
import WebSocketHelper from './integratedApps/webSocketHelper'
import Config from './integratedApps/config.json'

  let testBackground =  Config.main.mainBackground
  let mainBackground = testBackground;
  let tilesBackground = Config.main.tilesBackground;
  let focusBackground = Config.main.focusBackground;
  let shadowColor = Config.main.shadowColor;
  let resolution = Config.size.low;
  let fontSize1 = resolution.textStyle.fontSize;

const MyComponent = (props) =>  {

  let myRef = useRef(null);
  let top = 0;
  let left = 0;
  let WebSocketText = <Text/>
  let [width, setWidth] = useState(500);
  let [height, setHeight] = useState(300);
  let [pos, setPos] = useState('absolute')
  let subackground = testBackground;
  let [state, setState] = useState({
      bw:0,
      color1: 'red',
      bg: tilesBackground,
      sowidth: 0,
      soheight: 0,
      socketMessage: "NOT Yes Recived .. :)"
    });

  let [test, setTest] = useState(0)

  const changeCursor = (x,y, w, h, color) => {
    console.log("*********************************XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX**************************************")
    setState({bw:5})
  }

  const onPress1=(e) => {
    console.log("inOnpress--------------------" + " x:"+ top + " y:"+left)
    console.log("state::::::::" + JSON.stringify(props.layout.y))
    console.log("-----------"+ e )
    // if(props.layout.x == 10 && props.layout.y == 330 || props.layout.x == 1050 && props.layout.y == 0){
    //   setWidth(300);
    //   setHeight(200);
    // }
    
    setTest((test + 1) % 4);
    console.log("test: ", test, "props.count", props.count);
      // if(props.count == 4 || props.count == 3){
      //   if(test == 3) {
      //     setState({
      //     bg:tilesBackground,
      //     sowidth:15,
      //     soheight: 15})          
      //   } else {
      //       setState({
      //       bg: mainBackground})
      //   }
      // } else
       if(test == 3){
          setState({
          bg: tilesBackground,
          sowidth:15,
          soheight: 15})
      } else {
        setState({
          bg:focusBackground,
          sowidth:15,
          soheight:15
        });
      }
  }

  const onBlur1 = (e) => {
    console.log("onBlur1---------" +  " x:"+ top + " y:"+left)
    setState({
      bw:0,
      bg:tilesBackground,
      sowidth: 0,
      soheight: 0
      });
 
    setTest(0);
  }


  const onLayout1 = (e) => {
    console.log("***************************** e.layout:"+ e.nativeEvent.layout + " x:"+ top + " y:"+left)
    top = e.nativeEvent.layout.y;
    left = e.nativeEvent.layout.x;
  }

  const onFocus1 = (e) => {
    console.log("********************** focus props.layout: *******************************" + test)
    setState({
        bg: tilesBackground,
        sowidth:15,
        soheight: 15
    })
    
  }

  const messageReceived = (message)  =>{
      console.log("Message received:", message);
     setState({
       socketMessage: message
      });
  }

  const display = () => {
      console.log("Display call:", props.count);
      if(props.count == 0){
        return  <TextApp flag = {test} />
      } else if(props.count == 1){
        return <TextApp2 flag = {test} />
      } else if(props.count == 2){
        return   <BorderProps flag = {test} />
      } else if(props.count == 3) {
        return   <ShadowProps flag = {test} />
      } else if(props.count == 4){
        return <ImageProps flag = {test} />
      } else if(props.count == 5){
        return <WebSocketHelper flag = {test} />
      } else if(props.count == 6){
        return  <Animation />
      }
  }

    console.log("MyComponent color:" + state.bg)
    return (
    <Pressable isTVSelectable='true' ref={myRef} onBlur={onBlur1} onFocus={onFocus1} changeCursor={changeCursor} onLayout={ onLayout1} style={{zIndex: 100, borderColor: state.color1, borderWidth: state.bw, marginTop:50, position: pos, left: props.layout.x, top:props.layout.y, backgroundColor:state.bg, height:props.layout.h? props.layout.h:height, width:props.layout.w? props.layout.w:width, borderRadius:10, shadowOffset: {
                width: state.sowidth,
                height: state.soheight
              },
              shadowRadius:10,
              shadowColor:shadowColor,
              shadowOpacity: 1}}
   onPress={onPress1}>
      {display()}
    </Pressable>
    );
}

const SimpleViewApp = () => {

  let list1 = [
                   //{x:700,y:0,w:500,h:50},
                   {x:10,y:100},{x:550,y:100},{x:1090, y:100},
                   {x:10,y:500},{x:550,y:500},{x:1090, y:500},
                   //{x:1620, y:100, w:150, h:700}
              ];

  // let list1 = [
  //                  //{x:350,y:0,w:500,h:50},
  //                  {x:10,y:100},{x: 400,y:100},{x:800, y:100},
  //                  {x:10,y:368},{x:400,y:368},{x:800, y:368},
  //                  //{x:1190, y:100, w:80, h:470}
  //             ];

  const addItems = () => {
    var arr = [];
    for (var i = 0; i<list1.length; i++){
      console.log("======================random left:" +  list1[i].x + "  top:" + list1[i].y );
      arr.push(<MyComponent layout={list1[i]} count={i}></MyComponent>)
    }
    return arr;
  }

  const addItem = () =>{
    return (
      <ImageBackground style = {styles.image} source={require('./integratedApps/images/bg2_2.png')} resizeMode='cover'>
        {addItems()}
        <View style={[styles.header1, {backgroundColor:mainBackground, justifyContent:'center', alignItems:'center'}]}>
          <Text style={{fontSize:fontSize1, fontWeight:'bold', letterSpacing:3}}> Demo of First MileStone </Text>
        </View>
        <View style={[styles.header2, {backgroundColor:mainBackground}]}>
          <Animation />
        </View>
      </ImageBackground>
    );
  }
  return (
    addItem()
  );
}

const styles = StyleSheet.create({
  // header1 : {
  //   left:380,
  //   top:50,
  //   width:400,
  //   height:50
  // },
  // header2 : {
  //   left:1170,
  //   top:105,
  //   width:90,
  //   height:470
  // },
    header1 : {
    left:550,
    top:50,
    width:500,
    height:50
  },
  header2 : {
    left:1620,
    top:105,
    width:150,
    height:690
  },
  image: {
    flex: 1,
    justifyContent: "flex-start",
    alignItems:'flex-start',
    width: resolution.appSize.width,
    height:resolution.appSize.height
  }
});

AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);

export default SimpleViewApp
