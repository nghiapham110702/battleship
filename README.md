Nghia Pham
I worked with Ezra Frary in this project.

I WROTE:
-FIELD.C

-NEGOTIATION.C

-AGENTTEST.C

-MESSAGETEST.C


Summary of the Lab:
In this lab, we are required to create an OG game which is call BattleBoats where
each player secretly place their boats without letting other player knows and each player
will have to take turn to guess. The game will end when one of the player are out of
boats. This BattleBoat system used the state machine in Agent.c which is responsible for
coordinating the other modules. There are various of message type are generated,
encoded, and decoded by module Message.c and sent it to agent.c and it will decide what
to do based on the message it received. When the two Uno32s are connected and which
ever Uno32 press button 4 first, that Uno32 will act as a challenger. When that Uno32 is a
challenger, it will generate a random hash, which created by Negotiation.c and send it to
other Uno32 and other uno will return a random number. The two number are xored to
determine which Uno will go first, and the accepting Uno have a chance to check if the
other uno is cheating or not. When the game start the field module is responsible to
create grid for each player and allow each player to place their boats. Agent module is
responsible for keeping track of two grids. Each player will take turn to take attack until
one of the player’s boats all sunk and the one still have some boat remaining is the
winner.

Final Result:
This lab was very difficult and fun at the same time. I think me and my partner did
a great job for this lab. The collaboration between us is not about splitting even work for
each other, but rather than if one of us is stuck with something, we solve it together,
which makes the process of finishing this lab a lot faster. I think the hardest part in this
lab is the communication between your teammate because my partner was responsible for
Agent.c and Message.c, but in order for him to test his code, I need to finish my part
since his part rely on my part. What we did is that, we didn't really care who was
responsible for which files, because I sometimes helped my partner to debug his code and
sometimes he helped me debug my code. The strategy I found testing effectively is that
we first work on the module that do not rely on any other module and then we make sure
it work properly so that when we work on other module that rely on this module, we
don’t have to worry about this module is wrong. What I learned in this Lab is that the
communication between your teammate is extremely important and the way you write the
test harness for each module. What I love about this lab is that it taught me the important
of teammate. Like you can not create something big without people collaborate each
other. What I would change about this lab, is that I wish the lab manual is more in depth
in their description and the time should be longer for this lab because me and my partner
did not really get to play the game. Overall, this lab was fun to do.
