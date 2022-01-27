import React, { useState, useRef} from "react";
import { Pressable, Text, View, ImageBackground, Image, ScrollView } from "react-native";
import { Dimensions , StyleSheet} from "react-native";
//import content from './dataContent.json'

const windowSize = Dimensions.get('window');
let titleTextFontSize = windowSize.height/25;
let titleTextContentSize = windowSize.height/45;
let titleTextGenreSize = windowSize.height/35;
let content = [];

const thumbnailImagePath = [
      require('./images/thumbnail_300x160/1.jpg'),
      require('./images/thumbnail_300x160/2.jpg'),
      require('./images/thumbnail_300x160/3.jpg'),
      require('./images/thumbnail_300x160/4.jpg'),
      require('./images/thumbnail_300x160/5.jpg'),
      require('./images/thumbnail_300x160/6.jpg'),
      require('./images/thumbnail_300x160/7.jpg'),
      require('./images/thumbnail_300x160/8.jpg'),
      require('./images/thumbnail_300x160/22.jpg'),
      require('./images/thumbnail_300x160/20.jpg'),
      require('./images/thumbnail_300x160/23.jpg'),
      require('./images/thumbnail_300x160/21.jpg'),
      require('./images/thumbnail_300x160/18.jpg'),
      require('./images/thumbnail_300x160/17.jpg'),
      require('./images/thumbnail_300x160/19.jpg'),
      require('./images/thumbnail_300x160/24.jpg'),
      require('./images/thumbnail_300x160/13.jpg'),
      require('./images/thumbnail_300x160/15.jpg'),
      require('./images/thumbnail_300x160/12.jpg'),
      require('./images/thumbnail_300x160/16.jpg'),
      require('./images/thumbnail_300x160/14.jpg'),
      require('./images/thumbnail_300x160/11.jpg'),
      require('./images/thumbnail_300x160/10.jpg'),
      require('./images/thumbnail_300x160/9.jpg'),
];	
const posterImagePath = [
      require('./images/poster_716x300/1.jpg'),
      require('./images/poster_716x300/2.jpg'),
      require('./images/poster_716x300/3.jpg'),
      require('./images/poster_716x300/4.jpg'),
      require('./images/poster_716x300/5.jpg'),
      require('./images/poster_716x300/6.jpg'),
      require('./images/poster_716x300/7.jpg'),
      require('./images/poster_716x300/8.jpg'),
      require('./images/poster_716x300/22.jpg'),
      require('./images/poster_716x300/20.jpg'),
      require('./images/poster_716x300/23.jpg'),
      require('./images/poster_716x300/21.jpg'),
      require('./images/poster_716x300/18.jpg'),
      require('./images/poster_716x300/17.jpg'),
      require('./images/poster_716x300/19.jpg'),
      require('./images/poster_716x300/24.jpg'),
      require('./images/poster_716x300/13.jpg'),
      require('./images/poster_716x300/15.jpg'),
      require('./images/poster_716x300/12.jpg'),
      require('./images/poster_716x300/16.jpg'),
      require('./images/poster_716x300/14.jpg'),
      require('./images/poster_716x300/11.jpg'),
      require('./images/poster_716x300/10.jpg'),
      require('./images/poster_716x300/9.jpg'),
];

const FocusableComponent = (props) =>  {
   let myRef = useRef(null);	
   let [state, setState] = useState({soheight: 0,bwidth:0 });

   const onBlur = (e) => {
       console.log("onBlur---------" )
       setState({ soheight: 0,bwidth:0 });
   }

   const onFocus = (e) => {
       console.log("onFocus---------" )
       setState({ soheight: 15, bwidth:5 });
       props.change((props.count));
   }


   return (
       <Pressable isTVSelectable='true' ref={myRef} onBlur={onBlur} onFocus={onFocus} style={[styles.elementView, {shadowOffset: {width: 0,height: state.soheight}}]} >
           <Image style={{ borderColor:'lightblue',  borderWidth:state.bwidth, resizeMode:'stretch'}} source={thumbnailImagePath[props.count]}> 
           </Image>
       </Pressable>
    );
}

