import React, { useEffect} from "react";
import { Animated, View } from "react-native";
import Config from './config.json'

let resolution = Config.size.low;

const Animation = (props) => {

	let value = new Animated.ValueXY({x:10,y:10});
	let fading = new Animated.Value(resolution.maincontainer.width * 0.1);
	let backgroundColor = Config.animation.backgroundColor;

	useEffect( () =>   {
		console.log("---moveRect:" + JSON.stringify(value))
		Animated.loop(Animated.sequence([
			Animated.timing(value,{
				toValue: {x:10, y:400},
				duration: 4000,
				useNativeDriver:false
			}),
			Animated.timing(fading,{
				toValue: 120,
				duration: 4000,
				useNativeDriver: false
			}),
			Animated.timing(fading,{
				toValue: 50,
				duration: 4000,
				useNativeDriver: false
			}),
			Animated.timing(fading,{
				toValue: 50,
				duration: 1000,
				useNativeDriver: false
			}),
			Animated.timing(value,{
				toValue: {x:10, y:10},
				duration: 4000,
				useNativeDriver:false
			}),
			Animated.timing(fading,{
				toValue: 70,
				duration: 4000,
				useNativeDriver: false
			})			
		]),{}).start();
	})
 
	return(
		<View>
			<Animated.View 
				style={[
					value.getLayout(), 
					{
						backgroundColor: backgroundColor,width:fading, 
						height:fading, borderWidth:20, borderRadius:25,
						borderColor:backgroundColor,
						shadowOffset: {
							width: 5,
							height: 15
							},
						shadowRadius:10,
						shadowColor: Config.animation.shadowColor,
						shadowOpacity: 1
					}
				]}
			>
			</Animated.View>
		</View>
	);
}

export default Animation;
