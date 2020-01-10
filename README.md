# Branch-Predicitor-Sample
A short sample of my Branch Predictor code for my Advanced Computer Architecture class. 

The goal of this program was to create software implementations of various hardware systems that a computer uses in the process of branch prediction. Almost all code that runs on your computer contains branches, which will check a condition to be true or false, then run a certain chunk of code depending on this condition. For example:

if(x==5){
  //run this code when x equals 5
}else{
  //run this code when x does not equal 5
}

These branches are neccessary for most programs, but can cause delays due to pipelining. Modern processors use instruction pipelining to have the CPU running as much as possible. For example, one part of the CPU fetches instructions, while another part decodes these instructions. Without pipelining, one instruction will go through each stage of the processor before the next instruction is fetched, like below:
![alt text](http://simplecore-ger.intel.com/techdecoded/wp-content/uploads/sites/11/figure-1-5.png)
Doing so results in wasted time: we can begin fetching the next instruction as soon as our previous instruction is done using the fetch hardware. 
![alt text](http://simplecore-ger.intel.com/techdecoded/wp-content/uploads/sites/11/figure-2-3.png)
