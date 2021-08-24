import React, {useEffect} from 'react'
import {View, AppRegistry, Text, StyleSheet, Animated} from 'react-native'
import Config from './config.json'

let resolution = Config.size.low;
let fontSize1 = resolution.textStyle.fontSize;

const ShadowProps = (props) => {

    let test = 0;
    let text = <Text />
    var subViewWidth= resolution.maincontainer.width * .4;
    var subViewHeight=resolution.maincontainer.height * .67;
    var bgColor='';
    var bColor = '';

    var mVbgColor = Config.main.mainBackground;
    var sOffsetWidth= 0;
    var sOffsetheight=0;
    var sRadius = 0;
    let sOpacity = 0;
    let sColor = Config.shadowProps.view.shadowColor;

    let borderWidth = 0;
    let borderRadius = 0;


    let justifyContent = 'center';
    let alignItems = 'center';

    let value=  new Animated.ValueXY({x:0,y:0});

    useEffect( () =>   {
        console.log("---moveRect:" + JSON.stringify(value))
        if(props.flag == 2){
        console.log("---moveRect:" + JSON.stringify(value))
        Animated.timing(value,{
            toValue: {x: resolution.maincontainer.width * .28, y: resolution.maincontainer.height * 0.167},
            duration: 2000,
            velocity: 1,
            tension: 40,
            useNativeDriver:false
        }).start();
        } else if (props.flag == 3){
        Animated.timing(value,{
            toValue: {x:resolution.maincontainer.width * 0.2, y:  resolution.maincontainer.height * -0.167},
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


    console.log("props.flag", props.flag);
    if(props.flag == 0){
        sRadius = 0;
        sOpacity = 0;
        borderWidth = 0;
        borderRadius = 0;
        subViewWidth =  resolution.maincontainer.width * 0.6;
        bgColor = Config.main.tilesBackground;
        bColor = Config.main.tilesBackground;
        mVbgColor = Config.main.tilesBackground;
        text = <Text style={{textAlign:'center', 
        fontSize:fontSize1,
         color:'white', fontWeight:'bold', textShadowRadius:0, textShadowColor:'#3F454C', textShadowOffset:{width:3,height:3}}}> Shadow Properties </Text>
    } else if(props.flag == 1) {
        sRadius = 5;
        sOpacity = 1;
        borderWidth = 5;
        borderRadius = 5;
        sOffsetWidth= subViewWidth/ 5;
        sOffsetheight= subViewHeight/ 5;
        bgColor= Config.shadowProps.view.bgColor;
        bColor = Config.shadowProps.view.bgColor;
        justifyContent = 'flex-start';
        alignItems = 'flex-start';
        text = <Text>  With Shadows</Text>
    } else if(props.flag == 2) {
        sRadius = 20;
        sOpacity = 1;
        borderWidth = 5;
        borderRadius = 5;
        sOffsetWidth= subViewWidth/ 5;
        sOffsetheight= subViewHeight/ 5;
        bgColor= Config.shadowProps.view.bgColor;
        bColor = Config.shadowProps.view.bgColor;
        justifyContent = 'flex-start';
        alignItems = 'flex-start';
        text = <Text> With DIff shadow Radius</Text>
    } else if(props.flag == 3){
        sOffsetWidth= 0;
        sOffsetheight=0;
        sRadius = 40;
        sOpacity = 1;
        borderWidth = 5;
        borderRadius = 5;
        sOffsetWidth= subViewWidth/ 5;
        sOffsetheight= subViewHeight/ 5;
        bgColor= Config.shadowProps.view.bgColor;
        bColor = Config.shadowProps.view.bgColor;
        justifyContent = 'center';
        alignItems = 'center';
        //sColor = '#212529';
        text = <Text> DIff shadow Radius and color</Text>
    }

    return (
        <View style={[styles.MainContainer, {flex: 1, justifyContent:justifyContent, alignItems:alignItems, backgroundColor:mVbgColor, borderWidth:5, borderRadius:10, borderColor:mVbgColor}]}>
        <Animated.View style={value.getLayout()}>
            <View
            style={{
                width:subViewWidth ,
                height : subViewHeight,
                backgroundColor: bgColor,
                shadowOffset: {
                  width: sOffsetWidth,
                  height:sOffsetheight
                },
                shadowRadius:sRadius,
                shadowColor:sColor,
                shadowOpacity: sOpacity,
                borderWidth:borderWidth,
                borderRadius:borderRadius,
                borderColor: bgColor,
                justifyContent: 'center',
                alignItems: 'center'
            }}>
              {text}
            </View>
        </Animated.View>
        </View>
    );
    
}

const styles = StyleSheet.create({
  MainContainer: {
    width:resolution.maincontainer.width,
    height: resolution.maincontainer.height
  }
});

AppRegistry.registerComponent('ShadowProps', () => ShadowProps);
export default ShadowProps