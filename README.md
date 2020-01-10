# Branch-Predictor-Sample
A short sample of my Branch Predictor code for my Advanced Computer Architecture class. 

The goal of this program was to create software implementations of various hardware systems that a computer uses in the process of branch prediction. Almost all code that runs on your computer contains branches, which will check a condition to be true or false, then run a certain chunk of code depending on this condition. For example:

if(x==5){
  //run this code when x equals 5
}else{
  //run this code when x does not equal 5
}

These branches are neccessary for most programs, but can cause delays due to pipelining. Modern processors use instruction pipelining to have the CPU running as much as possible. For example, one part of the CPU fetches instructions, while another part decodes these instructions. Without pipelining, one instruction will go through each stage of the processor before the next instruction is fetched, like below:

<p align="center">
  <img src="http://simplecore-ger.intel.com/techdecoded/wp-content/uploads/sites/11/figure-1-5.png">
</p>


Doing so results in wasted time: instead, we can begin fetching the next instruction as soon as our previous instruction is done using the fetch hardware. This reduces the number of cycles from 15 to 7 in our example:

<p align="center">
  <img src="http://simplecore-ger.intel.com/techdecoded/wp-content/uploads/sites/11/figure-2-3.png">
</p>

However, this improvement comes at a cost. Now, we must pause execution when we reach a conditional branch, causing a bottleneck. This is because we need to access values that have not been stored in memory to access yet. Consider this example:

<p align="center">
  <img src="https://image.slidesharecdn.com/bp-presentation-160623083606/95/comp-architecture-branch-prediction-13-638.jpg?cb=1487938494">
</p>

Instruction 3 is a conditional branch that will jump to another part of the code labeled with the title "loop" if the value stored in register 1 equals 0. Otherwise (register 1 does not contain the value 0), we will not branch and execute instructions 4 and 5 before also jumping to "loop." Due to our pipeline, this code will take 14 cycles to complete ("jmp loop" will complete at cycle 14) rather than the expected 10 cycles if it were fetched in cycle 6. This is caused by a bubble - we need to see if we should branch or not before fetching instruction 4 in cycle 4, but this decision is not avaiable until cycle 7, when the 3rd instruction finishes executing. This results in the processor stalling for 4 cycles, where no work is done and we waste time. 

There are ways of reducing the number of cycles a bubble takes (like data forwarding), but there is no way to completely remove the stall without branch prediction. The basic idea behind branch prediction is to take the bubble out of the equation by deciding the results of the branch before it is ready. In other words, we use previous branch results to decide the outcome of our current branch. If our branch predictor sees that the last 10 branches have been taken, there is likely a high probablity that this current branch will also be taken (something that can often happen in loops). So, by using a branch predictor, which is implemented as a piece of hardware, we can tell the CPU to either continue fetching the next instruction or to jump elsewhere in the code. By doing so, we prevent bubbles from occurring. Modern predictors are very accurate, but occassionally make the wrong decision. To check for this, predictors save the addresses of code that we decided not to execute, then tell the CPU to continue executing the branch while it also fetches the instruction based on our predicition. In doing so, we can begin executing instructions while we simultaneously finish the branch instruction. Therefore, we also recieve the correct result of each predicted branch, allowing us to compare our prediction against the actual result. If the values match (both taken or both not taken), we predicted successfully, and continue executing. If our prediction was incorrect, we use the saved address of the code we did not execute to jump to where we need to be, then discard or "flush" the instructions we incorrectly started executing. 

Branch prediction is more complex than inserting a stall, but with well-designed predictors, we can achieve faster processing speeds. For this project, I was tasked with implementing several real-world branch predictors in software. I successfully wrote code for 6 different branch predictors. Here, I include a sample of my two-bit bimodal predictor to prevent future students from copying a full implementation while still showing the idea behind my code. 

A two-bit predictor works by following this state machine:

<p align="center">
  <img src="https://user.eng.umd.edu/~yavuz/enee446/images-446/2-bitbranchpredictor.gif">
</p>

We start at state 00, not taken. Therefore, our first prediction will be to not take the branch. When we obtain the true result, we use that to change states. If we were correct, we stay at state 00. If we were incorrect, we move to state 01, which also predicts not taken. This system allows us to make two consecutive incorrect predictions before changing our prediction. 
