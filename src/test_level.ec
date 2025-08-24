// A .ec file is a custom file format made for the purposes of this project
// these lines are commented out, so I can put whatever I want here, and my
// file I/O system won't care!!!

// Another cool thing about .lvl files is that it's separated from the game!
// Therefore, any time you want to make changes to a level, you don't have
// recompile.

// Note:
//  Since I have a box component now, it might be worthwhile to consider
//  what would happen if I were to change the .ec so that I just use '.B'
//  instead of needing to give every box a '.G', '.R', and '.P'.
//  You know .G and .P will be true, and .R can be determined by .B's int.
//
//  I could probably do the same thing with the goals.
//  Knowing the .O number allows me to know the sprite to use?

.E
.V3 0 0 10
.G
.R 0 4
.P

.E 
.V3 1 0 0
.G
.R 9 6

.E
.V3 1 1 2
.G
.R 6 0
.P
.B 0

.E
.V3 4 2 2
.G
.R 7 0
.P
.B 1

.E
.V3 4 4 0
.G
.R 9 6

.E
.V3 4 3 0
.G
.R 9 6

.E
.V3 8 6 1
.R 12 2
.O 1

.E
.V3 5 6 1
.R 12 1
.O 0
