# Unused functions

### ToggleControls
Appears to have been a way to change the steering handling? "Original Controls", "Accelerated steering", "0.75 Accelerated", "0.5 Accelerated", "New controls". The game is hardcoded to use "0.5 Accelerated" and this function is not called from anywhere.

### DrawSomeText / DrawSomeText2
Not accessible in the retail game, but the functions still exists. Can be invoked in a very early demo build. For each of 7 fonts, it prints 15 lines of dummy text and saves a screenshot each time.

### ToggleArrow
Does a `return` at the top of the function in the retail game. Appears to have been used for debugging. Replaces the current car model with an arrow(?). Also enables debug output showing the car location and stats.

### SetFlag2 / ToggleFlying
TBA


