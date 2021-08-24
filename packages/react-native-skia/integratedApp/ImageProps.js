import React from 'react'
import {View, Text, Image} from 'react-native'
import Config from './config.json'

let resolution = Config.size.low;

const ImageProps = (props) => {

    let index = 1;
    let bwidth = 0;
    let opacityValue = 1;
    let bColor = '';
    let bgColor = '';
    let width = resolution.maincontainer.width * 0.7;
    let height = resolution.maincontainer.width * 0.6;
    let resizeCase = 2;
  
    const imagePath = [
		require('./images/snackexpo.png'),
		require('./images/logo.png'),
		require('./images/tImage13.png') 
    ]
  
    let resizemodes = [
		"center",
		"cover",
		"contain",
		"stretch",
		"repeat",
    ];

	console.log("Flag: ", props.flag);
	if(props.flag == 0){
		index = 2;
		bgColor = Config.main.tilesBackground;
		width = resolution.maincontainer.width * 0.76;
	} else if(props.flag == 1){
		index = 1;
		bgColor = Config.main.focusBackground;
		bwidth = 0;
		resizeCase = 1;
	} else if( props.flag ==2){
		index = 1;
		resizeCase = 3;
		opacityValue = 0.5;
		bgColor = Config.main.focusBackground;
	} else if(props.flag == 3) {
		index = 0;
		resizeCase = 4;
		bwidth = 10;
		bColor = '#F8F9FA';
	} 

  return(
	<View 
		style={{
			justifyContent:'center', alignItems:'center', 
			width:resolution.maincontainer.width, 
			height:resolution.maincontainer.height}}
		> 
		<Image 
			style={{width:width,height:height,
			borderColor:bColor,  borderWidth:bwidth, 
			opacity:opacityValue, backgroundColor:bgColor,alt:'Hello',
			resizeMode:resizemodes[resizeCase]}}
			source={imagePath[index]}
		>
		</Image>
	</View>
  );
}

export default ImageProps
