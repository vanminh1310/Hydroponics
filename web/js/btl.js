var all = document.getElementById('all');
var tb1 = document.getElementById('tb1');
var tb2 = document.getElementById('tb2');
var tb3 = document.getElementById('tb3');
var tb4 = document.getElementById('tb4');

function btn_all(){
    if (all.innerHTML==="ON_ALL"){
       all.innerText= "OFF_ALL"
       all.style.color= "white"
     document.getElementById('btn_all1').style.backgroundColor=' #4391f0'
  

    //  firebase.database().ref().child("auto_man").set("1");
    }
    
    else{
      all.innerText= "ON_ALL"
      //all.innerText= "ALL"
     all.style.color='#4391f0'
      document.getElementById('btn_all1').style.backgroundColor='white'
     
    //   firebase.database().ref().child("auto_man").set("0");
    }
  }
  function btn_tb1(){
    if (tb1.innerHTML==="ON_TB1"){
       tb1.innerText= "OFF_TB1"
       tb1.style.color= "white"
     document.getElementById('btn_tb1').style.backgroundColor=' #4391f0'
   
     //  firebase.database().ref().child("auto_man").set("1");
    }
    
    else{
      tb1.innerText= "ON_TB1"
      //tb1.innerText= "ALL"
     tb1.style.color='#4391f0'
      document.getElementById('btn_tb1').style.backgroundColor='#fff'
    //   firebase.database().ref().child("auto_man").set("0");
    }
  }
  function btn_tb2(){
    if (tb2.innerHTML==="ON_TB2"){
       tb2.innerText= "OFF_TB2"
       tb2.style.color= "white"
     document.getElementById('btn_tb2').style.backgroundColor=' #4391f0'
    //  firebase.database().ref().child("auto_man").set("1");
    }
    
    else{
      tb2.innerText= "ON_TB2"
      //tb2.innerText= "ALL"
     tb2.style.color='#4391f0'
      document.getElementById('btn_tb2').style.backgroundColor='#fff'
    //   firebase.database().ref().child("auto_man").set("0");
    }
  }
  function btn_tb3(){
    if (tb3.innerHTML==="ON_TB3"){
       tb3.innerText= "OFF_TB3"
       tb3.style.color= "white"
     document.getElementById('btn_tb3').style.backgroundColor=' #4391f0'
    //  firebase.database().ref().child("auto_man").set("1");
    }
    
    else{
      tb3.innerText= "ON_TB3"
      //tb3.innerText= "ALL"
     tb3.style.color='#4391f0'
      document.getElementById('btn_tb3').style.backgroundColor='#fff'
    //   firebase.database().ref().child("auto_man").set("0");
    }
  }
  function btn_tb4(){
    if (tb4.innerHTML==="ON_TB4"){
       tb4.innerText= "OFF_TB4"
       tb4.style.color= "white"
     document.getElementById('btn_tb4').style.backgroundColor=' #4391f0'
    //  firebase.database().ref().child("auto_man").set("1");
    }
    
    else{
      tb4.innerText= "ON_TB4"
      //tb4.innerText= "ALL"
     tb4.style.color='#4391f0'
      document.getElementById('btn_tb4').style.backgroundColor='#fff'
    //   firebase.database().ref().child("auto_man").set("0");
    }
  }