# two-process-play-tic-tac-toe
An idea from Operating-System course's assignment

P.S.: mmap is used for creating shared memory for:
 - GameBoard variable (document current game state)
 - counter1, counter2 (both for documenting the number of steps of each player).

P.S.: Current code can't share CPU resource with others when it's waiting.
(Peterson's Algorithm may solve the problem in a software way.)
