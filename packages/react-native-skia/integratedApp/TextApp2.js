import React from 'react'
import {View, Text, StyleSheet} from 'react-native'
import Config from './config.json'

const resolution = Config.size.low;
const fontSize1 = resolution.textStyle.fontSize;

const TextApp2 = (props) =>{

	let backgroundColor = 'white';
	let borderColor = 'white';
	let subViewWidth = resolution.maincontainer.width;
	let subViewHeight = resolution.maincontainer.height;
	let leftAlignView = 15;
	let topAlignView = 17;
	let sWidth = 10;
	let sHeight = 10;
	let sRadius = 10;
	let sOPacity = 1;
	let text1 = <Text />;
	let text2 = <Text />;
	let text3 = <Text />;
	let text4 = <Text />;
	let text5 = <Text />;
	let text6 = <Text />;

    if(props.flag == 0){
		backgroundColor = Config.main.tilesBackground;
		borderColor = Config.main.tilesBackground;
		text3 = <Text style={{fontSize:fontSize1, fontWeight:'bold', color:'white', textShadowRadius:0, textShadowColor:'#3F454C', textShadowOffset:{width:3,height:3} }} > Text Shadow Properties </Text>
		sWidth = 0;
		sHeight = 0;
		sRadius = 0;
		sOPacity = 0;
    } else if(props.flag == 1) {
		var subFont = fontSize1 - 11;
		text1 = <Text style={{fontSize:subFont, margin:5}}>Default textShadowOffset:</Text>
		text2 = <Text style={{ textShadowRadius:1, textShadowColor:'yellow' , fontSize:subFont , margin:5}}>textShadowRadius:5, textShadowColor:'yellow'</Text>
		text3 = <Text style={{ textShadowRadius:5, textShadowColor:'red', fontSize:subFont, margin:5 }}>textShadowRadius:10, textShadowColor:'red'</Text>
		text4 = <Text style={{ textShadowRadius:15, textShadowColor:'blue', fontSize:subFont, margin:5 }}>textShadowRadius:15, textShadowColor:'blue'</Text>
		text5 = <Text style={{ textShadowRadius:20, textShadowColor:'red', fontSize:subFont, margin:5 }}>textShadowRadius:20, textShadowColor:'red'</Text>
		text6 = <Text style={{ textShadowRadius:25, textShadowColor:'yellow', fontSize:subFont, margin:5 }}>textShadowRadius:25, textShadowColor:'yellow'</Text>

      } else if(props.flag == 2){
			subFont = fontSize1 - 8;
			text1 =  <Text style={{fontSize:subFont, margin:5}}>Default textShadowOffset:</Text>
			text2 = <Text style={{ textShadowOffset:{width:2,height:2}, textShadowColor:'blue', fontSize:subFont, margin:5 }}>textShadowOffset: width:2,height:2 </Text>
			text3 = <Text style={{ textShadowOffset:{width:-10,height:-5}, textShadowColor:'red', fontSize:subFont, margin:5 }}>textShadowOffset: width:-5,height:-5</Text>
			text4 =   <Text style={{ textShadowOffset:{width:9.5,height:-7.6}, textShadowColor:'blue', fontSize:subFont, margin:5 }}>
			textShadowOffset: width:3.5,height:-7.6
			</Text>
			text5 =   <Text style={{ textShadowOffset:{width:16.6,height:-5.5}, textShadowColor:'red', fontSize:subFont, margin:5 }}>
			textShadowOffset: width:6.6,height:-5.5
			</Text>
      } else if (props.flag == 3){
			subFont = fontSize1 - 2;
			text1 =  <Text style={{fontSize:subFont, fontWeight:'bold'}}>Default textDecorationLine:</Text>
			text2 = <Text style={{ textDecorationLine:'underline', textDecorationColor:'blue', fontSize:subFont }}>'underline'</Text>
			text3 = <Text style={{ textDecorationLine:'line-through', textDecorationColor:'red', fontSize:subFont }}>'line-through'</Text>
			text4 = <Text style={{ textDecorationLine:'underline line-through', textDecorationColor:'red', fontSize:subFont }}>'underline line-through'</Text>
			text5 = <Text style={{ textDecorationLine:'none', textDecorationColor:'red', fontSize:subFont}}>'none'</Text>
      }

      console.log("Config.textApp2.view.width: ", subViewWidth)
      return (
          <View style={[styles.MainContainer, {
            backgroundColor: backgroundColor, 
            width:subViewWidth - 30,
            height:subViewHeight - 30,
            borderWidth:20, 
            borderRadius:10,
            borderColor:borderColor, 
            left:leftAlignView,
            top:topAlignView,
            justifyContent:'center',
            alignItems:'center',
            }]}
          >
            {text1}
            {text2}
            {text3}
            {text4}
            {text5}
            {text6}
          </View>
      ); 
}

const styles = StyleSheet.create({
	MainContainer: {
		width:  Config.textApp2.view.width,
		height:  Config.textApp2.view.height
	}
});

export default TextApp2
