import React, { useState, useRef, useEffect} from "react";
import { Pressable, AppRegistry, Text, View, StyleSheet, ImageBackground, Image, ScrollView} from "react-native";

import Config from './config.json'

let tilesBackground = Config.main.tilesBackground;
let shadowColor = Config.main.shadowColor;
let resolution = Config.resolution;

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

const title = [
   "Frozen II", "Bodied", "Black Panther", "The Lion King", "Joker", "The Addams Family", "Accelaration", "Rambo: Last Blood",
   "Dear John", "Islands Of  Creation", "Ordinary Love", "Four Christmases", "Underworld", "Da Vinci Code", "Happy Family", "Big Trip",
   "Jumanji : Welcome to the Jungle", "Spider-Man : Homecoming", "Dr. Seuss' How the Grinch Stole Christmas", "Ted", "Kung Fu Panda 3", "Cloudy With a Chance of Meatballs", "Closer", "Annie: A Royal Adventure"
]

const genre = [
   "Musical,Comedy", "Comedy", "Action", "Adventure", "Crime,Drama", "Comedy", "Action", "Action",
   "Romance,Drama", " " , "Romance,Drama", "Comedy,Romance,Drama", "Action,Thriller", "Thriller,Drama", "Animation", "Comedy,Adventure",
   "Comedy,Adventure", "Action,Thriller", "Comedy", "Comedy", "Action,Adventure", "Comedy", "Romance,Drama", "Musical"	
]

const duration = [
   "1h  43m", "2h  01m", "2h  02m", "1h  58m","2h  02m", "1h  27m", "1h  25m", "1h  29m",
   "1h  47m", "0h  48m", "1h  31m", "1h  28m","2h  13m", "2h  28m", "1h  32m", "1h  23m",
   "1h  59m", "2h  13m", "1h  44m", "1h  45m","1h  35m", "1h  29m", "1h  43m", "1h  32m"	
]

const year = [
   "2019", "2017", "2018", "2019", "2019", "2019", "2019", "2019",
   "2010", "2015", "2019", "2008", "2003", "2006", "2017", "2019",
   "2017", "2017", "2000", "2012", "2016", "2009", "2004", "1995"
]

const description = [
   "Elsa the Snow Queen has an extraordinary gift -- the power to create ice and snow. But no matter how happy she is to be surrounded by the people of Arendelle, Elsa finds herself strangely unsettled.",

   "A progressive graduate student finds success and sparks outrage when his interest in battle rap as a thesis subject turns into a competitive obsession.",

   "After the death of his father, T'Challa returns home to the African nation of Wakanda to take his rightful place as king. When a powerful enemy suddenly reappears....",

   "Betrayed and exiled from his kingdom, lion cub Simba must figure out how to grow up and take back his royal destiny on the plains of the African savanna.",

   "Isolated, bullied and disregarded by society, failed comedian Arthur Fleck begins a slow descent into madness as he transforms into the criminal mastermind known as the Joker.",

   "Members of the mysterious and spooky Addams family -- Gomez, Morticia, Pugsley, Wednesday, Uncle Fester and Grandma -- encounter a shady TV personality who despises their eerie hilltop mansion.",

   "When an evil crime lord is double-crossed by his most trusted operative, he kidnaps her young son.",

   "Vietnam War veteran John Rambo tries to find some semblance of peace by raising horses on a ranch in Arizona. He's also developed a special familial bond with a woman named Maria and her teenage   granddaughter Gabriela.",

   "A new king rises in the north; a Khaleesi finds new hope in the season finale.",

   "Dear John, narra la historia de John Tyree (Channing Tatum), un joven soldado que está de visita en la casa de su padre y Savannah Curtis (Amanda Seyfried) la universitaria idealista que se enamora durante las vacaciones de Pascua.",

   "En las selvas de un remoto archipiélago del Pacífico Sur, un biólogo intenta lograr algo que Charles Darwin y Ernst Mayr nunca lograron: captar la evolución en el acto de crear nuevas especies.",

   "Joan y Tom han estado casados durante muchos años. Hay una facilidad en su relación que proviene de pasar toda la vida juntos, una profundidad de amor que se expresa a través de la ternura y el humor.",

   "Brad y Kate suelen viajar cada Navidad para alejarse de sus familias, pero este año se ven obligados a cancelar sus planes y asistir a cuatro celebraciones familiares el mismo día.",

   "Durante siglos, dos razas han ido evolucionando en las profundidades de la Tierra: los vampiros y los hombres-lobo (Lycans).",

   "El afamado simbologista Robert Langdon (Tom Hanks) acude una noche al Museo del Louvre, luego del asesinato de un restaurador.",

   "Los Wünschmann no son exactamente una familia feliz, pero mamá Emma animará a todos a dejar sus diferencias de lado e ir juntos a un parque temático. Luego de una broma inocente, todos ellos son convertidos en monstruos.",

   "Una torpe cigüeña entrega un panda bebé a la dirección incorrecta. Entonces, un oso y sus amigos se embarcan en una audaz aventura para ayudar al pequeño panda a conocer finalmente a su verdadera familia y devolverlo a su hogar.",

   "Cuatro adolescentes en detención descubren una vieja consola de videojuegos. Cuando deciden jugar, son inmediatamente succionados hacia la jungla de Jumanji.",

   "Luego de su experiencia con los Vengadores, Peter Parker regresa a casa, donde vive con su tía May bajo la atenta mirada de su mentor Tony Stark.",

   "El Grinch era en su niñez un habitante más del pueblo de Villaquién, pero no era bien recibido. Hoy, vive apartado en la cima de una montaña.",

   "Cuando era niño, el mayor deseo de John se cumplió y su oso de peluche cobró vida. Pero 27 años después, el milagro dejó de ser simpático.",

   "En la tercera entrega de la película, Po debe hacer frente a dos desafíos épicos: uno, de origen sobrenatural; el otro, muy cerca de su hogar, con la aparición del que dice ser su padre biológico.",

   "Un joven científico sueña con inventar una máquina que mejore la vida de sus vecinos.",

   "Un viaje a Londres abre las puertas de una nueva aventura compartida entre Annie, su padre, sus amigos y el profesor Eli, que descubre un nuevo combustible."
]


