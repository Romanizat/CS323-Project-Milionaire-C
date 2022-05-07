# Millionaire Project in C


The topic of the project is the adaptation of the quiz "Who wants to be a millionaire?". 
Implementation has been done using the C programming language,
creating a console application with user input and files that will store data such as questions,
earnings levels, players and their achievements.




The player must answer 12 questions correctly in order to reach the sum of one million.
The questions are divided into 3 difficulty levels.


+ The first level represents the following amount range: € 500, € 1,000, € 2,000, € 5,000


+ The second level represents the following amount range: € 10,000, € 20,000, € 50,000, € 75,000


+ The third level represents the following range of amounts: € 150,000, € 250,000, € 500,000, € 1,000,000


For each level there is a list of questions of appropriate difficulty. 
The questions are randomly displayed to the player.


A player is entitled to four types of help (Jokers):
+ Phone a Friend, 
+ Half-Half, 
+ Ask The Audience,
+ Replace question (*only if the player has passed the first difficulty level*). 




When a player passes a difficulty level, 
the value he wins at the end of that level then becomes a guaranteed amount. 


If any of the following questions are answered incorrectly, the game ends and the player receives only 
the guaranteed amount. 


If a player decides to give up, he can leave with the curren amount before answering the question 


The achieved result is written to the file.


### *Gameplay Instructions:*

---

[//]: # (TODO)

### *Build Instructions:*

---

+ **CLion IDE**:
  + Edit the run configuration and set the **Working directory** to the *Working directory* folder 
  which is located in this project.


## ***Disclaimer***

---

This project was done as an assigment for a University course - *CS323 - C/C++ Programming Languages*