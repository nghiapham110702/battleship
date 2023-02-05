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
