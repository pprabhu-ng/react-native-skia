/**
 * This is an Integrated app where we show all the properties supported by the RNS currently.
 * This will be updated further based on the feture list supported by RNS
*/

import React, { useState, useRef} from "react";
import { Pressable, AppRegistry, Text, View, StyleSheet, ImageBackground} from "react-native";
import TextApp from './integratedApp/TextApp'
import Animation from './integratedApp/Animation'
import ShadowProps from './integratedApp/ShadowProps'
import BorderProps from './integratedApp/BorderProps'
import TextApp2 from './integratedApp/TextApp2'
import ImageProps from './integratedApp/ImageProps'
import WebSocketHelper from './integratedApp/webSocketHelper'
import Config from './integratedApp/config.json'

let testBackground =  Config.main.mainBackground
let mainBackground = testBackground;
let tilesBackground = Config.main.tilesBackground;
let focusBackground = Config.main.focusBackground;
let shadowColor = Config.main.shadowColor;
let resolution = Config.size.low;
let fontSize1 = resolution.textStyle.fontSize;

const MyComponent = (props) =>  {

	let myRef = useRef(null);
	let width = resolution.maincontainer.width;
	let height = resolution.maincontainer.height;
	let pos = 'absolute';
	let [state, setState] = useState({
		bw:0,
		color1: 'red',
		bg: tilesBackground,
		sowidth: 0,
		soheight: 0, 
    bgChange:0
	});

	let [test, setTest] = useState(0)

	const changeCursor = (x,y, w, h, color) => {
		console.log("*****XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX****")
		setState({bw:5})
	}

	const onPress1=(e) => {
	console.log(" inOnpress-----------"+ e )

	setTest((test + 1) % 4);
	console.log("test: ", test, "props.count", props.count);
		if(test == 3 && props.count !=6){
			setState({
			bg: Config.main.subtileFocus,
			sowidth:15,
			soheight: 15,
      bgChange:1})
		} else {
		setState({
			bg:focusBackground,
			sowidth:15,
			soheight:15,
      bgChange:1
		});
		}
	}

	const onBlur1 = (e) => {
	console.log("onBlur1---------", e)
		setState({
			bw:0,
			bg:tilesBackground,
			sowidth: 0,
			soheight: 0,
      bgChange:0
		});

		setTest(0);
	}


	const onFocus1 = (e) => {
		console.log("**** focus props.layout: ********", e)
    if(props.count != 6) {
      setState({
        bg: Config.main.subtileFocus,
        sowidth:15,
        soheight: 15,
        bgChange:1
      })
    }
	}

	const display = () => {
		console.log("Display call:", props.count);
		if(props.count == 0){
			return  <TextApp flag = {test}  bg = {state.bgChange} />
		} else if(props.count == 1){
			return <TextApp2 flag = {test}  bg = {state.bgChange} />
		} else if(props.count == 2){
			return   <BorderProps flag = {test}  bg = {state.bgChange} />
		} else if(props.count == 3) {
			return   <ShadowProps flag = {test} bg = {state.bgChange}/>
		} else if(props.count == 4){
			return <ImageProps flag = {test}  bg = {state.bgChange} />
		} else if(props.count == 5){
			return <WebSocketHelper flag = {test}  bg = {state.bgChange} />
		} else if(props.count == 6){
			return  (< View style={{alignContent:'center'}}>
        <View style={{left:30}}> 
          <Text style={{color:'white', fontWeight:'bold'}}> JS Animation</Text>
        </View>
        <Animation flag = {test}  bg = {state.bgChange} />
      </View>
      );
		}
	}

	return (
		<Pressable isTVSelectable='true' ref={myRef} 
			onBlur={onBlur1} 
			onFocus={onFocus1} 
			changeCursor={changeCursor} 
			style={{zIndex: 100, borderColor: state.color1, 
				borderWidth: state.bw, 
				marginTop:50, position: pos, 
				left: props.layout.x, top:props.layout.y, 
				backgroundColor:state.bg, 
				height:props.layout.h? props.layout.h:height, 
				width:props.layout.w? props.layout.w:width, 
				borderRadius:10, 
				shadowOffset: {
					width: state.sowidth,
					height: state.soheight
				},
				shadowRadius:10,
				shadowColor:shadowColor,
				shadowOpacity: 1
			}}
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
						{x:1620, y:100, w:150, h:700}
					];

		// Below list is for 720p, will be configured in future
		// let list1 = [
		//                  //{x:350,y:0,w:500,h:50},
		//                  {x:10,y:100},{x: 400,y:100},{x:800, y:100},
		//                  {x:10,y:368},{x:400,y:368},{x:800, y:368},
		//                  {x:1190, y:100, w:80, h:470}
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
				<ImageBackground style = {styles.image} source={require('./integratedApp/images/bg.jpg')} resizeMode='cover'>
					{addItems()}
					<View style={[styles.header1,styles.header, {backgroundColor:tilesBackground, borderWidth:5, borderRadius:10, borderColor:tilesBackground, justifyContent:'center', alignItems:'center'}]}>
						<Text style={{fontSize:fontSize1, fontWeight:'bold', letterSpacing:3, color:"white"}}> Demo of First MileStone </Text>
					</View>
				</ImageBackground>
			);
		}
		return (
			addItem()
		);
	}

const styles = StyleSheet.create({
	//Below Commented code is for 720p, will be configured in future.
	// header1 : {
	//   left:380,
	//   top:50,
	//   width:400,
	//   height:50
	// },
	// header2 : {
	//   left:1170,
	//   top:100,
	//   width:90,
	//   height:495
	// },
	header1 : {
		left:570,
		top:50,
		width:500,
		height:50
	},
	header2 : {
		left:1620,
		top:100,
		width:150,
		height:700
	},
	header: {
		shadowOffset: {
			width: 1,
			height: 1
		},
		shadowRadius:0,
		shadowColor:'#FFBA08',
		shadowOpacity: 1
	},
	image: {
		flex: 1,
		justifyContent: "flex-start",
		alignItems:'flex-start',
		width: resolution.appSize.width,
		height:resolution.appSize.height,
	}
});

AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);

export default SimpleViewApp
