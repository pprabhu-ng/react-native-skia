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
    console.log("content:" , content);	

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

