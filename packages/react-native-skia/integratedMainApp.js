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
	let top = 0;
	let left = 0;
	let width = resolution.maincontainer.width;
	let height = resolution.maincontainer.height;
	let pos = 'absolute';
	let subackground = testBackground;
	let [state, setState] = useState({
		bw:0,
		color1: 'red',
		bg: tilesBackground,
		sowidth: 0,
		soheight: 0
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
		console.log("*** e.layout:"+ e.nativeEvent.layout + " x:"+ top + " y:"+left)
		top = e.nativeEvent.layout.y;
		left = e.nativeEvent.layout.x;
	}

	const onFocus1 = (e) => {
		console.log("**** focus props.layout: ********" + test)
		setState({
			bg: tilesBackground,
			sowidth:15,
			soheight: 15
		})

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

	return (
		<Pressable isTVSelectable='true' ref={myRef} 
			onBlur={onBlur1} 
			onFocus={onFocus1} 
			changeCursor={changeCursor} 
			onLayout={ onLayout1} 
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
				<ImageBackground style = {styles.image} source={require('./integratedApp/images/bg2_2.png')} resizeMode='cover'>
					{addItems()}
					<View style={[styles.header1,styles.header, {backgroundColor:'#8D959D', borderWidth:5, borderRadius:10, borderColor:'#8D959D', justifyContent:'center', alignItems:'center'}]}>
						<Text style={{fontSize:fontSize1, fontWeight:'bold', letterSpacing:3}}> Demo of First MileStone </Text>
					</View>
					<View style={[styles.header2, styles.header, {backgroundColor:'#8D959D', borderWidth:5, borderRadius:10, borderColor:'#8D959D'}]}>
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
	header: {
		shadowOffset: {
			width: 3,
			height: 3
		},
		shadowRadius:2,
		shadowColor:shadowColor,
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
