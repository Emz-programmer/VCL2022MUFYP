
import React from 'react';
import axios from 'axios';
import imageToBase64 from 'image-to-base64/browser';
import './App.css';

const api = axios.create({
    baseURL: 'http://149.157.111.88:2022/2022fypVCL'

});

class App extends React.Component{
    constructor(){
        super();
        this.state = {
            image: null,
            imageSrc: null,
            imageB64: "",
            buildingName: ""
          };        
    }
    getImage = async() => {
        console.log("Locating Building")  
        let res = await api.post('/', await imageToBase64(this.state.image))
        this.state.buildingName=res.data
        
        document.getElementById("answer").innerHTML="<p>Building is: "+this.state.buildingName+"<p/>"        
    }
    /*testVCL = async() =>{
        var testOutput="";
        const labels = ["Arts", "Iontas", "Engineering", "Eolas", "John Hume"]
        for(var i=1;i<=50;i++){
            var start = performance.now()
            const img = require ('./images/testing/testing'+i+'.jpg')
            let res = await api.post('/', await imageToBase64(img))
            var end = performance.now()
            testOutput+= "\""+i+"\","+res.data+","+labels[parseInt((i-1)/10)]+","+(end-start)+"\n"
        }
        const element = document.createElement("a");
        const file = new Blob([testOutput], {
        type: "text/plain"
        });
        element.href = URL.createObjectURL(file);
        element.download = "testOutput.csv";
        document.body.appendChild(element);
        element.click();
        console.log("Testing Complete!");
    }*/

    onImageChange = event => {
        if (event.target.files && event.target.files[0]) {
          let img = event.target.files[0];
          this.setState({
            image: URL.createObjectURL(img),
            imageSrc: img,
            
          });
        }
      };
    render(){
        return(
            <div className='App'>
                <h1>Visual Campus Locator</h1><br></br>
                <p>The VCL is a tool for recognising buildings on Maynooth campus</p><br/>
                
                <div>
                    <input type="file" id="file-input" name="Upload Image" onChange={this.onImageChange}/>
                    <br/>
                    <img src={this.state.image} width="200"/>
                </div>
                <br/>         
                <button onClick={this.getImage}>Find Building</button>
                
                <br/>
                <div id="answer">

                </div><br/>
                


            </div>
        )
    }
}
export default App;
