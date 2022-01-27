import React, { useState,useRef} from "react";
import { TextInput,View,ImageBackground,TouchableOpacity,Text,Animated,Image} from "react-native";
import { Dimensions , StyleSheet} from "react-native";

import SampleVODPage from './SampleVODPage';

const windowSize = Dimensions.get('window');

const SampleLoginPage = () => {

    let [loginStatus, setLoginStatus] = useState(false);
    let [ti1State, setTi1State] = useState({borderColor:'black',shadowOpacity:0});
    let [ti2State, setTi2State] = useState({borderColor:'black',shadowOpacity:0});
    let [username,onChangeUserName] = useState(null);
    let [password,onChangePassword] = useState(null);
    let [requestStatus,setRequestStatus] = useState({text:"Load",viewColor:"darkblue"});
    let [content,setContent] = useState([]);	
    let [spinValue] = useState(new Animated.Value(0));

    const onFocus = () => {
       setTi1State({borderColor:'lightblue',shadowOpacity:1});
    }
    const onBlur = () => {
       setTi1State({borderColor:'black',shadowOpacity:0});
    }
    const onFocus1 = () => {
       setTi2State({borderColor:'lightblue',shadowOpacity:1});
    }
    const onBlur1 = () => {
       setTi2State({borderColor:'black',shadowOpacity:0});
    }

    const startFetchVODData = () => {
         var request = new XMLHttpRequest();
         request.open('GET','http://192.168.0.106:9081/dataContent.json');
	 request.responseType = "json"   
	 request.onreadystatechange = (e) => {
            if(request.status == 200) {
	      setContent(request.response);
              setRequestStatus({text:"WE ARE READY TO GO !! ",viewColor:"green"})
	      setTimeout(()=> {
		 setLoginStatus(true);
	      },5000);	    
	    } else {
	      console.error("request error:",request.status);
              setRequestStatus({text:"Error",viewColor:"red"})
	    }
	 }
	 request.send();   
    }

    const onPress = () => {
       setRequestStatus({text:"Loading",viewColor:"royalblue"})
       startRotation();	    
       setTimeout(()=> {
          startFetchVODData()	    
       },4000);	    
    }

    const userNameBlock = () => {
	return (
	   <View style={[styles.textinputView,{borderColor:ti1State.borderColor,shadowOpacity:ti1State.shadowOpacity}]}>	
	     <TextInput style={styles.textinput} 
		      placeholder="Username" placeholderTextColor="darkgrey" onFocus={onFocus} onBlur={onBlur}/>
	   </View>	
	);	
    }

    const passwordBlock = () => {
	return (
	   <View style={[styles.textinputView,{borderColor:ti2State.borderColor,shadowOpacity:ti2State.shadowOpacity}]}>	
	     <TextInput style={styles.textinput} 
		      placeholder="Password" placeholderTextColor="darkgrey" secureTextEntry={true} 
		      onFocus={onFocus1} onBlur={onBlur1}/>	
	   </View>	
	);	
    }

    startRotation=()=>{
      console.log("start rotation animation")
      Animated.loop(
         Animated.timing(
         spinValue,
         {
            toValue: 1,
            duration: 6000,
         })
         ,{iterations:1}).start();
    }
    
    const spin = spinValue.interpolate({
       inputRange: [0, 1],
       outputRange: ['0deg', '2000deg']
    })
   
    const animatedStyle = {
       transform : [
        {
          rotate: spin
        }
      ] 
    }
    
    const loadingButton = () => {
	if(requestStatus.text == "Loading") {   
           return (
              <Animated.View style={[animatedStyle]}>
                 <Image source={require('./images/loadingIcon.png')} style={{width:100,height:100}} resizeMode='cover'/>
              </Animated.View>
	   );
	} else {    
           return (
	      <TouchableOpacity style={[styles.submitView,{backgroundColor:requestStatus.viewColor}]} onPress={onPress} activeOpacity={0.85}>
                 <Text style={{margin:5,color:'black' , fontSize:windowSize.height/45 , fontWeight:'bold',textAlign:'center'}}>{requestStatus.text}</Text> 
              </TouchableOpacity>
	   );
	}
    }

    const loginPage = () => {
      return (
	<ImageBackground style={styles.backgroundimage} source={require('./images/bg.jpg')} resizeMode='cover'>
	  <Image style={{ margin:50,width: 200, height: 200 }} source={require('react-native/Libraries/NewAppScreen/components/logo.png')}/>
          {userNameBlock()}	      
          {passwordBlock()}
	  {loadingButton()}    
	</ImageBackground>
      )
    };

    if(loginStatus) {
      return <SampleVODPage contentData={content}></SampleVODPage>;	    
    } else {
      return (	    
	 loginPage()
      );
    }
}

const styles = StyleSheet.create({
    backgroundimage: {
       flex: 1,
       alignItems : 'center',
       justifyContent : 'center',	    
       width: windowSize.width,
       height: windowSize.height,
    },
    textinputView: {
       margin : 20,	    
       width : '40%',
       height : windowSize.height/20,	    
       borderWidth : 5,
       backgroundColor:'dimgrey',
       shadowColor : 'black',
       shadowRadius : 10,
       shadowOffset : {width:5, height:30},	    
    },
    textinput: {
       color :'white',
       fontSize:windowSize.height/35,
    },
    submitView: {
       alignItems : 'center',
       padding : 20,	    
       borderWidth : 5,
       color :'white',
    }
});

export default SampleLoginPage

