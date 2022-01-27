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
	      },2000);	    
	    } else {
	      console.error("request error:",request.status);
              setRequestStatus({text:"Loading Data Failed",viewColor:"red"})
	    }
	 }
	 request.send();   
    }

    const onPress = () => {
       setRequestStatus({text:"Loading",viewColor:"royalblue"})
       handleAnimation()
       setTimeout(()=> {
          startFetchVODData()	    
       },3000);	    
    }

    const userNameBlock = () => {
	return (
	   <TextInput style={[styles.textinputView,{borderColor:ti1State.borderColor,shadowOpacity:ti1State.shadowOpacity}]} 
		      placeholder="Username" placeholderTextColor="darkgrey" onFocus={onFocus} onBlur={onBlur}
		      value={username} onChangeText={onChangeUserName}/>	
	);	
    }
    const passwordBlock = () => {
	return (
	   <TextInput style={[styles.textinputView,{borderColor:ti2State.borderColor,shadowOpacity:ti2State.shadowOpacity}]} 
		      placeholder="Password" placeholderTextColor="darkgrey" secureTextEntry={true} 
		      onFocus={onFocus1} onBlur={onBlur1}
		      value={password} onChangeText={onChangePassword}/>	
	);	
    }

    const [rotateAnimation, setRotateAnimation] = useState(new Animated.Value(0));

    const handleAnimation = () => {
       console.log("animation start");	    
       Animated.timing(rotateAnimation, {
        toValue: 1,
        duration: 800,
       }).start(() => {
          rotateAnimation.setValue(0);
      });
    };

    const interpolateRotating = rotateAnimation.interpolate({
      inputRange: [0, 1],
      outputRange: ['0deg', '720deg'],
    });

    const loginPage = () => {
      return (
	<ImageBackground style={styles.backgroundimage} source={require('./images/bg.jpg')} resizeMode='cover'>
	  <Image style={{ margin:50,width: 200, height: 200 }} source={require('react-native/Libraries/NewAppScreen/components/logo.png')}/>
          {userNameBlock()}	      
          {passwordBlock()}
	  <TouchableOpacity style={[styles.submitView,{backgroundColor:requestStatus.viewColor}]} onPress={onPress}>
	     <Animated.View style={{transform:[{rotate:interpolateRotating}]}}> 
	         <Text style={{margin:5,color:'black' , fontSize:windowSize.height/45 , fontWeight:'bold',textAlign:'center'}}>{requestStatus.text}</Text> 
	     </Animated.View> 
	  </TouchableOpacity>
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
       color :'white',
       fontSize:windowSize.height/35,
       backgroundColor:'dimgrey',
       shadowColor : 'black',
       shadowRadius : 10,
       shadowOffset : {width:5, height:30},	    
    },
    submitView: {
       alignItems : 'center',
       margin : 20,
       padding : 20,	    
       width : '15%',
       borderWidth : 5,
       color :'white',
    }
});

export default SampleLoginPage

