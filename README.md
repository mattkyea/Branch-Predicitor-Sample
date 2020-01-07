# Branch-Predicitor-Sample
A short sample of my Branch Predictor code for my Advanced Computer Architecture class. 

The goal of this program was to create software implementations of various hardware systems that a computer uses in the process of branch prediction. Almost all code that runs on your computer contains branches, which will check a condition to be true or false, then run a certain chunk of code depending on this condition. For example:

if(x==5){
  //run this code when x equals 5
}else{
  //run this code when x does not equal 5
}

These branches are neccessary for most programs, but can cause delays: 
