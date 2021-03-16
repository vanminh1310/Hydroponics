var all = document.getElementById('all');
var tb1 = document.getElementById('tb1');
var tb2 = document.getElementById('tb2');
var tb3 = document.getElementById('tb3');
var tb4 = document.getElementById('tb4');

function btn_all(){
    if (all.innerHTML==="ALL"){
       all.innerText= "OFF_ALL"
       all.style.color= "white"
     document.getElementById('btn_all1').style.backgroundColor=' #4391f0'
    //  firebase.database().ref().child("auto_man").set("1");
    }
    
    else{
      all.innerText= "ALL"
      //all.innerText= "ALL"
     all.style.color='#4391f0'
      document.getElementById('btn_all1').style.backgroundColor='white'
    //   firebase.database().ref().child("auto_man").set("0");
    }
  }