const SampleVODPage = (props) => {

    let [index, setIndex] = useState(0);
    let content = props.contentData;

    function changeBackground (value) {
	 setIndex(value);
    }

    const addItems = (n) => {
	var arr = [];
	for (var i=n; i<n+8; i++){
	    arr.push(<FocusableComponent count={i} change={changeBackground}></FocusableComponent>);
	}
	return arr;
    }

    const horizontalScrollView = (x) => {
       return (
	   <ScrollView style={styles.horizontalScrollView} horizontal={true} >
	      {addItems(x)}
	   </ScrollView>
       );	       
    }
   
    const horizontalScrollViewHeader = (text) => {
       return (
	   <View style={{margin:10}}>
               <Text style={styles.elementText}>
	          {text}
	       </Text>
           </View> 
       );	       
    }

    const verticalScrollView = () => {
	return (    
	    <ScrollView style={styles.verticalScrollView}>
	      {horizontalScrollViewHeader('Continue Watching')}	
              {horizontalScrollView(0)}	      
	      {horizontalScrollViewHeader('Favorites')}	
              {horizontalScrollView(8)}	      
	      {horizontalScrollViewHeader('Recommendations')}	
              {horizontalScrollView(16)}	      
	    </ScrollView>
         );	       
    }

    const posterView = () => {
	return(    
	     <Image source={posterImagePath[index]} style={styles.posterView} resizeMode='stretch'>
	     </Image>  
         );	       
    }

    const posterContent = () => {
        let text1 = content[index].duration.concat("  |  ", content[index].genre, "  |  ", content[index].year)
	return (    
           <View style={styles.posterContentView}>
                <Text style={styles.titleText}>{content[index].title}</Text>
                <Text style={[styles.titleTextContent,{fontSize:titleTextGenreSize}]}>{text1}</Text>
                <Text style={[styles.titleTextContent,{fontSize:titleTextContentSize}]}>{content[index].description}</Text>
           </View>
         );	       
    }

    return (
	<ImageBackground style={styles.backgroundimage} source={require('./images/bg.jpg')} resizeMode='cover'>
	    {posterView()}  
	    {posterContent()}  
	    {verticalScrollView()}  
	</ImageBackground>
    );
}

const styles = StyleSheet.create({
    backgroundimage: {
       flex: 1,
       width: windowSize.width,
       height: windowSize.height,
    },
    posterView: {
       margin: 10,
       position : 'absolute', 	    
       top : 20,
       left : windowSize.width/2 ,  	    
       width : '40%',
       height : '40%',
       shadowColor: 'black',
       shadowRadius: 10,
       shadowOpacity: 1,
       shadowOffset: {width:5 , height:30},	    
    },
    posterContentView: {
       margin: 10,
       position : 'absolute', 	    
       top : 30,
       left : 30,  	    
       width : '40%',
       height : '40%',
    },
    titleText: {
       color : 'white',
       fontWeight : 'bold',
       textShadowColor : 'black',
       textShadowRadius : 1,
       fontSize : titleTextFontSize 	    
    },
    titleTextContent: {
       marginTop : 25,	    
       color : 'white',
    },
    verticalScrollView: {
       margin : 10,
       position : 'absolute',
       top : windowSize.height/2,
       left : 30,
       width : '90%',
       height : '40%',	    
       shadowColor: 'black',
       shadowRadius: 10,
       shadowOpacity: 1,
       shadowOffset: {width:5 , height:30},	    
    },
    horizontalScrollView: {
       margin : 5,
       shadowColor: 'black',
       shadowRadius: 10,
       shadowOpacity: 1,
       shadowOffset: {width:5 , height:30},	    
    },
    elementView: {	
       margin : 5,
       width : 300, // Thumbnail image size 
       height : 160,
       shadowColor : 'black',
       shadowRadius : 10,	    
       shadowOpacity : 1,
       zIndex : 100
    },
    elementText: {
       color : 'white',
       fontWeight : 'bold',
       fontSize : titleTextGenreSize 	    
    }
    	
});

export default SampleVODPage

