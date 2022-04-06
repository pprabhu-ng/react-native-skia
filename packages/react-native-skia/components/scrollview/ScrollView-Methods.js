'use strict';

const React = require('react');

const {
  ScrollView,
  StyleSheet,
  View,
  Text,
  TouchableOpacity,
  TextInput,
  AppRegistry	
} = require('react-native');

const NUM_ITEMS = 20;


const SimpleViewApp = React.Node = () => {

   const makeItems: (nItems: number, styles: any) => Array<any> = (
       nItems: number,
       styles,
   ): Array<any> => {
       const items = [];
       for (let i = 0; i < nItems; i++) {
           items[i] = (
              <TouchableOpacity key={i} style={styles}>
                 <Text>{'Item ' + i}</Text>
              </TouchableOpacity>
           );
       }
       return items;
   };

   const items = makeItems(NUM_ITEMS, styles.itemWrapper);
   let svRef = React.useRef();
   let isHorizontal = true;
   let offset=0;	

   const [animate,toggleAnimate] = React.useState(false);

   const scrollToEnd = () => {
      setTimeout(() => {	   
         svRef.current.scrollToEnd({animated:animate})
      },5000);
   }

   const scrollTo = () => {
      setTimeout(() => {	   
        if(isHorizontal) {
          svRef.current.scrollTo({x:offset,y:0,animated:animate})
	} else {
          svRef.current.scrollTo({x:0,y:offset,animated:animate})
	}
      },5000);
   }

   const setOffset = (e) => {
      console.log(e.nativeEvent.text);
      offset = parseInt(e.nativeEvent.text)
   }
   
   const setAnimate = () => {
      toggleAnimate(!animate);
   }

   const flashScrollIndicators = () => {
      svRef.current.flashScrollIndicators();
   }

    const scrollView = () => {
      return(
         <>
          <ScrollView ref={svRef} style={styles.scrollView} horizontal={isHorizontal}>
             {items}
          </ScrollView>

          <TouchableOpacity style={{margin:10,backgroundColor:'green',width:500,height:50}} onPress={setAnimate} >
             <Text>{'Tap : Set Animated =' + animate}</Text>
          </TouchableOpacity>
          <TextInput placeholder={'SetOffset'} onSubmitEditing={setOffset} style={{margin:10,backgroundColor:'green',width:500,height:50}}/>
          <TouchableOpacity style={{margin:10,backgroundColor:'green',width:500,height:50}} onPress={scrollTo} >
             <Text>{'Tap : ScrollTo'}</Text>
          </TouchableOpacity>
          <TouchableOpacity style={{margin:10,backgroundColor:'green',width:500,height:50}} onPress={scrollToEnd} >
             <Text>{'Tap : ScrollToEnd'}</Text>
          </TouchableOpacity>
          <TouchableOpacity style={{margin:10,backgroundColor:'green',width:500,height:50}} onPress={flashScrollIndicators} >
             <Text>{'Tap : Flash scroll indicators'}</Text>
          </TouchableOpacity>
         </>
      )
    }

  return (
    scrollView()
  );
}

const styles = StyleSheet.create({
  scrollView: {
    margin: 10,
    height:200,
    width:500,
    backgroundColor:'yellow',
    borderWidth:2,
    borderColor:'black'	  
  },
  itemWrapper: {
    backgroundColor: '#dddddd',
    alignItems: 'center',
    borderRadius: 5,
    borderWidth: 5,
    borderColor: '#a52a2a',
    padding: 30,
    margin:10,
    height:100
  }
});

export default SimpleViewApp;
AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);