const MyComponent = (props) =>  {
   let myRef = useRef(null);	
   let width = resolution.maincontainer.width;
   let height = resolution.maincontainer.height;
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
       <Pressable isTVSelectable='true' ref={myRef} onBlur={onBlur} onFocus={onFocus}
		   style={{zIndex: 100, backgroundColor:tilesBackground, margin:5,		 
			   height:height, width:width, 
			   shadowOffset: {width: 0,height: state.soheight},
			   shadowRadius:10,shadowColor:shadowColor,shadowOpacity: 1
			 }} >
	      <Image 
	  	  style={{width:width,height:height,
			  borderColor:'white',  borderWidth:state.bwidth, 
			  resizeMode:'stretch'}}
			  source={thumbnailImagePath[props.count]}> 
      	      </Image>
	</Pressable>
    );
}

const SimpleViewApp = () => {

    let [index, setIndex] = useState(0);
    function changeBackground (value) {
	 setIndex(value);
    }

    const addItems = (n) => {
	var arr = [];
	for (var i=n; i<n+8; i++){
		arr.push(<MyComponent count={i} change={changeBackground}></MyComponent>);
	}
	return arr;
    }

    const horizontalScrollView = (x) => {
       return (
	   <ScrollView style={{margin:5}} horizontal={true} >
	      {addItems(x)}
	   </ScrollView>
       );	       
    }

    const verticalScrollView = () => {
	return (    
	    <ScrollView style={{position:'absolute',top:320,left:5,margin:10,width:1240,height:380}}>
	      <View style={{margin:10}}>
                 <Text style={{color:'white', fontSize:18, fontWeight:'bold', textShadowRadius:5, textShadowColor:'black'}}>
	            Continue Watching
	         </Text>
              </View> 
              {horizontalScrollView(0)}	      
	      <View style={{margin:10}}>
                 <Text style={{color:'white', fontSize:18, fontWeight:'bold', textShadowRadius:5, textShadowColor:'black'}}>
	            Favorites
	         </Text>
              </View> 
              {horizontalScrollView(8)}	      
	      <View style={{margin:10}}>
                 <Text style={{color:'white', fontSize:18, fontWeight:'bold', textShadowRadius:5, textShadowColor:'black'}}>
	            Recommendations
	         </Text>
              </View> 
              {horizontalScrollView(16)}	      
	    </ScrollView>
         );	       
    }

    const posterView = () => {
	return(    
	    <View style={{zIndex:-1, width:resolution.posterView.width, 
			  height:resolution.posterView.height, left:resolution.posterView.left, 
			  top:resolution.posterView.top,
			  shadowOffset: {
				width: 5,
				height: 30
			  },
			  shadowRadius:10,
			  shadowColor:shadowColor,
			  shadowOpacity: 1,
			}}>								
	         <ImageBackground source={posterImagePath[index]} resizeMode='stretch'  style={{ zIndex:1, width:resolution.posterView.width, height:resolution.posterView.height}}>
	         </ImageBackground>  
	   </View>
         );	       
    }

    const posterContent = () => {
        let text1 = duration[index].concat("  |  ", genre[index], "  |  ", year[index])
	return (    
           <View style={{ position:'absolute',width:resolution.descriptionView.width, height:resolution.descriptionView.height, top:resolution.descriptionView.top, left:resolution.descriptionView.left}}>
                <Text style={{ color:'white', fontSize:23, marginBottom: 5, fontWeight:'bold',textShadowRadius:1, textShadowColor:'black'}}>{title[index]}</Text>
                <Text style={{ color:'white', fontSize:13, marginBottom: 10, fontWeight:'normal'}}>{text1}</Text>
                <Text style={{color:'white', fontSize:20, fontWeight:'normal', lineHeight:25}}>{description[index]}</Text>
           </View>
         );	       
    }

    return (
	<ImageBackground style = {styles.backgroundimage} source={require('./images/bg.jpg')} resizeMode='cover'>
	    {verticalScrollView()}  
	    {posterView()}  
	    {posterContent()}  
	</ImageBackground>
    );
}

const styles = StyleSheet.create({
    backgroundimage: {
	flex: 1,
	justifyContent: "flex-start",
	alignItems:'flex-start',
	width: resolution.appSize.width,
	height:resolution.appSize.height
    }
});

AppRegistry.registerComponent('SimpleViewApp', () => SimpleViewApp);

export default SimpleViewApp

